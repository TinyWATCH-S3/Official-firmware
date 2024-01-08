#pragma once

#include "json.h"
#include "json_conversions.h"
#include "utilities/logging.h"
#include <map>

using json = nlohmann::json;

// Activity data struct
struct Activity_Data
{
	public:
		std::map<uint32_t, std::array<uint32_t, 31>> steps_day;
		std::map<uint32_t, uint32_t> steps_year;

		uint8_t imu_day = 0;
		uint8_t imu_month = 0;
		uint8_t imu_year = 0;

		json last_saved_data;
};

class Activity
{
	public:
		Activity_Data data;

		bool load();
		bool save(bool force);
		bool create();

		void track_steps(uint32_t steps, uint8_t day, uint8_t month, uint16_t year);

		bool loaded = false;

	protected:
		//

	private:
		static constexpr const char *filename = "/activity.json";
		static constexpr const char *tmp_filename = "/tmp_activity.json";

		unsigned long max_time_between_saves = 300000; // every 5 mins
		unsigned long last_save_time = 0;			   //
};

extern Activity activity;
