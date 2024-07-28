#include "themes.h"
#include "tinywatch.h"
#include <LittleFS.h>

using json = nlohmann::json;

/**
 * @brief Construct a new nlohmann define type non intrusive with default object
 *
 * All settings you want to be serialised and deserialised with JSON and stored in user flash need to be added here.
 * This has a HARD (NOT CHANGEABLE) LIMIT of 64 items
 */

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(theme, name, col_background_dull, col_background_bright, col_control_back, col_primary, col_secondary, col_low_intensity, col_warning, col_error, widget_style);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(WatchThemes, themes);

theme &Themes::current()
{
	if (watch_themes.themes.size() == 0)
		return default_theme;

	return default_theme;
}

/**
 * @brief Load the theme settings from the FLASH FS and deserialise them from JSON back into the Config struct
 *
 * @return true
 * @return false
 */
bool Themes::load()
{
	info_println("Loading themes");

	File file = LittleFS.open(filename);
	if (!file || file.isDirectory() || file.size() == 0)
	{
		// No data on the flash chip, so create new data
		tinywatch.log_system_message("No theme data on flash");
		file.close();

		info_println("Themes CREATE: Creating new data...");

		watch_themes = {};

		tinywatch.log_system_message("created new theme file");

		save();

		return true;
	}

	std::vector<char> _data(file.size());
	size_t data_bytes_read = file.readBytes(_data.data(), _data.size());
	if (data_bytes_read != _data.size())
	{
		// Reading failed
		tinywatch.log_system_message("Reading data failed");
		String log = "bad read " + String(file.size()) + " " + String((int)data_bytes_read);
		// log_to_nvs("load_status", log.c_str());
		file.close();
		// create();
		return false;
	}

	try
	{
		json json_data = json::parse(_data);

		// Convert json to struct
		watch_themes = json_data.get<WatchThemes>();
	}
	catch (json::exception &e)
	{
		info_println("Settings parse error:");
		info_println(e.what());
		tinywatch.log_system_message("JSON parse error on read");
		file.close();
		// create();
		// log_to_nvs("load_status", "bad json parse");
		return false;
	}

	file.close();

	tinywatch.log_system_message("Loaded themes");

	return true;
}

bool Themes::save()
{
	// // We only want to attempt  save every 1 min unless it's a forced save.
	// if (!force && millis() - last_save_time < max_time_between_saves)
	// 	return false;

	// Implicitly convert struct to json
	json data = watch_themes;

	// // If the data is the same as the last data we saved, bail out
	if (data == watch_themes.last_saved_data)
	{
		return false;
	}

	std::string serializedObject = data.dump();

	// info_print("Data Length: "+String(serializedObject.length())+"-> ");
	// info_println(serializedObject);

	File file = LittleFS.open(tmp_filename, FILE_WRITE);
	if (!file)
	{
		error_println("Failed to write to themes file");
		tinywatch.log_system_message("Write themes failed");
		// log_to_nvs("save_status", "failed to open for write");
		return false;
	}

	file.print(serializedObject.c_str());
	// log_to_nvs("save_status", "data written");

	file.close();
	// log_to_nvs("save_status", "file closed");

	LittleFS.rename(tmp_filename, filename);
	// log_to_nvs("save_status", "file renamed");

	info_println("Themes SAVE: Saved!");

	tinywatch.log_system_message("Themes saved");

	// Store last saved data for comparison on next save
	watch_themes.last_saved_data.swap(data);

	// last_save_time = millis();
	return true;
}

Themes themes;