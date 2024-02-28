/**
 * @file display.cpp
 * @details The `Display` class is responsible for managing the display and touch functionalities of the TinyWATCH S3 project. It includes methods for initializing the screen, handling touch interactions, managing faces, and controlling backlight levels.
 *
 */

#include "display.h"
#include "Arduino.h"
#include "esp32-hal-cpu.h"
#include "tinywatch.h"
#include "tw_widgets/tw_widget.h"

Display display;

// Peripherals
#include "peripherals/battery.h"
#include "peripherals/buzzer.h"
#include "peripherals/imu.h"

// Faces Clock
#include "tw_faces/face_Watch_DefaultAnalog.h"
#include "tw_faces/face_Watch_DefaultDigital.h"

// Custom Faces
#include "tw_faces/face_Watch_CustomBinary.h"
#include "tw_faces/face_Watch_CustomWindows.h"

// Faces General
#include "tw_faces/face_AppList.h"
#include "tw_faces/face_BatteryEmpty.h"
#include "tw_faces/face_Boot.h"
#include "tw_faces/face_IMU.h"
#include "tw_faces/face_Notifications.h"
#include "tw_faces/face_Settings.h"
#include "tw_faces/face_System.h"
#include "tw_faces/face_WatchSettings.h"

// Widgets
#include "tw_widgets/widget_ActivityRing.h"
#include "tw_widgets/widget_Battery.h"
#include "tw_widgets/widget_ESP32.h"
#include "tw_widgets/widget_Message.h"
#include "tw_widgets/widget_Wifi.h"
// #include "tw_widgets/widget_OpenWeather.h"

// Controls
#include "tw_controls/control_Button.h"
#include "tw_controls/control_Label.h"
#include "tw_controls/control_Toggle.h"
#include "tw_controls/control_Value.h"
#include "tw_controls/control_ValueSlider.h"

// Apps
#include "tw_apps/app_Compass.h"
#include "tw_apps/app_Microphone.h"
#include "tw_apps/tw_app.h"
#include "tw_apps/app_Empty.h"

// Other
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "settings/settings.h"

#define TFT_LED 13

// touch
#define TP_SDA 5
#define TP_SCL 10
#define TP_RST 12
#define TP_IRQ 11

// Managing the backlight is done in a seperate thread
static void process_backlight(void *param);
static TaskHandle_t backlightTaskHandle;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite canvas[] = {TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite loading_screen_canvas = TFT_eSprite(&tft);

// Touch is initialised on I2C bus 2, using IO5 for SDA, and IO10 for SCL
cst816t touchpad(TP_RST, TP_IRQ);

String dirNames[7] = {"UP", "RIGHT", "DOWN", "LEFT", "CLICK", "CLICK_DBL", "CLICK_LONG"};

unsigned long last_finger_move = 0;
unsigned long last_touch = 0;
unsigned long dbl_touch[2] = {0};
unsigned long drag_rate = 0;

int16_t deltaX = 0, deltaY = 0;
int16_t moved_x = 0, moved_y = 0;
int16_t startX = 0, startY = 0;
uint touchTime = 0;
bool isTouched = false;
bool last_was_click = false;
bool last_was_long = false;
bool prevent_long_press = false;

static std::vector<tw_face *> clock_faces;

void Display::init_screen()
{
	if (touchpad.begin(mode_change))
	{
		info_print("Touch started\nVersion: ");
		info_println(touchpad.version());
	}
	tft.init();
	tft.setRotation(settings.config.flipped ? 2 : 0);
	tft.setPivot(120, 140);
	tft.fillScreen(TFT_BLACK);

	for (int c = 0; c < 2; c++)
	{
		canvas[c].setFreeFont(RobotoMono_Regular[3]);
		canvas[c].setSwapBytes(true);
		canvas[c].createSprite(240, 280);
		canvas[c].setRotation(0);
		canvas[c].setTextDatum(4);
	}

	loading_screen_canvas.createSprite(64, 64);

	xTaskCreate(process_backlight, "backlight", configMINIMAL_STACK_SIZE * 3, nullptr, 3, &backlightTaskHandle);
}

void Display::kill_backlight_task()
{
	info_println("Killing backlight RTOS task!");
	vTaskDelete(backlightTaskHandle);
}

void Display::add_clock_face(tw_face *face)
{
	clock_faces.push_back(face);
	info_println("Now " + String(clock_faces.size()) + " clock faces");
}

void Display::set_current_face(tw_face *face) { current_face = face; }
tw_face *Display::get_current_face() { return current_face; }

/**
 * @brief Before we switch the current clock face to a new clock face, we need to copy over the current clock faces navigation to the new face.
 *
 * @param should_draw
 * @return tw_face*
 */
tw_face *Display::set_current_clock_face(bool should_draw)
{
	tw_face *new_clock_face = clock_faces[settings.config.clock_face_index];
	if (current_face != nullptr && current_face->is_face_clock_face() && current_face != new_clock_face)
	{
		for (int i = 0; i < 4; i++)
			new_clock_face->set_single_navigation((Directions)i, current_face->navigation[i]);
	}

	current_face->reset_cache_status();
	current_face = new_clock_face;

	if (should_draw)
		current_face->draw(true);

	return current_face;
}

/**
 * @brief Cycle the clock face number we are displaying. This is called by double tapping the current clock face being shown
 */
void Display::cycle_clock_face()
{
	settings.config.clock_face_index++;
	// Hacky way to check the int against the number of elements in the enum
	if (settings.config.clock_face_index == clock_faces.size())
		settings.config.clock_face_index = 0;

	set_current_clock_face(true);
}

/**
 * @brief Adjust the rotation of the TFT based on the flipped user setting
 *
 */
void Display::update_rotation() { tft.setRotation(settings.config.flipped ? 2 : 0); }

void Display::set_display_state(display_states state, String message)
{
	current_display_state = state;

	// dont ask...  more to come here...
	if (state == LOADING)
	{
		showing_loading_icon = false;
	}
	else
	{
		showing_loading_icon = false;
	}
}

void Display::show_loading_icon()
{
	loading_screen_canvas.createSprite(64, 64);
	loading_screen_canvas.setPivot(32, 32);
	loading_screen_canvas.pushImage(0, 0, 64, 64, bitmap_loading_icon);
	loading_screen_canvas.pushRotated(loading_screen_rotation, TFT_BLACK);

	loading_screen_canvas.deleteSprite();
	loading_screen_rotation += 30;
}

void Display::force_save() { settings.save(true); }

void Display::show_watch_from_boot()
{
	if (current_face == nullptr)
		set_current_clock_face(false);

	current_face->draw(true);

	last_touch = millis();
	dbl_touch[0] = millis();
	dbl_touch[1] = millis();
}

/**
 * @brief This is the entry point that creates all of the faces and widgets and controls, and hooks them all together. All face to face navigation is also setup here.
 *
 * This also shows the watch boot face on startup, if the watch is booting from power up, otherwise, the clock wil be shows through a fast path is the watch is waking from sleep.
 *
 * @param was_sleeping
 */
void Display::createFaces(bool was_sleeping)
{
	tft.setSwapBytes(true);
	tft.fillScreen(TFT_BLACK);

	update_rotation();

	// Now create the boot and watch tw_face
	face_boot.add("boot", 0, 80);

	if (!was_sleeping)
	{
		face_boot.draw(true);
		BuzzerUI({{262, 210}, {1851, 150}, {523, 150}});
	}

	backlight_level = 0;
	set_backlight(0, true);

	WidgetBattery *wBattery = new WidgetBattery();
	wBattery->create("Battery", 210, 7, 40, 40, 1000);

	WidgetActivityRing *wActivity = new WidgetActivityRing();
	wActivity->create("Activity", 40, 218, 40, 40, 1000);

	// Used to show the current CPU frequency on a watch face
	// WidgetESP32 * wESP32 = new WidgetESP32();
	// wESP32->create("ESP32", 50, 5, 32, 24, 500);

	// Used to show the WiFi icon if WiFi is enabled
	// Needs to be refactored once the new WiFi threaded manager is ready
	WidgetWifi *wWifi = new WidgetWifi();
	wWifi->create("Wifi", 30, 7, 40, 40, 1000);

	// Faces that are "Clocks" that you want to be switchable with a dbl click need to use `add_clock()` instead of `add()`
	// or they will not be added to the clock list for cycling.
	face_watch_default_analog.add_clock("Clock_Def_Analog", 1000);
	face_watch_default_analog.add_widget(wBattery);
	face_watch_default_analog.add_widget(wActivity);
	face_watch_default_analog.add_widget(wWifi);

	face_watch_default_digital.add_clock("Clock_Def_Digital", 1000);
	// face_watch_default_digital.add_widget(wBattery);
	// face_watch_default_digital.add_widget(wActivity);
	// face_watch_default_digital.add_widget(wWifi);

	face_watch_custom_binary.add_clock("Clock_Custom_Binary", 1000);
	face_watch_custom_binary.add_widget(wBattery);
	face_watch_custom_binary.add_widget(wActivity);
	face_watch_custom_binary.add_widget(wWifi);

	face_watch_custom_windows.add_clock("Clock_Custom_Windows", 1000);
	// face_watch_custom_windows.add_widget(wBattery);
	// face_watch_custom_windows.add_widget(wActivity);
	// face_watch_custom_windows.add_widget(wWifi);

	// needs a default clock face so it won't crash
	// all clock faces need to be initialised before this
	tw_face *current_clock_face = set_current_clock_face(false);

	// face_watch.add_widget(wESP32);

	// If we were sleeping, show the clock here, before processing everything else.
	// Otherwise the clock will be shown from main.cpp once all boot processing is done.
	// Michael H is going to HATE how this logic is split!
	if (was_sleeping)
		show_watch_from_boot();

	// Create Applications
	app_microphone.add("FFT", 25, 160);
	app_compass.add("Compass", 100, 160);
	app_empty.add("Empty", 1000, 40);

	// Create Applications Face
	face_applist.add("APPS", 1000, 40);
	face_applist.set_single_navigation(LEFT, current_clock_face);
	face_applist.add_app(&app_microphone);
	face_applist.add_app(&app_compass);
	face_applist.add_app(&app_empty);

	// Create Faces
	face_imu.add("IMU", 100, 80);

	face_notifications.add("Messages", 1000, 80);
	face_notifications.set_scrollable(false, true);

	face_system.add("System Info", 0);
	face_system.set_scrollable(false, true);
	face_system.set_single_navigation(UP, &face_notifications);

	face_settings.add("Settings", 0, 80);
	face_settings.set_scrollable(false, true);
	face_settings.set_single_navigation(LEFT, &face_boot);

	// face_watch is a pointer to the current clock face
	current_clock_face->set_single_navigation(LEFT, &face_settings);
	current_clock_face->set_single_navigation(UP, &face_imu);
	current_clock_face->set_single_navigation(DOWN, &face_notifications);

	// WidgetOpenWeather * wWeather = new WidgetOpenWeather();
	// wWeather->create("Weather", 10, 180, 90, 90, 5000);
	// face_watch.add_widget(wWeather);

	// example of a notification widget - non functional for now
	// WidgetMessage * wMessage = new WidgetMessage();
	// wMessage->create("Message", 0, 0, 200, 50, 1000);
	// face_notifications.add_widget(wMessage);

	ControlToggle *cToggle = new ControlToggle();
	cToggle->create("24 Hour", "OFF", "OK", 30, 60, 80, 30);
	cToggle->set_data(&settings.setting_time_24hour);

	ControlToggle *cToggle2 = new ControlToggle();
	cToggle2->create("Handed", "RIGHT", "LEFT", 130, 60, 80, 30);
	cToggle2->set_data(&settings.setting_left_handed);

	ControlToggle *cToggle3 = new ControlToggle();
	cToggle3->create("Rotation", "NORMAL", "FLIPPED", 30, 130, 80, 30);
	cToggle3->set_data(&settings.setting_flipped);
	cToggle3->set_callback(display.update_rotation);

	ControlLabel *cLabel1 = new ControlLabel();
	cLabel1->create("AUDIO", 120, 180);

	ControlToggle *cToggle4 = new ControlToggle();
	cToggle4->create("UI Sound", "OFF", "ON", 30, 210, 80, 30);
	cToggle4->set_data(&settings.setting_audio_ui);

	ControlToggle *cToggle5 = new ControlToggle();
	cToggle5->create("Alarm", "OFF", "ON", 130, 210, 80, 30);
	cToggle5->set_data(&settings.setting_audio_alarm);

	ControlToggle *cToggle6 = new ControlToggle();
	cToggle6->create("Date Format", "DMY", "MDY", 130, 130, 80, 30);
	cToggle6->set_data(&settings.setting_time_dateformat);

	// ControlButton * cButton1 = new ControlButton();
	// cButton1->create("SAVE", 70, 250, 100, 40);
	// cButton1->set_callback(force_save);

	// ControlValue * cValue1 = new ControlValue();
	// cValue1->create("Click Value", "", "", 120, 250, 200, 50);

	// ControlValueSlider * cValue2 = new ControlValueSlider();
	// cValue2->create("Slide Value", "", "", 120, 340, 200, 50);
	// cValue2->set_scrollable(true, false);

	face_settings.add_control(cToggle);
	face_settings.add_control(cToggle2);
	face_settings.add_control(cToggle3);
	face_settings.add_control(cLabel1);
	face_settings.add_control(cToggle4);
	face_settings.add_control(cToggle5);
	face_settings.add_control(cToggle6);
	// face_settings.add_control(cValue1);
	// face_settings.add_control(cValue2);
}

void Display::update_boot_face(wifi_states status)
{
	setCpuFrequencyMhz(240);
	current_face = &face_boot;
	face_boot.wifi_connect_status(status);
}

/**
 * @brief Tell the current face being shown to draw itself. Respects the update rate of the face by passing in false in the draw(false) method on the face.
 *
 */
void Display::update_current_face() { current_face->draw(false); }

void Display::show_low_battery()
{
	current_face = &face_batteryempty;
	update_current_face();
}

/**
 * @brief Process the touch input from the user and coordinates clicks, dbl clicks, long presses, dragging, scrolling, widgets and controls. It's quite complex.
 *
 */
void Display::process_touch()
{
	Directions _dir = NONE;
	Directions swipe_dir = NONE;

	uint8_t move_margin_for_drag = 5;

	if (touchpad.available(settings.config.flipped))
	{
		tinywatch.set_cpu_frequency(current_face->get_cpu_speed(), CPU_CHANGE_HIGH);

		if (!isTouched && touchpad.finger_num == 1)
		{
			startX = touchpad.x;
			startY = touchpad.y;
			deltaX = 0;
			deltaY = 0;
			moved_x = touchpad.x;
			moved_y = touchpad.y;
			isTouched = true;
			prevent_long_press = false;
			touchTime = millis();

			last_touch = millis();
			drag_rate = millis();
			last_finger_move = millis();

			current_face->drag_begin(startX, startY);

			backlight_level = 0;
			set_backlight(backlight_level, false);
		}
		else if (isTouched && touchpad.finger_num == 1)
		{
			if (last_was_long)
				return;
			deltaX = touchpad.x - startX;
			deltaY = touchpad.y - startY;

			int16_t moved_much_x = touchpad.x - moved_x;
			int16_t moved_much_y = touchpad.y - moved_y;

			moved_x = touchpad.x;
			moved_y = touchpad.y;

			last_touch = millis();

			if ((abs(deltaX) > move_margin_for_drag || abs(deltaY) > move_margin_for_drag))
			{
				current_face->drag(deltaX, deltaY, moved_much_x, moved_much_y, touchpad.x, touchpad.y, true);
				prevent_long_press = true;
			}
			else if (!prevent_long_press && last_touch - touchTime > 600)
			{
				// might be a long click?
				touch_event_t touch_data = touch_event_t(touchpad.x, touchpad.y, TOUCH_LONG);
				if (current_face->process_touch(touch_data))
				{
					BuzzerUI({{2000, 400}});

					last_was_click = false;
					last_was_long = true;
				}
			}
		}
		else if (isTouched && touchpad.finger_num == 0)
		{
			isTouched = false;

			if (last_was_long)
			{
				last_was_long = false;
				return;
			}

			last_touch = millis();

			deltaX = touchpad.x - startX;
			deltaY = touchpad.y - startY;

			uint16_t deltaX_abs = abs(deltaX);
			uint16_t deltaY_abs = abs(deltaY);

			// info_printf("drag: %d, deltaX: %d, deltaY: %d, time: %d\n", current_face->drag_dir, deltaX, deltaY, (last_touch - touchTime));

			// If the current face is blocked from dragging, it's likely because of an app, so if teh distance from first touch to last is enough to suggest a swipe, pass a swipe to the current face for the app to get
			if (current_face->is_drag_blocked() && (deltaX_abs > 25 || deltaY_abs > 25))
			{
				// info_println("Sending swipe");
				// Calculate swipe dir to pass on
				int8_t dir = -1;
				if (deltaY_abs > deltaX_abs)
					dir = (deltaY < 0) ? 0 : 2;
				else
					dir = (deltaX > 0) ? 1 : 3;

				// 	if (current_face->swipe(touchpad.x, touchpad.y, dir, deltaX, deltaY))

				touch_event_t touch_data = touch_event_t(touchpad.x, touchpad.y, TOUCH_SWIPE, (TouchSwipeDirection)dir, deltaX, deltaY);
				if (current_face->process_touch(touch_data))
				{
					return;
				}
			}

			// If no drag direction was selected and the delta from first touch to last is small enough to suggest a click, process a click or long click
			if (current_face->drag_dir == -1 && (abs(deltaX) < 5 && abs(deltaY) < 5))
			{
				dbl_touch[0] = dbl_touch[1];
				dbl_touch[1] = millis();

				bool double_click = (dbl_touch[1] - dbl_touch[0] < 200);
				if (double_click)
				{
					// block 2 dbl clicks in a row from 3 clicks
					dbl_touch[1] = 0;

					last_was_click = false;
					// if (current_face->click_double(touchpad.x, touchpad.y))

					touch_event_t touch_data = touch_event_t(touchpad.x, touchpad.y, TOUCH_DOUBLE);
					if (current_face->process_touch(touch_data))
					{
						BuzzerUI({
							{2000, 40},
							{0, 15},
							{2000, 40},
						});
						return;
					}
				}
				else
				{
					last_was_click = true;
				}
			}
			// If we have selected a drag direction, make sure the delta is long enough to activate the drag
			else if (current_face->drag_dir > -1 && (abs(deltaX) > 5 || abs(deltaY) > 5))
			{
				int distance = sqrt(pow((touchpad.x - startX), 2) + pow((touchpad.y - startY), 2));
				touchTime = millis() - touchTime;

				int16_t last_dir_x = touchpad.x - moved_x;
				int16_t last_dir_y = touchpad.y - moved_y;

				if (current_face->drag_end(deltaX, deltaY, true, distance, touchpad.x, touchpad.y, last_dir_x, last_dir_y))
				{
					// switch face to the new one and make it the current face
					int dir = current_face->drag_dir;
					if (current_face->navigation[dir] != nullptr)
					{
						current_face = current_face->navigation[dir];
						current_face->reset_cache_status();
						current_face->draw(true);
					}
					return;
				}
			}
		}
	}
	else
	{
		tinywatch.set_cpu_frequency(current_face->get_cpu_speed(), CPU_CHANGE_LOW);

		if (current_face->is_face_cached())
			current_face->reset_cache_status();

		// if (backlight_level > 0)
		// {
		// 	imu.update();
		// 	if (imu.is_looking_at_face())
		// 	{
		// 		backlight_level = 0;
		// 		set_backlight(backlight_level);
		// 		last_touch = millis();
		// 		info_println("IMU backlight level: " + String(backlight_level));
		// 	}
		// }
	}

	// If there was a pervious click, and the time past has been longer than what a double click would trigger, process the original single click
	if (millis() - last_touch > 150 && last_was_click)
	{
		last_was_click = false;

		touch_event_t touch_data = touch_event_t(touchpad.x, touchpad.y, TOUCH_TAP);

		// A click should only happen if the finger didn't drag - much
		if (current_face->widget_process_clicks(touch_data))
		{
			BuzzerUI({{2000, 10}});
		}
		else if (current_face->control_process_clicks(touch_data))
		{
			BuzzerUI({{2000, 10}});
		}
		// else if (current_face->click(touchpad.x, touchpad.y))
		else if (current_face->process_touch(touch_data))
		{
			BuzzerUI({{2000, 20}});
		}
		last_was_click = false;
	}

	// Process the backlight timer
	if (millis() - last_touch > get_backlight_period())
	{
		last_touch = millis();
		if (backlight_level < 3)
		{
			backlight_level++;
			set_backlight(backlight_level, false);
			info_println("Setting backlight level: " + String(backlight_level));
		}
		else if (!tinywatch.vbus_present())
		{
			tinywatch.go_to_sleep();
		}
	}
}

uint Display::get_backlight_period()
{
	// Is 5V present even though we think we are on battery power?
	return tinywatch.vbus_present() ? settings.config.bl_period_vbus : settings.config.bl_period_vbat;
}

void Display::set_backlight(int level, bool force)
{
	if (last_backlight != level || force)
	{
		last_backlight = level;
		backlight_target_val = tinywatch.vbus_present() ? backlight_settings_vbus[level] : backlight_settings_vbat[level];
	}
}

/**
 * @brief Set the backlight to be a specific PWM value
 *
 * @param val PWM duty value between 0 and 255, constrained.
 */
void Display::set_backlight_val_direct(uint8_t val)
{
	backlight_target_val = constrain(val, 0, 255);
	// info_printf("backlight cur: %d, target: %d\n", backlight_current_val, backlight_target_val);
}

/**
 * @brief Is called from a separate task and fades the backlight value up and down based on a target value
 *
 * @return true
 * @return false
 */
bool Display::adjust_backlight()
{
	if (backlight_current_val < backlight_target_val - 1)
	{
		float delta = constrain((backlight_target_val - backlight_current_val) / 50, 1, backlight_target_val);
		backlight_current_val = constrain(backlight_current_val + (int)delta, 0, backlight_target_val);
		// info_printf("> backlight cur: %d, target: %d, delta: %f\n", backlight_current_val, backlight_target_val, delta);
		return true;
	}
	else if (backlight_current_val > backlight_target_val + 1)
	{
		float delta = constrain((backlight_current_val - backlight_target_val) / 50, 1, backlight_current_val);
		backlight_current_val = constrain(backlight_current_val - (int)delta, backlight_target_val, 255);
		// info_printf("< backlight cur: %d, target: %d, delta: %f\n", backlight_current_val, backlight_target_val, delta);
		return true;
	}
	else if (backlight_current_val != backlight_target_val)
	{
		backlight_current_val = backlight_target_val;
		return true;
	}
	return false;
}

uint8_t Display::get_current_backlight_val() { return backlight_current_val; }

display_states Display::get_current_display_state() { return current_display_state; }

// TODO: Convert backlight values from 0-255 to 0-100%
/**
 * @brief FreeRTOS Task for controlling the screen backlight to allow for fading and non-blocking adjustment
 *
 * @param param
 */
static void process_backlight(void *param)
{
	info_print("Starting backlight control and loading icon system on core ");
	info_println(xPortGetCoreID());

	while (true)
	{
		bool update_pwm = display.adjust_backlight();
		bool doing_something = false;

		if (update_pwm)
		{
			doing_something = true;
			analogWrite(TFT_LED, display.get_current_backlight_val());
			vTaskDelay(1);
		}

		if (display.get_current_display_state() == LOADING)
		{
			doing_something = true;
			display.show_loading_icon();
			vTaskDelay(100);
		}

		if (!doing_something)
			vTaskDelay(10);
	}
}

/**
 * @brief Old Fill Arc code from TFT_eSPI creator to draw non smoothed arcs. TFT_eSPI now has smooth arc drawing routines built in, but the microphone face still uses this method as it's faster.
 *
 * @param canvasid
 * @param x
 * @param y
 * @param start_angle
 * @param seg_count
 * @param rx
 * @param ry
 * @param w
 * @param colour
 */
void Display::fill_arc(uint8_t canvasid, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

	byte seg = 6; // Segments are 3 degrees wide = 120 segments for 360 degrees
	byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

	// Calculate first pair of coordinates for segment start
	float sx = cos((start_angle - 90) * DEG2RAD);
	float sy = sin((start_angle - 90) * DEG2RAD);
	uint16_t x0 = sx * (rx - w) + x;
	uint16_t y0 = sy * (ry - w) + y;
	uint16_t x1 = sx * rx + x;
	uint16_t y1 = sy * ry + y;

	// Draw colour blocks every inc degrees
	for (int i = start_angle; i < start_angle + seg * seg_count; i += inc)
	{

		// Calculate pair of coordinates for segment end
		float sx2 = cos((i + seg - 90) * DEG2RAD);
		float sy2 = sin((i + seg - 90) * DEG2RAD);
		int x2 = sx2 * (rx - w) + x;
		int y2 = sy2 * (ry - w) + y;
		int x3 = sx2 * rx + x;
		int y3 = sy2 * ry + y;

		canvas[canvasid].fillTriangle(x0, y0, x1, y1, x2, y2, colour);
		canvas[canvasid].fillTriangle(x1, y1, x2, y2, x3, y3, colour);

		// Copy segment end to segment start for next segment
		x0 = x2;
		y0 = y2;
		x1 = x3;
		y1 = y3;
	}
}
