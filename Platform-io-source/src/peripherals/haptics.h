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
	WAKE,
	LONGPRESS,
	CHARGE,
};

class Haptics
{
	public:
		bool init();
		void go_standby();
		void play(const std::vector<uint8_t> &sequence);
		void play_trigger(Triggers trigger);

		bool available = false;

	private:
		Adafruit_DRV2605 drv;
		std::array<std::vector<uint8_t>, 7> sounds;
};

extern Haptics haptics;
