#include "wifi_common.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFi.h>

class WebServer
{

	public:
		WebServer() : web_server(80)
		{
		}

		void start();
        void start_callback(bool success, const String &response);
		void stop(bool restart);
		void process();
		bool is_running();

		static String processor(const String &var);

	private:
		bool _running = false;
		AsyncWebServer web_server;
		DNSServer dns_server;

};

extern WebServer web_server;