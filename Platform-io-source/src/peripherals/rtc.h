#pragma once

#include "RV3028C7.h"

struct TT_Time
{
	public:
		uint8_t secs = 0;
		uint8_t mins = 0;
		uint8_t hours = 0;
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

	private:
		bool enabled = false;

		uint8_t interruptPin = 33;
		unsigned long next_rtc_read = 0;
		uint16_t cached_day = 0;
		uint16_t cached_month = 0;
		uint16_t cached_year = 0;
};

extern RTC rtc;
;
