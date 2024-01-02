#pragma once

#include "wifi_common.h"
#include "HTTPClient.h" 
#include "json.h"
#include "json_conversions.h"
#include <functional>
#include <freertos/queue.h>

typedef std::function<void(bool, const String&)> _CALLBACK;

class WifiController
{
	public:
		WifiController();

		bool connect();
		void disconnect(bool force);
		bool is_busy();
		void kill_controller_task();
        bool is_connected();

		// task queue related functions
		void perform_wifi_request(const String &url, _CALLBACK callback);
		void add_to_queue(const String &url, _CALLBACK callback);
		void loop();

		String http_request(const String &url);

		bool wifi_blocks_display = false;
        bool wifi_prevent_disconnect = false;

	private:
		String user_config_json;
		wifi_states current_state = BOOT;
		bool wifi_busy = false;
		unsigned long next_wifi_loop = 0;

		// Structure for task items
		struct wifi_task_item
		{
			String * url;
			_CALLBACK callback;
		};

		// Structure for callback items
		struct wifi_callback_item
		{
			bool success;
			String * response;
			_CALLBACK callback;
		};

		TaskHandle_t wifi_task_handler;
		QueueHandle_t wifi_task_queue;
		QueueHandle_t wifi_callback_queue;

		static void wifi_task(void *pvParameters);
};

extern WifiController wifi_controller;