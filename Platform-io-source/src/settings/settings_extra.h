#pragma once
#include "Arduino.h"

/**
 * @brief Extra settings structs go in here and are then added to the main struct
 * in settings.h
 */

/**
 * @brief Settings struct for MQTT
 */
struct Config_mqtt
{
		bool enabled = false;
		String broker_ip = "";
		int broker_port = 1883;
		String username = "";
		String password = "";
		String device_name = "TinyWATCH";
		String topic = "tinywatch";
};

/**
 * @brief Settings struct for battery configuration
 */
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

/**
 * @brief Settings struct for open weather widget
 */
struct Config_open_weather
{
		uint32_t poll_frequency = 180000; // Open Weather poll interval - 30mins.
		String api_key = "";			  // API key for Open Weather
};

/**
 * @brief Settings struct for Binary clock face
 * Added by https://github.com/optoisolated
 */
struct Config_custom_binary
{
		uint8_t binary_clockcolour = 0;
		uint8_t binary_clockstyle = 0;
};

struct Config_app_microphone
{
		uint8_t sweep_size = 1;
		uint8_t gain_factor = 0;
};

/**
 * @brief Settings required by the compass
 */
struct Config_compass
{
		// {17.77, -30.08, 16.21};
		float hard_iron_x = 0;
		float hard_iron_y = 0;
		float hard_iron_z = 0;
		float soft_iron_x = 1;
		float soft_iron_y = 1;
		float soft_iron_z = 1;
		float magnetic_declination = 0.0; //-1.233;
};