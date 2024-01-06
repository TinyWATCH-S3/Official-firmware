#pragma once

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

class Battery
{
	public:
		bool init();
		float get_voltage(bool forced);
		float get_percent(bool forced);
		float get_rate(bool forced);
		void set_hibernate(bool state);
		uint8_t get_alert_status();
		void clear_alert_status();
		void update_interrupt_values(uint8_t perc, float low_bat);
		bool is_low(bool using_perc);
		bool is_high();

	private:
		SFE_MAX1704X maxlipo;
		unsigned long next_battery_read = 0;
		float cached_voltage = 0;
		float cached_percent = 0;
		float cached_rate = 0;

		void update_stats(bool forced);
};

extern Battery battery;
