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
 * @brief Settings struct for Binary clock face
 * Added by https://github.com/optoisolated
 */
struct Config_custom_binary
{
		uint8_t binary_clockcolour = 0;
		uint8_t binary_clockstyle = 0;
};

/**
 * @brief Settings struct for Haptics support
 */
struct Config_haptics
{
		bool enabled = true;
		bool trigger_on_boot = true;
		bool trigger_on_alarm = true;
		bool trigger_on_hour = true;
		bool trigger_on_event = false;
		bool trigger_on_wake = false;
		bool trigger_on_longpress = true;
};
