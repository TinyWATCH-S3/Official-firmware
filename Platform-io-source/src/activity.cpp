#include "activity.h"
#include "utilities/logging.h"
#include "utilities/user_flash.h"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Activity_Data, steps_day, steps_year, imu_day, imu_month, imu_year);


bool Activity::load()
{
	info_println("Loading activity");

	File file = flashFS.open(filename);
	if (!file || file.isDirectory() || file.size() == 0)
	{
		// No data on the flash chip, so create new data
		file.close();
		create();
		return false;
	}

	std::vector<char> _data(file.size());
	if (file.readBytes(_data.data(), _data.size()) != _data.size())
	{
		// Reading failed
		file.close();
		create();
		return false;
	}

	try
	{
		json json_data = json::parse(_data);

		// info_println("Activity_data");
		// for (int i = 0; i < _data.size(); i++)
		// 	info_print((char)_data[i]);
		// info_println("");

		// Convert json to struct
		data = json_data.get<Activity_Data>();

		// Store loaded data for comparison on next save
		data.last_saved_data.swap(json_data);
	}
	catch (json::exception &e)
	{
		info_println("Activity parse error:");
		info_println(e.what());
		file.close();
		create();
		return false;
	}

	file.close();
	loaded = true;
	return true;
}

bool Activity::save(bool force)
{
	// We only want to attempt  save every 5 mins unless it's a forced save.
	if (!force && millis() - last_save_time < max_time_between_saves)
		return false;

	// Implicitly convert struct to json
	json json_data = data;

	// If the data is the same as the last data we saved, bail out
	if (json_data == data.last_saved_data)
	{
		last_save_time = millis();
		return false;
	}

	// info_println(F("Activity SAVE: Saving data..."));

	std::string serializedObject = json_data.dump();

	File file = flashFS.open(filename, FILE_WRITE);
	if (!file)
	{
		error_println("Failed to write to activity file");
		return false;
	}

	file.print(serializedObject.c_str());
	file.close();
	info_println(F("Activity SAVE: Saved!"));

	// Store last saved data for comparison on next save
	data.last_saved_data.swap(json_data);

	last_save_time = millis();
	return true;
}

bool Activity::create()
{
	info_println("Activity CREATE: Creating new data...");

	data = {};

	save(true);
	loaded = true;

	return true;
}


void Activity::track_steps(uint32_t steps, uint8_t day, uint8_t month, uint16_t year)
{
	uint32_t key = (year * 100) + month;
	info_println("Year: "+String(year) +", month: "+String(month) +", day: "+String(day));
	info_println("Key: "+String(key));
	data.steps_day[key][day-1] += steps;  // day is 1-31, but array is 0-30
	data.steps_year[year] += steps;

	// data.steps_day += steps;
	// data.steps_month += steps;
	// data.steps_year += steps;
}

Activity activity;
