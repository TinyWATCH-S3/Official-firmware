#pragma once
#include "Arduino.h"

/**
 * @brief App settings structs go in here and are then added to the main struct
 * in settings.h
 */

struct Config_app_microphone
{
		uint8_t sweep_size = 1;
		uint8_t gain_factor = 0;
};

/**
 * @brief Settings required by the compass
 */
struct Config_app_compass
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