#include "web/webserver.h"
#include "peripherals/buzzer.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "web/wifi_controller.h"

// HTML Templates
#include "web/www/www_general.h"
#include "web/www/www_settings_watch.h"
#include "web/www/www_settings_widgets.h"
#include "web/www/www_settings_apps.h"

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
	// WIDGETS
	else if (var == "SET_WID_OW_ENABLE_YES")
	{
		return (settings.config.open_weather.enabled ? "selected" : "");
	}
	else if (var == "SET_WID_OW_ENABLE_NO")
	{
		return (settings.config.open_weather.enabled ? "" : "selected");
	}
	else if (var == "SET_WID_OW_API_KEY")
	{
		return String(settings.config.open_weather.api_key);
	}
	else if (var == "SET_WID_OW_POLL_FREQ")
	{
		return String(settings.config.open_weather.poll_frequency);
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

		web_server.on("/web_settings_apps.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_apps_html, processor); });

		web_server.on("/web_settings_widgets.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_widgets_html, processor); });

		web_server.onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found"); });

		web_server.on("/update_widget_ow", HTTP_POST, [](AsyncWebServerRequest *request) {
			AsyncWebParameter *ow_enable = request->getParam("_set_widget_ow_enable", true);
			settings.config.open_weather.enabled = (String(ow_enable->value().c_str()) == "1");

			AsyncWebParameter *ow_api_key = request->getParam("_set_widget_ow_api_key", true);
			settings.config.open_weather.api_key = String(ow_api_key->value().c_str());
			settings.config.open_weather.api_key.trim();

			AsyncWebParameter *ow_poll_frequency = request->getParam("_set_widget_ow_poll_frequency", true);
			settings.config.open_weather.poll_frequency = String(ow_poll_frequency->value().c_str()).toInt();

			Buzzer({{2000, 20}});
			request->send(200);
		});

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