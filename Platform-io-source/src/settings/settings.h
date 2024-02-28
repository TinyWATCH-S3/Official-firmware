#pragma once

#include "json.h"
#include "json_conversions.h"
#include "settings/settingsOption.h"
#include "settings_extra.h"
#include "settings_apps.h"
#include "settings_widgets.h"
#include "utilities/logging.h"
#include <Preferences.h>
#include <vector>

using json = nlohmann::json;

struct wifi_station
{
		String wifi_ssid = "";
		String wifi_pass = "";
		uint8_t channel = 9;
};

// Save data struct
struct Config
{
		int ver = 1;
		bool first_time = true;

		bool wifi_start = false;
		String wifi_ssid = "";
		String wifi_pass = "";

		std::vector<wifi_station> wifi_options;

		String mdns_name = "tinywatch";
		bool website_darkmode = true;

		// UTC and Country settings - country needed for some widgets like Open Weather
		String country = "";
		String city = "";
		int16_t utc_offset = 999;

		// Display
		uint bl_period_vbus = 60000;
		uint bl_period_vbat = 30000;

		// Time
		bool time_24hour = false;
		bool time_dateformat = false; // False - DMY, True - MDY
		uint8_t clock_face_index = 0;

		// Watch
		bool left_handed = true;
		bool flipped = false;

		// Sound
		bool audio_ui = true;
		bool audio_alarm = true;

		// IMU
		bool imu_process_steps = true;
		bool imu_process_wrist = false;

		json last_saved_data;

		/*
		 * Additional settings stucts go here
		 */

		// MQTT specific settings - see Struct above
		Config_mqtt mqtt;

		// Binary Clock Face specific settings
		Config_custom_binary custom_binary;

		/*
		 * Widget Settings Structs
		 */

		// Battery/FG specific settings - see Struct above
		Config_widget_battery battery;

		// Open Weather specific settings - see Struct above
		Config_widget_open_weather open_weather;

		/*
		 * App Settings Structs
		 */

		// Microphone App specific Settings
		Config_app_microphone app_microphone;

		// Compass specific settings
		Config_app_compass compass;
};

class Settings
{

	public:
		Config config;
		bool load();
		bool save(bool force);
		bool backup();
		bool create();
		void print_file(void);
		bool has_wifi_creds(void);
		bool has_country_set(void);
		void update_wifi_credentials(String ssid, String pass);
		String get_load_status();
		String get_save_status();

		// Add any SettingsOption values here for any settings you want to bind with a tw_Control
		SettingsOptionBool setting_wifi_start{&config.wifi_start};
		SettingsOptionBool setting_time_24hour{&config.time_24hour};
		SettingsOptionBool setting_time_dateformat{&config.time_dateformat};
		SettingsOptionBool setting_left_handed{&config.left_handed};
		SettingsOptionBool setting_flipped{&config.flipped};
		SettingsOptionBool setting_audio_ui{&config.audio_ui};
		SettingsOptionBool setting_audio_alarm{&config.audio_alarm};

	private:
		static constexpr const char *filename = "/settings.json";
		static constexpr const char *tmp_filename = "/tmp_settings.json";
		static constexpr const char *log = "/log.txt";
		static constexpr const char *backup_prefix = "settings_back_";
		static const int max_backups = 10;
		static long backupNumber(const String);

		unsigned long max_time_between_saves = 60000; // every 1 min
		unsigned long last_save_time = 0;			  //

		void log_to_nvs(const char *key, const char *log);
		Preferences nvs;
};

extern Settings settings;
