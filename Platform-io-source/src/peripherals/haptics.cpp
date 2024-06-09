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

	drv.setRealtimeValue(0x40);
	delay(200);
	drv.setRealtimeValue(0x00);

	return true;
}

void Haptics::play()
{
	if (!available)
		return;

	uint8_t rtp_index = 0;
	uint8_t rtp[] = {
		0x30, 100, 0x32, 100,
		0x34, 100, 0x36, 100,
		0x38, 100, 0x3A, 100,
		0x00, 100,
		0x40, 200, 0x00, 100,
		0x40, 200, 0x00, 100,
		0x40, 200, 0x00, 100
	};

	while (rtp_index < sizeof(rtp) / sizeof(rtp[0]))
	{
		drv.setRealtimeValue(rtp[rtp_index]);
		rtp_index++;
		delay(rtp[rtp_index]);
		rtp_index++;
	}

	drv.setRealtimeValue(0x00);
	delay(1000);
	rtp_index = 0;
}

Haptics haptics;
