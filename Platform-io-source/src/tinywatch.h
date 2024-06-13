#pragma once
#include <Arduino.h>
#include <vector>

enum CPU_SPEED
{
	CPU_CHANGE_LOW = 0,
	CPU_CHANGE_HIGH = 1,
};

class TinyWATCH
{
	public:
		bool was_sleeping();
		int woke_by();
		void go_to_sleep();
		bool vbus_present();
		void set_cpu_frequency(uint32_t freq, CPU_SPEED speed);
		void log_system_message(String txt);

		bool block_update_low_battery = false;
		unsigned long low_battery_update_check = 0;

		const String version_firmware = "v0.4";
		const String version_year = "2024";

		std::vector<String> messages;

		static void get_public_ip(bool success, const String &response);
		static void get_and_update_utc_settings(bool success, const String &response);

	protected:
		//

	private:
		//
};

extern TinyWATCH tinywatch;