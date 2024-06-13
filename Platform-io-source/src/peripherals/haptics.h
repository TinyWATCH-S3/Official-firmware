#pragma once

#include <Adafruit_DRV2605.h>
#include <array>
#include <vector>

enum Triggers
{
	STARTUP,
	ALARM,
	HOUR,
	EVENT,
	WAKE
};

class Haptics
{
	public:
		bool init();
		void play(Triggers trigger);

		bool available = false;

	private:
		Adafruit_DRV2605 drv;
		std::array<std::vector<uint8_t>, 5> sounds;
};

extern Haptics haptics;
