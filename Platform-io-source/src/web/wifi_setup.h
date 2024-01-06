#pragma once

#include "display.h"
#include "wifi_common.h"
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <functional>

class WifiSetup
{
		friend struct scan_responder;
		friend struct connect_responder;

	public:
		WifiSetup() : webServer(80) {}

		String wifi_ap_messages;
		uint32_t wifi_ap_message_color;

		void start();
		void stop(bool restart);
		void process();

		// Successfully collected and validated wifi creds?
		bool is_done() const { return done; }
		String get_ssid() const { return ssid; }
		String get_pass() const { return pass; }

	private:
		void update_wifisetup_status(String status, uint32_t color)
		{
			wifi_ap_messages = status;
			wifi_ap_message_color = color;
		}

		AsyncWebServer webServer;
		DNSServer dnsServer;
		String ssid;
		String pass;
		bool done;
};

extern WifiSetup wifiSetup;
