#pragma once

#include <Adafruit_DRV2605.h>
#include <array>
#include <vector>

enum Triggers
{
	STARTUP,
	ALARM,
	HOUR,
	EVENT
};

class Haptics
{
	public:
		bool init();
		void play(Triggers trigger);

		bool available = false;

	private:
		Adafruit_DRV2605 drv;
		std::array<std::vector<uint8_t>, 4> sounds;
};

extern Haptics haptics;
