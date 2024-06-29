#include "wifi_common.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "settings/settings.h"

class WebServer
{

	public:
		WebServer() : web_server(80) {}

		void start();
		void start_callback(bool success, const String &response);
		void stop(bool restart);
		void process();
		bool is_running();

		static String processor(const String &var);

		static String generate_settings_html(int group);

		SettingsOptionBase *get_obj_from_id(String id);

	private:
		bool _running = false;
		AsyncWebServer web_server;
		DNSServer dns_server;
};

extern WebServer web_server;