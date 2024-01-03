#include "web/wifi_controller.h"
#include "settings/settings.h"
#include "utilities/logging.h"

using json = nlohmann::json;

// Initialise the controller, create the incoming and outgoing queues and start the process task
WifiController::WifiController()
{
    // Create the task and callback queues
    wifi_task_queue = xQueueCreate(10, sizeof(wifi_task_item));
    wifi_callback_queue = xQueueCreate(10, sizeof(wifi_callback_item));

    if (wifi_task_queue == NULL || wifi_callback_queue == NULL) {
        Serial.println("Error creating the queues");
        return;
    }

    // Start the WiFi task
    xTaskCreate(WifiController::wifi_task, "wifi_task", 8192, this, 5, &wifi_task_handler);
}

// Kill the pinned threaded task
void WifiController::kill_controller_task()
{
	info_println("Killing WiFi queue task!");
	vTaskDelete(wifi_task_handler);
}

// Return the busy state of the WiFi queue
bool WifiController::is_busy()
{
	return wifi_busy;
}

bool WifiController::is_connected()
{
    return (WiFi.status() == WL_CONNECTED);
}

// Connect to the WiFi network
bool WifiController::connect()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		wifi_busy = false;
		return true;
	}

	WiFi.disconnect(true);
	if (!settings.has_wifi_creds())
	{
		wifi_busy = false;
		return false;
	}
	else
	{
        delay(500);
		wifi_busy = true;
		WiFi.mode(WIFI_STA);
		WiFi.begin(settings.config.wifi_ssid, settings.config.wifi_pass);

		unsigned long start_time = millis();
		// Time out the connection if it takes longer than 5 seconds
		while ((millis() - start_time < 5000) && WiFi.status() != WL_CONNECTED)
		{
			delay(500);
		}
	}

	wifi_busy = false;
	return (WiFi.status() == WL_CONNECTED);
}

// Disconnect from the WiFi network
void WifiController::disconnect(bool force)
{
    info_println("wifi disconnect, forced? "+String(force));
    if (!wifi_prevent_disconnect || force)
    {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }
	wifi_busy = false;
}

// Process the task queue - called from the main thread in loop() in tinywatch.cpp
// only process every 1 seconds
void WifiController::loop()
{
	if (millis() - next_wifi_loop > 1000)
	{
		next_wifi_loop = millis();
		wifi_callback_item result;
		while (xQueueReceive(wifi_callback_queue, &result, 0) == pdTRUE)
		{
			result.callback(result.success, *result.response);
			delete result.response;
		}
	}
}

// Make an HTTP request and return the result as a String
String WifiController::http_request(const String &url)
{
	WiFiClient client;
	HTTPClient http;
	String payload = "ERROR";

	int http_code = -1;
	unsigned long timer = millis();

	String url_lower = url;
	url_lower.toLowerCase();

	bool is_https = (url_lower.substring(0, 5) == "https");

	while (millis() - timer < 5000)
	{
		if (is_https)
			http.begin(url.c_str());
		else
			http.begin(client, url.c_str());

		http_code = http.GET();  //send GET request
		info_println("Response Code: "+String(http_code));
		if (http_code == 200)
		{
			payload = http.getString();
			http.end();
			break;
		}
		http.end();
		delay(500);
	}

	return payload;
}

// Function to call out to the HTTP Request and then add the result to the outgoing queue 
void WifiController::perform_wifi_request(const String &url, _CALLBACK callback)
{
    bool success = true;
    String response = "OK";

    // Only process if there is an actual URL, otherwise do the callback
    if (!url.isEmpty())
    {
        response = http_request(url);
        success = (response != "ERROR"); // or false, based on the HTTP request result
    }

	// Create a wifi_callback_item and enqueue it
	wifi_callback_item result = {success, new String(response), callback};
	xQueueSend(wifi_callback_queue, &result, portMAX_DELAY);
}

// Task for processing the queue items
void WifiController::wifi_task(void *pvParameters)
{
	WifiController* controller = static_cast<WifiController*>(pvParameters);
    while (true)
	{
		wifi_task_item item;
		if (xQueueReceive(controller->wifi_task_queue, &item, portMAX_DELAY) == pdTRUE)
		{
			// Connect to WiFi / pass through if already connected
			if (controller->connect())
			{
				controller->wifi_busy = true;
				// Perform the request
				controller->perform_wifi_request(*item.url, item.callback);
				delete item.url;

				if (uxQueueMessagesWaiting(controller->wifi_task_queue) == 0 && WiFi.status() == WL_CONNECTED)
				{
					// If we are connected and the queue is empty, disconnect WiFi - maybe?
					if (!controller->wifi_blocks_display && !settings.config.wifi_start && !controller->wifi_prevent_disconnect)
						controller->disconnect(false);
				}
				controller->wifi_busy = false;
			}
		}
    }
}

// Function to add items to the queue
void WifiController::add_to_queue(const String &url, _CALLBACK callback)
{
	wifi_task_item item = {new String(url), callback};
    if (*item.url == "")
        info_println("Adding request to connect to wifi if not connected!");
    else
	    info_println("Adding request to "+*item.url);
	xQueueSend(wifi_task_queue, &item, portMAX_DELAY);
}

WifiController wifi_controller;
