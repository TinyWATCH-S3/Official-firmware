/**
 * @file tinywatch.cpp
 * @details This is the main firmware starting point. The code includes functionality for handling Wi-Fi setup, display initialization, and interacting with peripherals like the buzzer, battery, IMU, and RTC.
 *
 */

#include "tinywatch.h"

#include <SPI.h>
#include <Wire.h>

#include "peripherals/battery.h"
#include "peripherals/buzzer.h"
#include "peripherals/imu.h"
#include "peripherals/rtc.h"
#include "peripherals/haptics.h"
#include "tw_faces/face_Notifications.h"
#include "tw_faces/face_AppList.h"
#include "web/webserver.h"
#include "web/wifi_controller.h"
#include "web/wifi_setup.h"
#include <PubSubClient.h>

#include "activity.h"
#include "settings/settings.h"
#include <LittleFS.h>

#include "utilities/logging.h"
#include "utilities/capture.h"

// CPU speeds
#define CPU_LOW_FREQ 40
#define CPU_HIGH_FREQ 80
#define CPU_LOW_FREQ_WIFI 80
#define CPU_HIGH_FREQ_WIFI 80

#define VBUS_SENSE 34
#define BUZZER 18
#define PWR_SHUTDOWN 21
#define USER_FLASH_CS 33
#define TFT_LED 13

#define WAKE_REASON_TOUCH BIT64(GPIO_NUM_11)
#define WAKE_REASON_RTC_ALARM BIT64(GPIO_NUM_4)
#define WAKE_REASON_FG_ALERT BIT64(GPIO_NUM_14)
#define WAKE_REASON_BMI270_WRIST BIT64(GPIO_NUM_7)

RTC_DATA_ATTR int rtc_mem_watch_clock_screen = 0; // 0 Digital, 1: Analog
RTC_DATA_ATTR bool rtc_mem_watch_flipped = false; // True: Rotated, False: Default
RTC_DATA_ATTR bool rtc_mem_watch_handed = true;	  // True: Left Handed, False: Right Handed

void setup()
{
	pinMode(0, INPUT_PULLUP);
	pinMode(PWR_SHUTDOWN, OUTPUT);
	digitalWrite(PWR_SHUTDOWN, LOW);

	tinywatch.messages.clear();

	Serial.begin(115200);
	Serial.setDebugOutput(true); // sends all log_e(), log_i() messages to USB HW CDC
	Serial.setTxTimeoutMs(0);	 // sets no timeout when trying to write to USB HW CDC

	tinywatch.log_system_message("TinyWATCH S3");
	tinywatch.log_system_message("FW " + tinywatch.version_firmware + " " + tinywatch.version_year);
	tinywatch.log_system_message(String(ESP.getChipModel()) + " R" + String(ESP.getChipRevision()) + " " + String(ESP.getChipCores()) + "C");

	if (!LittleFS.begin(true))
	{
		error_println("LittleFS failed to initialise");
		tinywatch.log_system_message("LittleFS Error!");
		return;
	}
	else
	{
		delay(100);
		settings.load();
	}

	tinywatch.log_system_message("Loaded settings");

	display.init_screen();

	analogWriteResolution(8);

	Wire.begin(8, 9);

	// Initialise the Buzzer
	init_buzzer(BUZZER, 4);
	esp_register_shutdown_handler([]() { deinit_buzzer(BUZZER); });

	bool was_asleep = tinywatch.was_sleeping();

	if (was_asleep)
	{
		// Temporarily set the settings from rtc memory save state
		settings.config.clock_face_index = rtc_mem_watch_clock_screen;
		settings.config.flipped = rtc_mem_watch_flipped;
		settings.config.left_handed = rtc_mem_watch_handed;
		tinywatch.log_system_message("Wake from sleep");
	}
	else
	{
		tinywatch.log_system_message("Clean boot");
	}

	// Start the RTC & Battery early as they are needed for wake fom sleep
	battery.init();
	rtc.init();
	imu.preinit(was_asleep);

	// Start the haptics peripheral if present, so we can buzz on wake or start if enabled.
	// If haptics fails to initialise, then we are on older HW than P7.
	tinywatch.hw_revision_p7_or_later = haptics.init();

	if (was_asleep)
	{
		// Wake up the peripherals because we were sleeping!
		battery.set_hibernate(false);
	}

	// battery.update_interrupt_values(settings.config.battery.low_perc, settings.config.battery.low_volt_warn);

	display.set_backlight(0, true);

	// Create the UI and show the boot screen is it's a clean boot
	display.createFaces(was_asleep);

	display.update_rotation();

	if (was_asleep)
	{
		// Process any POST DS callbacks onw that we have faces!
		for (size_t i = 0; i < tinywatch.post_ds_callbacks.size(); i++)
		{
			if (tinywatch.post_ds_callbacks[i] != nullptr)
				tinywatch.post_ds_callbacks[i]();
		}

		// Wake up the peripherals because we were sleeping!

		// work out why we were woken up and do something about it
		// 0: Touched Screen
		// 1: RTC Alarm
		// 2: FG Alert
		// 3: IMU Wrist
		int wake_reason = tinywatch.wake_reason = tinywatch.woke_by();

		info_println("Woke from sleep by " + String(wake_reason));

		// String bat_volt = String(battery.get_voltage(true));
		tinywatch.log_system_message("BAT: " + String(battery.get_voltage(true)) + "V");

		if (wake_reason == 1)
		{
			// show alarm status and then clear the RTC alarm flag
			tinywatch.log_system_message("WOKE RTC");
			rtc.process_alarms();
			// tinywatch.log_system_message("INT TRIG?" + String(rtc.rtc.isInterruptDetected(INTERRUPT_ALARM)));
		}
		else if (wake_reason == 2)
		{
			if (battery.is_high())
			{
				tinywatch.log_system_message("WOKE FG HIGH V");
				tinywatch.block_update_low_battery = false;
			}
			else if (battery.is_low(false))
			{
				tinywatch.log_system_message("WOKE FG LOW V");
				tinywatch.block_update_low_battery = true;
			}
			else if (battery.is_low(true))
			{
				tinywatch.log_system_message("WOKE FG LOW %");
				tinywatch.block_update_low_battery = true;
			}

			battery.clear_alert_status();
			tinywatch.low_battery_update_check = millis();

			// If our voltage is too low, we exit startup and show a blocking battery screen until the battery charges
			// We only do this if we are running from battery, with no USB connected
			// if (!tinywatch.vbus_present() && tinywatch.block_update_low_battery)
			// 	return;
		}
		else
		{
			// We woke from touch, so nothing really to do
			tinywatch.log_system_message("WOKE TOUCH");
		}
	}
	else
	{
		bool ok_to_show_watch_face = true;

		if (rtc.requiresNTP || !settings.has_wifi_creds() || !settings.has_country_set())
		{
			// TODO: We really need to make this more robust, with retried, and proper error handling.
			if (!settings.has_wifi_creds())
			{
				tinywatch.log_system_message("no SSID");
				ok_to_show_watch_face = false;
				info_println("Starting WiFi AP");
				display.update_boot_face(WIFI_SETUP);
				WiFi.disconnect(true);
				delay(1000);
				wifiSetup.start();
			}
			else
			{
				if (!settings.has_country_set())
					tinywatch.log_system_message("no country set");
				else
					tinywatch.log_system_message("requires NTP");

				display.set_display_state(LOADING, "Calculating the time!");

				// We want to hold wifi on for this as it's multiple requests and we dont want to have to connect/disconnect for each
				// We also want to show a spinning/loading icon - looks ugly right now..
				wifi_controller.wifi_blocks_display = true;

				// Grab our public IP address, and then get out UTC offset and country and suburb.
				wifi_controller.add_to_queue("http://api.ipify.org", [](bool success, const String &response) { tinywatch.get_public_ip(success, response); });
			}
		}

		// We show this last on a clean boot, but if we woke from sleeping, the clock should already be up!
		if (ok_to_show_watch_face)
		{
			// Hold the boot screen for a smidge
			delay(1000);
			display.show_watch_from_boot();
		}
	}

	// load the activity data
	activity.load();

	if (was_asleep)
	{
		// Do the steps last, after activity is loaded
		imu.set_hibernate(false);
		if (settings.config.imu_process_steps)
		{
			imu.process_steps(true);
		}
		haptics.play_trigger(Triggers::WAKE);
	}
	else
	{
		haptics.play_trigger(Triggers::STARTUP);
	}

	// Start the rest of the peripherals
	imu.init();

	// settings.print_file();
	// rtc.print_file();
	// rtc.create_alarm(AlarmType::A_HOURLY, rtc.get_mins() + 2);
}

void loop()
{
	// Nothing gets past this...  If the battery is too low, the watch cannot operate.
	// if (!tinywatch.vbus_present() && tinywatch.block_update_low_battery)
	// {
	// 	if (millis() - tinywatch.low_battery_update_check > 2000)
	// 	{
	// 		tinywatch.low_battery_update_check = millis();

	// 		display.show_low_battery();
	// 		display.set_backlight(2, true);

	// 		if (tinywatch.vbus_present())
	// 		{
	// 			// User has plugged in the USB, so the battery is charging...
	// 			tinywatch.block_update_low_battery = false;
	// 			display.show_watch_from_boot();
	// 			return;
	// 		}
	// 		else
	// 		{
	// 			float bat_volt = battery.get_voltage(true);
	// 			if (bat_volt > 2.0 && bat_volt < settings.config.battery.low_volt_cutoff)
	// 			{
	// 				// battery is too low, so shutting down the watch power, but we need to make sure
	// 				// it's not low because it's not initialised yet on wake from sleep or boot
	// 				digitalWrite(PWR_SHUTDOWN, HIGH);
	// 			}
	// 		}
	// 	}
	// 	return;
	// }

	// Process non sleeping battery low state every 5 seconds
	if (!tinywatch.vbus_present())
	{
		if (millis() - tinywatch.low_battery_update_check > 5000)
		{
			tinywatch.low_battery_update_check = millis();
			if (battery.is_low(true) || battery.is_low(false))
			{
				// Was triggered by an interrupt - most likely under voltage
				info_println("battery low alert");
				info_print(battery.get_voltage(true));
				info_println("V");

				tinywatch.log_system_message("loop LOW V: " + String(battery.is_low(false)));
				tinywatch.log_system_message("loop LOW %: " + String(battery.is_low(true)));
				battery.clear_alert_status();
				// tinywatch.block_update_low_battery = true;
			}
		}
	}

	// // Wifi will crash if CPU speed is less than 80
	// // We might not need to call this in the loop though
	// if (wifi_controller.is_busy() || web_server.is_running())
	// {
	//     setCpuFrequencyMhz(80);
	// }

	// Process the wifi controller task queue
	// Only processes every 1 second inside it's loop
	wifi_controller.loop();

	// Process the RTC alarm interrupt detection
	rtc.process_alarms();

	// If we are doing a loading activity, halt the rest of the loop
	if (display.get_current_display_state() == LOADING)
	{
		return;
	}

	// TODO: Nasty and need to find a better way
	if (wifi_controller.wifi_blocks_display)
	{
		return;
	}

	display.update_rotation();
	// Constantly call saving settting
	// If it's been less than 1mins, or there's nothing to save, it wil bail
	settings.save(false);

	if (settings.config.imu_process_steps)
	{
		imu.process_steps();
	}

	if (settings.config.imu_process_wrist)
		imu.process_wrist_gestures();

	// Check if we have swiped or clicked
	display.process_touch();

	// Update the current face based on it's own update period
	display.update_current_face();

	// If the user has pressed IO0/BOOT then we want to immediately go into deep sleep
	// Extra prevention around IO0 when using Apps - specifically the I2S Microphone that triggers IO0 :(
	// pinMode(0, ((face_applist.app_running() || millis() - tinywatch.go_to_sleep_timer_io0 < 750) ? OUTPUT : INPUT_PULLUP));
	// if (!face_applist.app_running() && (millis() - tinywatch.go_to_sleep_timer_io0 > 1500))
	if (!face_applist.app_running())
	{
		if (tinywatch.hw_revision_p7_or_later && digitalRead(0) == LOW)
			tinywatch.go_to_sleep();
	}

	// If the USB was just plugged in,
	if (tinywatch.vbus_changed())
	{
		// Beep speaker is enabled
		if (settings.config.audio.charge)
			BuzzerUI({{2000, 50}});

		// Buzz haptics if enabled
		haptics.play_trigger(Triggers::CHARGE);
	}

	yield();
}

void TinyWATCH::pause_can_sleep()
{
	go_to_sleep_timer_io0 = millis();
}

void TinyWATCH::notify_alarm()
{
	info_println("Alarm!!!!!");
	haptics.play_trigger(Triggers::ALARM);
	Buzzer({{262, 210}, {1851, 150}, {523, 150}});
	// settings.config.audio.audio
}

bool TinyWATCH::update_available() { return (version_latest > version_build); }

void TinyWATCH::set_cpu_frequency(uint32_t freq, CPU_SPEED speed)
{
	// Wifi will crash if the CPU speed is less than 80
	if (wifi_controller.is_busy() || web_server.is_running())
	{
		setCpuFrequencyMhz(80);
		return;
	}

	if (speed == CPU_CHANGE_LOW)
	{
		if (freq > CPU_LOW_FREQ)
			setCpuFrequencyMhz(freq);
		else
			setCpuFrequencyMhz(CPU_LOW_FREQ);
	}
	else if (speed == CPU_CHANGE_HIGH)
	{
		if (freq < CPU_HIGH_FREQ)
			setCpuFrequencyMhz(freq);
		else
			setCpuFrequencyMhz(CPU_HIGH_FREQ);
	}
}

bool TinyWATCH::was_sleeping() { return (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1); }

int TinyWATCH::woke_by()
{
	if (esp_sleep_get_ext1_wakeup_status() == WAKE_REASON_RTC_ALARM)
		return 1;
	else if (esp_sleep_get_ext1_wakeup_status() == WAKE_REASON_FG_ALERT)
		return 2;
	else if (esp_sleep_get_ext1_wakeup_status() == WAKE_REASON_BMI270_WRIST)
		return 3;
	else
		return 0;
}

void TinyWATCH::go_to_sleep()
{
	// if the web server is running, we don't want to go to sleep...
	if (web_server.is_running())
	{
		info_println("Web server is running, exiting call to sleep");
		return;
	}

	if (wifi_controller.is_connected())
		wifi_controller.disconnect(true);

	if (wifi_controller.is_busy())
	{
		info_println("Cant sleep yet, wifi is busy!");
		return;
	}

	for (size_t i = 0; i < pre_ds_callbacks.size(); i++)
	{
		if (pre_ds_callbacks[i] != nullptr)
			pre_ds_callbacks[i]();
	}

	// Dont call this if the task was not created!
	wifi_controller.kill_controller_task();

	rtc_mem_watch_clock_screen = settings.config.clock_face_index;
	rtc_mem_watch_flipped = settings.config.flipped;
	rtc_mem_watch_handed = settings.config.left_handed;

	display.kill_backlight_task();
	digitalWrite(TFT_LED, 0);
	deinit_buzzer(BUZZER);
	battery.set_hibernate(true);

	if (settings.config.imu_process_steps)
	{
		imu.process_steps(true);
	}

	imu.set_hibernate(false);
	settings.save(true);
	delay(200);
	activity.save(true);
	delay(500); // no delay and it sometimes wakes up immediately

	LittleFS.end();

	// esp_sleep_enable_ext1_wakeup(WAKE_REASON_BMI270_WRIST, ESP_EXT1_WAKEUP_ALL_LOW);
	// esp_sleep_enable_ext1_wakeup(WAKE_REASON_TOUCH | WAKE_REASON_RTC_ALARM | WAKE_REASON_FG_ALERT | WAKE_REASON_BMI270_WRIST, ESP_EXT1_WAKEUP_ANY_LOW);
	esp_sleep_enable_ext1_wakeup(WAKE_REASON_TOUCH | WAKE_REASON_RTC_ALARM | WAKE_REASON_BMI270_WRIST, ESP_EXT1_WAKEUP_ANY_LOW);
	esp_deep_sleep_start();
}

bool TinyWATCH::vbus_present()
{
	bool vbus = digitalRead(VBUS_SENSE);
	// info_println("vbus " + String(vbus));
	return (vbus);
}

/// @brief Detect if 5V as gone from NO to YES. Look complicated, but we dont want to check a change in both ways, just a change from NO to YES.
/// @return if 5V was not present and now is.
bool TinyWATCH::vbus_changed()
{
	bool detected = false;
	bool vbus = digitalRead(VBUS_SENSE);
	if (vbus && vbus != is_5V_detected)
		detected = true;

	is_5V_detected = vbus;
	return detected;
}

void TinyWATCH::log_system_message(String txt)
{
	tinywatch.messages.push_back(log_message((millis() / 1000), txt));
}

void TinyWATCH::get_public_ip(bool success, const String &response)
{
	// info_println("Callback executed. Success: " + String(success ? "TRUE" : "FALSE") + ", Response: " + String(response));
	wifi_controller.add_to_queue("https://ipapi.co/" + response + "/json/", [](bool success, const String &response) { tinywatch.get_and_update_utc_settings(success, response); });
}

void TinyWATCH::get_and_update_utc_settings(bool success, const String &response)
{
	info_println("Callback executed. Success: " + String(success ? "TRUE" : "FALSE") + ", Response: " + String(response));

	// don't hold wifi on anymore
	settings.config.wifi_start = false;

	if (success && !response.isEmpty())
	{
		json data = json::parse(response);

		settings.config.city = data["city"].get<String>();
		settings.config.country = data["country_code"].get<String>();
		String utc_offset = data["utc_offset"].get<String>();

		info_printf("city: %s\n", settings.config.city);
		info_printf("country: %s\n", settings.config.country);
		info_printf("utc: %d\n", utc_offset);

		const char *utc_offset_data = utc_offset.c_str();

		int32_t calc_offset = ((utc_offset_data[1] - '0') * 10) + (utc_offset_data[2] - '0'); // hour
		calc_offset *= 60;
		calc_offset += ((utc_offset_data[3] - '0') * 10) + (utc_offset_data[4] - '0'); // minute
		calc_offset = (calc_offset * 60 * ((utc_offset_data[0] == '-' ? -1 : 1)));

		settings.config.utc_offset = calc_offset / 3600;

		info_printf("utc fixed: %d\n", settings.config.utc_offset);

		settings.save(true);
		tinywatch.log_system_message("UTC set " + String(settings.config.utc_offset));

		bool obtained = false;
		uint8_t retries = 3;
		while (!obtained && retries > 0)
		{
			obtained = rtc.set_time_from_NTP(settings.config.utc_offset);
			retries--;
		}
		if (!obtained)
		{
			error_println("Failed to set NTP time");
			tinywatch.log_system_message("Failed NTP time");
		}
		else
		{
			tinywatch.log_system_message("NTP time set");
		}
	}
	else
	{
		error_println("Failed to obtain UTC details");
		tinywatch.log_system_message("Failed UTC details");
	}

	wifi_controller.wifi_blocks_display = false;
	display.set_display_state(NORMAL, "");

	if (!settings.config.wifi_start)
		wifi_controller.disconnect(false);
}

void TinyWATCH::ds_remove(const char *key)
{
	nvs.begin(deep_sleep_store_name);
	nvs.remove(key);
	nvs.end();
}

void TinyWATCH::ds_store_string(const char *key, const char *data)
{
	nvs.begin(deep_sleep_store_name);
	nvs.putString(key, data);
	nvs.end();
}

void TinyWATCH::ds_store_int32(const char *key, int32_t data)
{
	nvs.begin(deep_sleep_store_name);
	nvs.putInt(key, data);
	nvs.end();
}

void TinyWATCH::ds_store_uint32(const char *key, uint32_t data)
{
	nvs.begin(deep_sleep_store_name);
	nvs.putUInt(key, data);
	nvs.end();
}

String TinyWATCH::ds_retrieve_string(const char *key)
{
	nvs.begin(deep_sleep_store_name);
	String data = nvs.getString(key);
	nvs.end();

	return data;
}

int32_t TinyWATCH::ds_retrieve_int32(const char *key)
{
	nvs.begin(deep_sleep_store_name);
	int32_t data = nvs.getInt(key);
	nvs.end();

	return data;
}

uint32_t TinyWATCH::ds_retrieve_uint32(const char *key)
{
	uint32_t data = 99;
	nvs.begin(deep_sleep_store_name);
	if (nvs.isKey(key))
		data = nvs.getUInt(key, 101);
	nvs.end();

	return data;
}

void TinyWATCH::register_pre_ds_callback(_CALLBACK_DS _callback)
{
	pre_ds_callbacks.push_back(_callback);
	info_printf("Adding item to PRE DS callbacks Now %d items\n", pre_ds_callbacks.size());
}

void TinyWATCH::register_post_ds_callback(_CALLBACK_DS _callback)
{
	post_ds_callbacks.push_back(_callback);
	info_printf("Adding item to POST DS callbacks Now %d items\n", pre_ds_callbacks.size());
}

TinyWATCH tinywatch;
