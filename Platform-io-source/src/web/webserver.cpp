#include "web/webserver.h"
#include "peripherals/buzzer.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "web/webserver_templates.h"
#include "web/wifi_controller.h"

String WebServer::processor(const String &var)
{
	if (var == "META")
	{
		return String(meta);
	}
	if (var == "FOOTER")
	{
		return String(footer);
	}
	else if (var == "THEME")
	{
		if (settings.config.website_darkmode)
			return "dark";
		else
			return "light";
	}
	else if (var == "CSS")
	{
		if (settings.config.website_darkmode)
		{
			return String(css_dark);
		}
		else
		{
			return String(css);
		}
	}

	return "";
}

void WebServer::start()
{
	setCpuFrequencyMhz(80);
	_running = true;
	wifi_controller.wifi_prevent_disconnect = true;

	info_println("Starting webserver");

	// Start the WiFi
	WiFi.mode(WIFI_STA);

	// Start the webserver by connecting to the wifi network first, done via a non-blocking callback
	wifi_controller.add_to_queue("", [this](bool success, const String &response) { this->start_callback(success, response); });
}

void WebServer::start_callback(bool success, const String &response)
{
	if (response == "OK")
	{
		info_print("IP Address: ");
		info_println(WiFi.localIP());

		// Set the local mDSN name so you can navigate to tinywatchs3.local instead of IP address
		settings.config.mdns_name.trim();
		if (settings.config.mdns_name.isEmpty())
			settings.config.mdns_name = "tinywatchs3";

		if (!MDNS.begin(settings.config.mdns_name.c_str()))
		{
			error_println("Error starting mDNS");
			wifi_controller.disconnect(false);
			WiFi.mode(WIFI_OFF);
			_running = false;
			return;
		}

		web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.on("/index.htm", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found"); });

		info_println("web_server.begin();");
		web_server.begin();
		_running = true;
	}
	else
	{
		_running = false;
		error_println("Failed to connect to wifi to start webserver!");
		setCpuFrequencyMhz(40);
	}
}

void WebServer::stop(bool restart)
{
	info_println("Webserver stop");
	web_server.end();
	wifi_controller.wifi_prevent_disconnect = false;
	wifi_controller.disconnect(true);
	_running = false;
}

void WebServer::process() {}

bool WebServer::is_running() { return _running; }

WebServer web_server;