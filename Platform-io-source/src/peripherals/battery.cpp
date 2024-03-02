#include "peripherals/battery.h"
#include "Wire.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "utilities/logging.h"

#define FG_INT 14

bool Battery::init()
{
	pinMode(FG_INT, INPUT);

	maxlipo.setDevice(MAX1704X_MAX17048);
	maxlipo.enableDebugging();

	if (!maxlipo.begin())
	{
		info_println(F("Couldn't find MAX17048?\nMake sure a battery is plugged in!"));
		return false;
	}
	info_println(F("Starting MAX17048"));

	// Dont use this every boot
	// maxlipo.quickStart();

	// We can set an interrupt to alert when the battery SoC gets too low.
	// We can alert at anywhere between 1% and 32%:
	maxlipo.setThreshold(25); // Set alert threshold to 20%.

	info_print(F("Battery empty threshold is now: "));
	info_print(maxlipo.getThreshold());
	info_println(F("%"));

	// Enable the State Of Change alert
	info_print(F("Enabling the State Of Change alert: "));
	if (maxlipo.enableSOCAlert())
		info_println(F("success."));
	else
		info_println(F("FAILED!"));

	// Set the high voltage threshold
	maxlipo.setVALRTMax((float)4.1); // Set high voltage threshold (Volts) 

	// Read and print the high voltage threshold
	info_print(F("High voltage threshold is now: "));
	float highVoltage = ((float)maxlipo.getVALRTMax()) * 0.02; // 1 LSb is 20mV. Convert to Volts.
	info_print(highVoltage, 2);
	info_println(F("V"));

	// Set the low voltage threshold
	maxlipo.setVALRTMin((float)3.5); // Set low voltage threshold (Volts)

	// Read and print the low voltage threshold
	info_print(F("Low voltage threshold is now: "));
	float lowVoltage = ((float)maxlipo.getVALRTMin()) * 0.02; // 1 LSb is 20mV. Convert to Volts.
	info_print(lowVoltage, 2);
	info_println(F("V"));

	update_stats(true);
	next_battery_read = millis();

	return true;
}

void Battery::update_interrupt_values(uint8_t perc, float low_bat_v)
{
	maxlipo.setThreshold(constrain(perc, 1, 35)); // Set alert threshold from user settings
	info_print(F("Battery empty threshold is now: "));
	info_print(maxlipo.getThreshold());
	info_println(F("%"));

	maxlipo.setVALRTMin(low_bat_v); // Set low voltage threshold (Volts)
	// Read and print the low voltage threshold
	info_print(F("Low voltage threshold is now: "));
	float lowVoltage = ((float)maxlipo.getVALRTMin()) * 0.02; // 1 LSb is 20mV. Convert to Volts.
	info_print(lowVoltage, 2);
	info_println(F("V"));
}

// Returns the interrupt alert status for either low % or low voltage threshold
bool Battery::is_low(bool using_perc)
{
	if (using_perc)
		return maxlipo.isLow();
	else
		return maxlipo.isVoltageLow();
}

// returns if we have tiggered the voltage too high threshold (4.2V) - should not ever happen, but maybe?
bool Battery::is_high() { return maxlipo.isVoltageHigh(); }

void Battery::set_hibernate(bool state)
{
	if (state)
		maxlipo.enableHibernate();
	else
		maxlipo.disableHibernate();
}

void Battery::update_stats(bool forced)
{
	if ((millis() - next_battery_read > 500) || forced) // we only read the IC every 500ms unless forced
	{
		cached_voltage = maxlipo.getVoltage();
		// Why this fudge?
		// Well, the FG internal table it set to see 4.2V as the max voltage, but the PMIC (battery charge IC) won't charge above 4.1-4.13
		// So the FG will technically never reach 100%, so will never show the battery as full.
		// This is all based on experimentation, and I'm not really sure what else to do here - it's just a visual tweak really as
		// the charge/depletion rate still shows a depletion.
		cached_percent = constrain(maxlipo.getSOC() + settings.config.battery.perc_offset, 0.0, 100.0);
		cached_rate = maxlipo.getChangeRate();
		next_battery_read = millis();		
	}

	// Create a historical record of the rate of change every 5 seconds for ~ 5 minutes
	if (millis() - next_battery_timeupdate > 5000)
	{
		// Only gather stats if the watch is not plugged in, and is discharging.
		if ((tinywatch.vbus_present() == false) && (cached_rate <= 0))
		{
			// Fill the array every couple of seconds,
			if (rate_historyrecords < max_historyrecords)
			{
				rate_history[rate_historyrecords] = cached_rate;
				rate_historyrecords++;
			}
			else
			{
				// FIFO the Array
				for (int x = max_historyrecords - 1; x < 0; x--)
					rate_history[x + 1] = rate_history[x];

				rate_history[rate_historyrecords] = cached_rate;
				rate_historyrecords = max_historyrecords;
				
			}
			next_battery_timeupdate = millis();
			tinywatch.log_system_message("Records: " + String(rate_historyrecords));
		}
	}
}

float Battery::get_rate(bool forced)
{
	update_stats(forced);
	return cached_rate;
}

float Battery::get_voltage(bool forced)
{
	update_stats(forced);
	return cached_voltage;
}

float Battery::get_percent(bool forced)
{

	update_stats(forced);
	return cached_percent;
}

float Battery::get_time_remaining(bool forced)
{
	// TODO: Implement Forced/Cached data;

	float timeRemaining = 0.0;

	if (rate_historyrecords > min_historyrecords) // Don't bother trying to calculate a time without a decent number of records
	{
		float sumratehistory = 0.0;
		for (int x = 0; x < rate_historyrecords; x++)
			sumratehistory += rate_history[x];

		// From the average rate, calculate an estimate time remaining
		float avgrate = sumratehistory / (float)rate_historyrecords;
		timeRemaining = cached_percent / abs(avgrate);

		tinywatch.messages.clear();
		tinywatch.log_system_message("Records: " + String(rate_historyrecords));
		tinywatch.log_system_message("Average Rate: " + String(avgrate));
		tinywatch.log_system_message("Hrs Remain: " + String(timeRemaining));
		// return timeRemaining;
	}
	return timeRemaining;
}

uint8_t Battery::get_alert_status()
{
	uint8_t status = 0;
	if ((millis() - next_battery_read > 500))
	{
		next_battery_read = millis();
		uint8_t status = maxlipo.getAlert(true);
		if (status > 0)
		{
			info_print("BAT Alert Status: ");
			info_println(status);
		}
	}
	return status;
}

void Battery::clear_alert_status()
{
	info_print("BAT Clear Alert Status Bit");

	maxlipo.clearAlert();
}

Battery battery;
