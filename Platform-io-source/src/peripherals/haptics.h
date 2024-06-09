#pragma once

#include <Adafruit_DRV2605.h>

class Haptics
{
	public:
		bool init();
		void play();

		bool available = false;

	private:
		Adafruit_DRV2605 drv;
		unsigned long next_battery_read = 0;
		float cached_voltage = 0;
		float cached_percent = 0;
		float cached_rate = 0;
};

extern Haptics haptics;
