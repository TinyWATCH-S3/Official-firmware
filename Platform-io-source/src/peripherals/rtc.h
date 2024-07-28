#pragma once

#include "RV3028C7.h"
#include "Arduino.h"
#include "json.h"
#include "json_conversions.h"
#include "utilities/logging.h"
#include <vector>

using json = nlohmann::json;

struct TT_Time
{
	public:
		uint8_t secs = 0;
		uint8_t mins = 0;
		uint8_t hours = 0;
};

/*
		bool setDateAlarm(AlarmMode_t mode, uint8_t dayOfMonth, uint8_t hour = 0, uint8_t minute = 0);
		bool setWeekdayAlarm(AlarmMode_t mode, DayOfWeek_t dayOfWeek, uint8_t hour = 0, uint8_t minute = 0);
		bool setDailyAlarm(uint8_t hour = 0, uint8_t minute = 0);
		bool setHourlyAlarm(uint8_t minute = 0);
*/

enum AlarmType
{
	A_DISABLED = 0,
	A_MONTHLY = 1,
	A_WEEKLY = 2,
	A_DAILY = 3,
	A_HOURLY = 4
};

struct Alarm
{
	public:
		AlarmType type = AlarmType::A_DISABLED;
		int8_t dayOfMonth = -1;
		int8_t dayOfWeek = -1;
		int8_t hour = -1;
		int8_t minute = -1;
		bool enabled = false;

		void add_alarm_monthly(AlarmType _mode, uint8_t _dayOfMonth, uint8_t _hour = 0, uint8_t _minute = 0)
		{
			type = _mode;
			dayOfMonth = _dayOfMonth;
			hour = _hour;
			minute = _minute;
			enabled = true;
		}

		void add_alarm_weekly(AlarmType _mode, DayOfWeek_t dayOfWeek, uint8_t _hour = 0, uint8_t _minute = 0)
		{
			type = _mode;
			dayOfWeek = dayOfWeek;
			hour = _hour;
			minute = _minute;
			enabled = true;
		}

		void add_alarm_daily(AlarmType _mode, uint8_t _hour = 0, uint8_t _minute = 0)
		{
			type = _mode;
			hour = _hour;
			minute = _minute;
			enabled = true;
		}

		void add_alarm_hourly(AlarmType _mode, uint8_t _minute = 0)
		{
			type = _mode;
			minute = _minute;
			enabled = true;
		}

		void enable_alarm(bool state)
		{
			enabled = state;
		}
};

struct Alarms
{
		std::vector<Alarm> alarms;
		json last_saved_data;
};

class RTC
{
	public:
		bool init();
		void setup_interrupt();
		bool check_interrupt();
		bool set_time_from_NTP(int16_t utc_offset);
		void set_hourly_alarm(uint minutes);
		// String getTime();
		String get_hours_string(bool padded, bool is24hour);
		String get_mins_string(bool padded);
		String get_secs_string(bool padded);
		String get_day_date();
		String get_day_of_week();
		String get_month_date();
		String get_time_string(bool padded, bool is24hour);
		int get_hours();
		int get_mins();
		int get_seconds();

		uint16_t get_day();
		uint16_t get_month();
		uint16_t get_year();

		void get_step_date(uint16_t &day, uint16_t &month, uint16_t &year);

		TT_Time time_components;
		RV3028C7 rtc;

		bool requiresNTP = false;

		void process_alarms();

		// Alarms
		void create_alarm(AlarmType _mode, uint8_t _dayOfMonth, uint8_t _hour, uint8_t _minute);
		void create_alarm(AlarmType _mode, uint8_t _hour, uint8_t _minute);
		void create_alarm(AlarmType _mode, uint8_t _minute);

		// Savedata
		bool load();
		bool save();
		void print_file();

	private:
		bool enabled = false;

		uint8_t interruptPin = 4;
		unsigned long next_rtc_read = 0;
		unsigned long next_int_check = 0;
		uint16_t cached_day = 0;
		uint16_t cached_month = 0;
		uint16_t cached_year = 0;

		Alarms current_alarms;

		static constexpr const char *filename = "/alarms.json";
		static constexpr const char *tmp_filename = "/tmp_alarms.json";
};

extern RTC rtc;
