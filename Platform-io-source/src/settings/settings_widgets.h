#pragma once
#include "Arduino.h"

/**
 * @brief Widget settings structs go in here and are then added to the main struct
 * in settings.h
 */

/**
 * @brief Settings struct for battery configuration
 */
struct Config_widget_battery
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

/**
 * @brief Settings struct for open weather widget
 */
struct Config_widget_open_weather
{
		uint32_t poll_frequency = 180000; // Open Weather poll interval - 30mins.
		String api_key = "";			  // API key for Open Weather
		bool enabled = false;
};
