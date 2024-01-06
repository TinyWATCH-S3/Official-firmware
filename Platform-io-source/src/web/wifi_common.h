#pragma once

#include "peripherals/buzzer.h"
#include <Arduino.h>
#include <WiFi.h>

enum wifi_states
{
	BOOT = 0,
	WIFI_SETUP = 1,
	WIFI_SETUP_STEP_2 = 2,
	WIFI_CONNECTING = 3,
	WIFI_CONNECTED = 4,
	WIFI_DISCONNECTED = 5,
	WIFI_GET_TIME = 10,
	WIFI_DONE = 11,
	WIFI_ERROR = 20,
	WIFI_ERROR_NOSSID = 21,
	WIFI_RESET = 30,
};
