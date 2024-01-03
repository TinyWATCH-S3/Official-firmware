#pragma once

#include "json.h"
#include "json_conversions.h"
#include "utilities/logging.h"
#include "settings/settingsOption.h"
#include <Preferences.h>

using json = nlohmann::json;

struct Config_mqtt
{
	//MQTT Stuff
    bool enabled = false;
    String broker_ip = "";
    int broker_port = 1883;
    String username = "";
    String password = "";
    String device_name = "TinyWATCH";
	String topic = "tinywatch";
};

struct Config_battery
{
	// Fuel Gauge/Battery
	// This can be used to adjust what 100% is on your battery
	// This can be needed because the PMIC will stop charging the battery before it gets to 4.2V, usually around 4.1V,
	// So the fuel gauge will never actually get to a 100% state. 
	// I'm not sure how to solve this othe than allow users to set an offset that visually shows full/no charging at 100%
	float perc_offset = 7.0;
	// User settable % to trigger fuel gauge wake from sleep trigger.
	// This can only be between 1% and 32% 
	uint8_t low_perc = 25;
	// User settable V to trigger fuel gauge wake from sleep trigger.
	float low_volt_warn = 3.5;
	// User settable V to trigger power cutoff to the watch power switch.
	float low_volt_cutoff = 3.2;
};

struct Config_open_weather
{
	// Widget specific settings for Open Weather
	uint32_t poll_frequency = 180000; // Open Weather poll interval - 30mins. 
	String api_key = ""; // API key for Open Weather
};

struct Config_custom_binary
{
	// Custom Watch Face - Binary Clock
	uint8_t binary_clockcolour = 0;
	uint8_t binary_clockstyle = 0;
};

// Save data struct
struct Config
{
	int ver = 1;
	bool first_time = true;

	bool wifi_start = false;
	String wifi_ssid = "";
	String wifi_pass = "";
    String mdns_name = "tinywatch";
    bool website_darkmode = true;

	// UTC and Country settings - country needed for some widgets like Open Weather
	String country = "";
	String city = "";
	int16_t utc_offset = 999;

	// MQTT specific settings - see Struct above
	Config_mqtt mqtt;

	// Battery/FG specific settings - see Struct above
	Config_battery battery;

	// Open Weather specific settings - see Struct above
	Config_open_weather open_weather;

	// Binary Clock Face specific settings
	Config_custom_binary custom_binary;

	// Display
	uint bl_period_vbus = 60000;
	uint bl_period_vbat = 30000;

	// Time
	bool time_24hour = false;
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
	SettingsOptionBool setting_left_handed{&config.left_handed};
	SettingsOptionBool setting_flipped{&config.flipped};
	SettingsOptionBool setting_audio_ui{&config.audio_ui};
	SettingsOptionBool setting_audio_alarm{&config.audio_alarm};

private:
	static constexpr const char *filename = "/settings.json";
	static constexpr const char *log = "/log.txt";
	static constexpr const char *backup_prefix = "settings_back_";
	static const int max_backups = 10;
	static long backupNumber(const String);

	unsigned long max_time_between_saves = 60000; // every 1 min
	unsigned long last_save_time = 0; // 

	void log_to_nvs(const char * key, const char * log);
	Preferences nvs;
};

extern Settings settings;
