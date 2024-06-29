#include "web/webserver.h"
#include "peripherals/buzzer.h"
#include "peripherals/haptics.h"
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

	else if (var == "SETTING_OPTIONS")
	{
		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::CONTROL)
				html += generate_settings_html(i);
		}

		return html;
	}
	else if (var == "WIDGET_OPTIONS")
	{
		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::WIDGET)
				html += generate_settings_html(i);
		}

		return html;
	}

	return "";
}

String WebServer::generate_settings_html(int group_id)
{
	auto &group_name = settings.settings_groups[group_id]; // Cache the current group

	if (group_name.name.indexOf("Haptics") != -1 && haptics.available == false)
		return "";

	String group_id_str = String(group_id);

	String html = "\n<div id='settings_group_" + group_id_str + "'>\n";
	html += "	<span class='settings_heading'>" + group_name.name + "</span>\n";
	html += "	<div class='settings_frame' id='group_" + group_id_str + "' style='margin-bottom:15px; padding-bottom:5px;'>\n";

	if (group_name.description != "")
	{
		html += "		<div class='center w-100 mt-1 mb-2'>\n";
		html += "			<span class='settings_info'>" + group_name.description + "</span>\n";
		html += "		</div>\n";
	}

	html += "		<form hx-post='/update_settings_group' hx-target='#settings_group_" + group_id_str + "' >\n";
	html += "			<input type='hidden' name='group_id' id='group_id' value='" + group_id_str + "'>\n";
	html += "			<div class ='row'>\n";

	for (size_t i = 0; i < group_name.groups.size(); ++i)
	{
		html += "				<div class='col-6'>\n";
		html += group_name.groups[i]->generate_html(i);
		html += "				</div>\n";
	}
	html += "			</div>\n";

	html += "			<div class ='row'>\n";
	html += "				<div class='col-12 right align-middle' style='height:36px;'>\n";
	html += "					<span class='flash-span me-2' style='display:none; color:green;'>Settings Updated!</span>\n";
	html += "					<button type='submit' class='btn btn-sm btn-success m-1' style='width:100px;'>Update</button>\n";
	html += "				</div>\n";
	html += "			</div>\n";
	html += "		</form>\n";
	html += "	</div>\n";
	html += "</div>\n";

	return html;
}

SettingsOptionBase *WebServer::get_obj_from_id(String id)
{
	// Split the string at "__"
	int pos = id.indexOf("__");
	String firstPart = id.substring(0, pos);

	// Split the first part at ","
	int commaIndex = firstPart.indexOf(",");
	int group = firstPart.substring(0, commaIndex).toInt();
	int index = firstPart.substring(commaIndex + 1).toInt();

	return (settings.settings_groups[group].groups[index]);
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

			info_println("Widget OW Save!");

			Buzzer({{2000, 100}});
			request->send(200, "text/plain", "Settings Saved!");
		});

		web_server.on("/update_settings_group", HTTP_POST, [](AsyncWebServerRequest *request) {
			if (request->hasParam("group_id", true))
			{
				AsyncWebParameter *_group = request->getParam("group_id", true);
				info_printf("** Save Settings for Group ID: %s\n", String(_group->value().c_str()));
				uint8_t group_id = String(_group->value().c_str()).toInt();

				auto &group = settings.settings_groups[group_id]; // Cache the current group

				for (size_t i = 0; i < group.groups.size(); ++i)
				{
					auto *setting = group.groups[i]; // Cache the current setting

					String fn = setting->fieldname;
					fn.replace(" ", "_");
					fn.toLowerCase();
					fn.replace("_(sec)", "");
					fn.replace("_(min)", "");
					fn.replace("_(%%)", "");

					if (setting->getType() == SettingsOptionBase::INT_VECTOR)
					{
						SettingsOptionIntVector *intPtr = static_cast<SettingsOptionIntVector *>(setting);
						for (size_t v = 0; v < intPtr->vector_size(); v++)
						{
							String fn_indexed = String(group_id) + "," + String(i) + "__" + fn + "_" + String(v);

							info_printf("Looking for id: %s - ", fn_indexed.c_str());

							if (request->hasParam(fn_indexed, true))
							{
								info_print("Found - ");
								AsyncWebParameter *_param = request->getParam(fn_indexed, true);
								int data = String(_param->value().c_str()).toInt();

								info_printf("Web data: %d, class data %d, change? %s\n", data, intPtr->get(v), (intPtr->update(v, data) ? "YES" : "no"));
								intPtr->update(v, data);
							}
						}
					}
					else
					{
						String fn_indexed = String(group_id) + "," + String(i) + "__" + fn;

						info_printf("Looking for id: %s - ", fn_indexed.c_str());

						if (request->hasParam(fn_indexed, true))
						{
							AsyncWebParameter *_param = request->getParam(fn_indexed, true);

							if (setting->getType() == SettingsOptionBase::BOOL)
							{
								bool data = (String(_param->value().c_str()) == "1");
								SettingsOptionBool *intPtr = static_cast<SettingsOptionBool *>(setting);

								info_printf("Web data (new): %s, class data (current): %s - ", (data ? "T" : "F"), (intPtr->get() ? "T" : "F"));
								bool updated = intPtr->update(data);
								info_printf("Now: %s - changed? %s\n", (intPtr->get() ? "T" : "F"), (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::FLOAT)
							{
								float data = String(_param->value().c_str()).toFloat();
								SettingsOptionFloat *intPtr = static_cast<SettingsOptionFloat *>(setting);
								info_printf("Web data: %f, class data %f - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::STRING)
							{
								String data = String(_param->value().c_str());
								SettingsOptionString *intPtr = static_cast<SettingsOptionString *>(setting);
								info_printf("Web data: %s, class data %s - ", data, intPtr->get());
								bool updated = intPtr->update(&data);
								info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::INT)
							{
								int data = String(_param->value().c_str()).toInt();
								SettingsOptionInt *intPtr = static_cast<SettingsOptionInt *>(setting);
								info_printf("Web data: %d, class data %d - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::INT_RANGE)
							{
								int data = String(_param->value().c_str()).toInt();
								SettingsOptionIntRange *intPtr = static_cast<SettingsOptionIntRange *>(setting);
								info_printf("Web data: %d, class data %d - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							// info_printf("Data: %s\n", String(_param->value().c_str()));
							// 	settings.config.look_ahead = String(_set_reflow_lookahead->value().c_str()).toInt();
						}
					}
				}

				Buzzer({{2000, 20}});
				request->send_P(200, "text/html", generate_settings_html(group_id).c_str(), processor);
			}
			else
			{
				request->send_P(200, "text/html", "<div class='container'><h2>ERROR POSTING DATA</h2><div>", processor);
			}
		});

		// web_server.on("/update_settings_watch", HTTP_POST, [](AsyncWebServerRequest *request) {
		// 	for (size_t g = 0; g < settings.setting_groups.size(); g++)
		// 	{
		// 		auto &group = settings.setting_groups[g]; // Cache the current group
		// 		for (size_t i = 0; i < group.size(); ++i)
		// 		{
		// 			auto *setting = group[i]; // Cache the current setting

		// 			String fn = setting->fieldname;
		// 			fn.replace(" ", "_");
		// 			fn.toLowerCase();
		// 			fn.replace("_(sec)", "");
		// 			fn.replace("_(%%)", "");

		// 			if (setting->getType() != SettingsOptionBase::INT_VECTOR)
		// 			{
		// 				fn = String(g) + "," + String(i) + "__" + fn;

		// 				info_printf("Looking for id: %s - ", fn.c_str());

		// 				if (request->hasParam(fn, true))
		// 				{
		// 					info_print("Found - ");
		// 					AsyncWebParameter *_param = request->getParam(fn, true);
		// 					info_printf("Data: %s\n", String(_param->value().c_str()));
		// 					// 	settings.config.look_ahead = String(_set_reflow_lookahead->value().c_str()).toInt();
		// 				}
		// 				else
		// 				{
		// 					info_println("...");
		// 				}
		// 			}
		// 			else
		// 			{
		// 				SettingsOptionIntVector *intPtr = static_cast<SettingsOptionIntVector *>(setting);
		// 				for (size_t v = 0; v < intPtr->vector_size(); v++)
		// 				{
		// 					String fn_indexed = String(g) + "," + String(i) + "__" + fn + "_" + String(v);

		// 					info_printf("Looking for id: %s - ", fn_indexed.c_str());

		// 					if (request->hasParam(fn_indexed, true))
		// 					{
		// 						info_print("Found - ");
		// 						AsyncWebParameter *_param = request->getParam(fn_indexed, true);
		// 						info_printf("Data: %s\n", String(_param->value().c_str()));
		// 						// 	settings.config.look_ahead = String(_set_reflow_lookahead->value().c_str()).toInt();
		// 					}
		// 					else
		// 					{
		// 						info_println("...");
		// 					}
		// 				}
		// 			}
		// 		}
		// 	}

		// 	Buzzer({{2000, 20}});
		// 	request->send(200, "text/plain", "Settings Saved!");
		// });

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