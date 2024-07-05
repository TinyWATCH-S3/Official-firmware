#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <vector>

typedef std::function<void(void)> _CALLBACK_DS;

enum CPU_SPEED
{
	CPU_CHANGE_LOW = 0,
	CPU_CHANGE_HIGH = 1,
};

struct log_message
{
		float timestamp = 0;
		String message = "";

		log_message(float tm, const String &mess) : timestamp(tm), message(mess){};

		String get_time()
		{
			return String((int)timestamp) + "&nbsp;sec";
			// if (timestamp < 60)
			// 	return String((int)timestamp)+ "sec";
			// else if
		}
};

class TinyWATCH
{
		// using CallbackFunction = void (*)();

	public:
		bool was_sleeping();
		int woke_by();
		void go_to_sleep();
		bool vbus_present();
		void set_cpu_frequency(uint32_t freq, CPU_SPEED speed);
		void log_system_message(String txt);
		bool update_available();

		bool block_update_low_battery = false;
		unsigned long low_battery_update_check = 0;

		const String version_firmware = "v0.64";
		const String version_year = "2024";
		const uint16_t version_build = 3;
		uint16_t version_latest = 0;

		bool hw_revision_p7_or_later = false;

		std::vector<log_message> messages;

		static void get_public_ip(bool success, const String &response);
		static void get_and_update_utc_settings(bool success, const String &response);

		// NVS stuff for pre/post DS storage
		void ds_remove(const char *key);
		void ds_store_string(const char *key, const char *data);
		void ds_store_int32(const char *key, int32_t data);
		void ds_store_uint32(const char *key, uint32_t data);
		String ds_retrieve_string(const char *key);
		int32_t ds_retrieve_int32(const char *key);
		uint32_t ds_retrieve_uint32(const char *key);

		void register_pre_ds_callback(_CALLBACK_DS _callback);
		void register_post_ds_callback(_CALLBACK_DS _callback);

		std::vector<_CALLBACK_DS> pre_ds_callbacks;
		std::vector<_CALLBACK_DS> post_ds_callbacks;

	protected:
		//

	private:
		Preferences nvs;
		const char *deep_sleep_store_name = "ds_store";
};

extern TinyWATCH tinywatch;