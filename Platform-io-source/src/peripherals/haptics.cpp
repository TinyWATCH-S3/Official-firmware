/**
 * @brief Haptic peripheral for TinyWATCH P7 and later
 *
 * The haptics driver is initialised here, and will fail silently if no haptics IC is found, so the
 * project remains backward compatible
 */

#include "peripherals/haptics.h"
#include "Wire.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "utilities/logging.h"

/**
 * @brief Initialise the haptics IC if found, or fail cleanly for backward compatibility.
 *
 * @return true
 * @return false
 */
bool Haptics::init()
{
	available = false;

	if (!drv.begin())
	{
		Serial.println("Could not find DRV2605");
		available = false;
		return false;
	}

	available = true;
	info_println(F("Starting HAPTICS!"));

	// drv.selectLibrary(1);
	// drv.setMode(DRV2605_MODE_INTTRIG);
	drv.setMode(DRV2605_MODE_REALTIME);

	// Load all of the trigger type sounds into an array of vectors for easy playback.
	sounds[0] = {
		0x30, 100, 0x32, 100,
		0x34, 100, 0x36, 100,
		0x38, 100, 0x3A, 100,
		0x00, 100,
		0x40, 200, 0x00, 100,
		0x40, 200, 0x00, 100,
		0x40, 200, 0x00, 100
	};

	sounds[1] = {
		0x30, 150, 0x00, 255,
		0x30, 150, 0x00, 255,
		0x30, 150
	};

	sounds[2] = {
		0x40, 50
	};

	sounds[3] = {
		0x40, 50, 0x40, 50
	};

	sounds[4] = {
		0x50, 50
	};

	return true;
}

/**
 * @brief Play a custom haptic sound sequence
 *
 * The format of the Vector sequence is {buzzer intensity, delay time, buzzer intensity, delay time...}
 *
 * @param vector array
 */
void Haptics::play(const std::vector<uint8_t> &sequence)
{
	if (!available)
		return;

	if (!settings.config.haptics.enabled)
		return;

	uint8_t rtp_index = 0;

	while (rtp_index < sequence.size())
	{
		drv.setRealtimeValue(sequence[rtp_index++]);
		delay(sequence[rtp_index++]);
	}

	drv.setRealtimeValue(0x00);
	delay(500);
	rtp_index = 0;
}

/**
 * @brief Play a haptic sound sequence based on an ENUM trigger type, if that option it true in haptic settings
 *
 * @param trigger
 */
void Haptics::play_trigger(Triggers trigger)
{
	if (!available)
		return;

	if (!settings.config.haptics.enabled)
		return;

	if (trigger == Triggers::STARTUP && !settings.config.haptics.trigger_on_boot)
		return;

	if (trigger == Triggers::ALARM && !settings.config.haptics.trigger_on_alarm)
		return;

	if (trigger == Triggers::HOUR && !settings.config.haptics.trigger_on_hour)
		return;

	if (trigger == Triggers::EVENT && !settings.config.haptics.trigger_on_event)
		return;

	if (trigger == Triggers::WAKE && !settings.config.haptics.trigger_on_wake)
		return;

	uint8_t rtp_index = 0;

	while (rtp_index < sounds[(int)trigger].size())
	{
		drv.setRealtimeValue(sounds[(int)trigger][rtp_index++]);
		delay(sounds[(int)trigger][rtp_index++]);
	}

	drv.setRealtimeValue(0x00);
	delay(500);
	rtp_index = 0;
}

Haptics haptics;
