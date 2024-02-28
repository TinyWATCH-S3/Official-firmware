#include "settings/settings.h"
#include "tinywatch.h"
#include <LittleFS.h>

using json = nlohmann::json;

/**
 * @brief Construct a new nlohmann define type non intrusive with default object
 *
 * All settings you want to be serialised and deserialised with JSON and stored in user flash need to be added here.
 * This has a HARD (NOT CHANGEABLE) LIMIT of 64 items
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_mqtt, enabled, broker_ip, broker_port, username, password, device_name, topic);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_widget_battery, perc_offset, low_perc, low_volt_warn, low_volt_cutoff);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_widget_open_weather, api_key, poll_frequency);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_custom_binary, binary_clockcolour, binary_clockstyle);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_app_microphone, sweep_size, gain_factor);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config_app_compass, hard_iron_x, hard_iron_y, hard_iron_z, soft_iron_x, soft_iron_y, soft_iron_z, magnetic_declination);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config, wifi_start, wifi_ssid, wifi_pass, mdns_name, website_darkmode, mqtt, battery, open_weather, city, country, utc_offset, bl_period_vbus, bl_period_vbat, time_24hour, time_dateformat, clock_face_index, left_handed, flipped, audio_ui, audio_alarm, imu_process_steps, imu_process_wrist, app_microphone, compass, custom_binary);

void Settings::log_to_nvs(const char *key, const char *log)
{
	nvs.begin("flash_log");
	nvs.putString(key, log);
	nvs.end();
}

/**
 * @brief Checks to see if there are WiFi credentials stores in the user settings
 *
 * @return true credentials are not empty strings
 * @return false credentials are empty strings
 */
bool Settings::has_wifi_creds(void)
{
	// //  info_printf("SSID %s, PASS %s\n", config.wifi_ssid, config.wifi_pass);
	return !config.wifi_ssid.isEmpty() && !config.wifi_pass.isEmpty();
}

bool Settings::has_country_set(void) { return !config.country.isEmpty(); }

/**
 * @brief Update the users WiFi credentials in the settings struct
 *
 * @param ssid
 * @param pass
 */
void Settings::update_wifi_credentials(String ssid, String pass)
{
	config.wifi_ssid = ssid;
	config.wifi_pass = pass;
	save(true);
}

String Settings::get_load_status()
{
	String log = "";
	nvs.begin("flash_log");
	log = nvs.getString("load_status", "load_nada");
	nvs.end();

	return log;
}

String Settings::get_save_status()
{
	String log = "";
	nvs.begin("flash_log");
	log = nvs.getString("save_status", "save_nada");
	nvs.end();

	return log;
}

/**
 * @brief Load the user settings from the user flash FS and deserialise them from JSON back into the Config struct
 *
 * @return true
 * @return false
 */
bool Settings::load()
{
	info_println("Loading settings");

	File file = LittleFS.open(filename);
	if (!file || file.isDirectory() || file.size() == 0)
	{
		// No data on the flash chip, so create new data
		tinywatch.log_system_message("No data on flash");
		file.close();
		create();
		log_to_nvs("load_status", "no file");
		return false;
	}

	std::vector<char> _data(file.size());
	size_t data_bytes_read = file.readBytes(_data.data(), _data.size());
	if (data_bytes_read != _data.size())
	{
		// Reading failed
		tinywatch.log_system_message("Reading data failed");
		String log = "bad read " + String(file.size()) + " " + String((int)data_bytes_read);
		log_to_nvs("load_status", log.c_str());
		file.close();
		create();
		return false;
	}

	try
	{
		json json_data = json::parse(_data);

		// Convert json to struct
		config = json_data.get<Config>();

		// Store loaded data for comparison on next save
		config.last_saved_data.swap(json_data);
	}
	catch (json::exception &e)
	{
		info_println("Settings parse error:");
		info_println(e.what());
		tinywatch.log_system_message("JSON parse error on read");
		file.close();
		create();
		log_to_nvs("load_status", "bad json parse");
		return false;
	}

	file.close();

	return true;
}

// Return the number from a string in the format "settings_back_N.json" for N>0 or 0 if wrong format
long Settings::backupNumber(const String filename)
{
	if (!filename.startsWith(backup_prefix) || !filename.endsWith(".json"))
		return 0;
	// toInt() returns zero if not a number
	return filename.substring(strlen(backup_prefix)).toInt();
}

// TODO: Reimplement settings backup
bool Settings::backup()
{
	// Find the backup file with the highest number
	long highestBackup = 0;
	// File rootDir = SD.open("/");
	// while (true)
	// {
	// 	File file = rootDir.openNextFile();
	// 	if (!file)
	// 		break;
	// 	highestBackup = max(highestBackup, backupNumber(file.name()));
	// 	file.close();
	// }

	// // Delete older files to satisfy max_backups
	// rootDir.rewindDirectory();
	// while (true)
	// {
	// 	File file = rootDir.openNextFile();
	// 	if (!file)
	// 		break;
	// 	long num = backupNumber(file.name());
	// 	if (num > 0 && num <= highestBackup + 1 - max_backups)
	// 	{
	// 		info_printf("Remove old backup %s\n", file.name());
	// 		// Removing an open file might be bad.
	// 		// We close it before removing, making sure to take a copy of the path as will be free'd
	// 		String path = file.path();
	// 		file.close();
	// 		SD.remove(path);
	// 	}
	// 	file.close();
	// }
	// rootDir.close();

	// SD.rename(filename, "/" + String(backup_prefix) + String(highestBackup + 1) + ".json");

	return true;
}

/**
 * @brief Serialise the cConfig struct into JSON and save to the user flash FS
 * Only check for save every 5 mins, and then only save if the data has changed
 *
 * We only want to save data when it's changed because we dont want to wear out the Flash.
 *
 * @param force for the save regardless of time, but again, only if the data has changed
 * @return true
 * @return false
 */
bool Settings::save(bool force)
{
	// We only want to attempt  save every 1 min unless it's a forced save.
	if (!force && millis() - last_save_time < max_time_between_saves)
		return false;

	// Implicitly convert struct to json
	json data = config;

	// If the data is the same as the last data we saved, bail out
	if (data == config.last_saved_data)
	{
		last_save_time = millis();
		return false;
	}

	// Backup the settings file before we save new settings!
	// backup();

	// info_println(F("Settings SAVE: Saving data..."));

	std::string serializedObject = data.dump();

	// info_print("Data Length: "+String(serializedObject.length())+"-> ");
	// info_println(serializedObject);

	File file = LittleFS.open(tmp_filename, FILE_WRITE);
	if (!file)
	{
		error_println("Failed to write to settings file");
		tinywatch.log_system_message("Write settings failed");
		log_to_nvs("save_status", "failed to open for write");
		return false;
	}

	file.print(serializedObject.c_str());
	log_to_nvs("save_status", "data written");

	file.close();
	log_to_nvs("save_status", "file closed");

	LittleFS.rename(tmp_filename, filename);
	log_to_nvs("save_status", "file renamed");

	info_println("Settings SAVE: Saved!");

	tinywatch.log_system_message("Settings saved");

	// Store last saved data for comparison on next save
	config.last_saved_data.swap(data);

	last_save_time = millis();
	return true;
}

/**
 * @brief Create a new set of save data, either because this is the very first save, or because the load failed due to FS corruption,
 * or malformed JSON data that could not be deserialised.
 *
 * Once created, the data is automatically saved to flash.
 *
 * @return true
 * @return false
 */
bool Settings::create()
{
	info_println("Settings CREATE: Creating new data...");

	config = {};

	tinywatch.log_system_message("created new settings");

	save(true);

	return true;
}

void Settings::print_file(void) { info_println("] Done!"); }

Settings settings;
