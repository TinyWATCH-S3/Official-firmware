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
#include "web/www/www_settings_web.h"
#include "web/www/www_settings_themes.h"
#include "web/www/www_debug_logs.h"

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
	else if (var == "FW_VER")
	{
		return (tinywatch.version_firmware + " " + tinywatch.version_year);
	}
	else if (var == "UPDATE_NOTICE")
	{
		if (tinywatch.update_available())
			return tinywatch.version_firmware + " " + tinywatch.version_year + " <a href='https://tinywatch.io/firmware_alpha/' target='_blank'>NEW VERSION AVAILABLE</a>\n";
		else

			return (tinywatch.version_firmware + " " + tinywatch.version_year);
	}
	else if (var == "DEBUG_LOGS")
	{
		String logs = "";
		for (size_t l = 0; l < tinywatch.messages.size(); l++)
		{
			String mess = tinywatch.messages[l].message;
			mess.replace(" %:", " %%:");
			logs += "<div class='row p-1 " + String(l % 2 == 0 ? "alt" : "alt2") + "'>\n";
			logs += "<div class='col-2 text-end nowrap'>" + tinywatch.messages[l].get_time() + "</div>\n";
			logs += "<div class='col-10 text-start'>" + mess + "</div>\n";
			logs += "</div>\n";
		}

		return logs;
	}
	else if (var == "SETTING_OPTIONS_WATCH")
	{
		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::WATCH)
				html += generate_settings_html(i);
		}

		return html;
	}
	else if (var == "SETTING_OPTIONS_WEB")
	{
		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::WEB)
				html += generate_settings_html(i);
		}

		return html;
	}
	else if (var == "SETTING_OPTIONS_WIDGETS")
	{
		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::WIDGET)
				html += generate_settings_html(i);
		}

		return html;
	}
	else if (var == "SETTING_OPTIONS_THEMES")
	{
		return "Soon!";

		String html = "";
		for (size_t i = 0; i < settings.settings_groups.size(); i++)
		{
			if (settings.settings_groups[i].type == SettingType::THEME)
				html += generate_themes_html(i);
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
		if (group_name.groups[i]->req_full_width)
			html += "				<div class='col-12 pb-1'>\n";
		else
			html += "				<div class='col-6 pb-1'>\n";
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

String WebServer::generate_themes_html(int group_id)
{
	auto &group_name = settings.settings_groups[group_id]; // Cache the current group

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

	html += "			<div class ='row'>\n";

	for (size_t i = 0; i < group_name.groups.size(); ++i)
	{
		if (group_name.groups[i]->req_full_width)
			html += "				<div class='col-12 pb-1'>\n";
		else
			html += "				<div class='col-6 pb-1'>\n";
		html += group_name.groups[i]->generate_html(i);
		html += "				</div>\n";
	}
	html += "			</div>\n";

	html += "			<div class ='row m-2 center'>\n";

	html += "			<div class='row' id='add_new_theme' style='display:none;'>\n";
	html += "				<span class='settings_heading'>Add New Theme</span>\n";
	html += "				<div class='settings_frame_inner' style='margin-bottom:15px; padding-bottom:5px;'>\n";
	html += "				<form hx-post='/add_theme'>\n";
	html += "				<div class='row'>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Theme Name</span>\n";
	html += "							<input type='text' class='form-control form-control-sm' id='_name' name='_name' value='' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Widget Style</span>\n";
	html += "							<input type='number' class='form-control form-control-sm' id='_widget_style' name='_widget_style' value='0' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				<div class='row'>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Background Dull</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_background_dull' name='_col_background_dull' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Background Bright</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_background_bright' name='_col_background_bright' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				<div class='row'>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Control Back</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_control_back' name='_col_control_back' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Low Intensity</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_low_intensity' name='_col_low_intensity' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				<div class='row'>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Primary</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_primary' name='_col_primary' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Secondary</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_secondary' name='_col_secondary' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				<div class='row'>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Warning</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_warning' name='_col_warning' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 p-2'>\n";
	html += "						<div class='input-group input-group-sm'>\n";
	html += "							<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Error</span>\n";
	html += "							<input type='color' class='form-control form-control-color' id='_col_error' name='_col_error' value='#222222' />\n";
	html += "						</div>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				<div class='row'>\n";

	html += "					<div class='col-6 align-middle' style='height:36px;'>\n";
	html += "						<button type='button' class='btn btn-sm btn-primary m-1' onclick='toggle_new_theme(false);' style='width:100px;'>Cancel</button>\n";
	html += "					</div>\n";
	html += "					<div class='col-6 right align-middle' style='height:36px;'>\n";
	html += "						<button type='submit' class='btn btn-sm btn-success m-1' style='width:100px;'>Save</button>\n";
	html += "					</div>\n";
	html += "				</div>\n";
	html += "				</form>\n";
	html += "				</div>\n";
	html += "			</div>\n";

	html += "			</div>\n";

	html += "			<div class ='row' id='add_new_theme_button' style='display:block;'>\n";
	html += "				<div class='col-6 align-middle' style='height:36px;'>\n";
	html += "					<button type='button' class='btn btn-sm btn-success m-1' onclick='toggle_new_theme(true);' style='width:100px;'>Add Theme</button>\n";
	html += "				</div>\n";
	html += "			</div>\n";
	html += "		</div>\n";
	html += "	</div>\n";

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
			settings.config.mdns_name = "tinywatch";

		if (!MDNS.begin(settings.config.mdns_name.c_str()))
		{
			error_println("Error starting mDNS");
			wifi_controller.disconnect(false);
			WiFi.mode(WIFI_OFF);
			_running = false;
			return;
		}

		wifi_controller.add_to_queue("https://tinywatch.io/latestver", [this](bool success, const String &response) { this->process_version(success, response); });

		web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.on("/index.htm", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });

		web_server.on("/web_settings_apps.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_apps_html, processor); });

		web_server.on("/debug_logs.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", debug_logs_html, processor); });

		web_server.on("/web_settings_widgets.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_widgets_html, processor); });

		web_server.on("/web_settings_themes.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_themes_html, processor); });

		web_server.on("/web_settings_web.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_settings_web_html, processor); });

		web_server.onNotFound([](AsyncWebServerRequest *request) { request->send(404, "text/plain", "Not found"); });

		// web_server.on("/update_widget_ow", HTTP_POST, [](AsyncWebServerRequest *request) {
		// 	AsyncWebParameter *ow_enable = request->getParam("_set_widget_ow_enable", true);
		// 	settings.config.open_weather.enabled = (String(ow_enable->value().c_str()) == "1");

		// 	AsyncWebParameter *ow_api_key = request->getParam("_set_widget_ow_api_key", true);
		// 	settings.config.open_weather.api_key = String(ow_api_key->value().c_str());
		// 	settings.config.open_weather.api_key.trim();

		// 	AsyncWebParameter *ow_poll_frequency = request->getParam("_set_widget_ow_poll_frequency", true);
		// 	settings.config.open_weather.poll_frequency = String(ow_poll_frequency->value().c_str()).toInt();

		// 	info_println("Widget OW Save!");

		// 	Buzzer({{2000, 100}});
		// 	request->send(200, "text/plain", "Settings Saved!");
		// });

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

							// info_printf("Looking for id: %s - ", fn_indexed.c_str());

							if (request->hasParam(fn_indexed, true))
							{
								// info_print("Found - ");
								AsyncWebParameter *_param = request->getParam(fn_indexed, true);
								int data = String(_param->value().c_str()).toInt();

								// info_printf("Web data: %d, class data %d, change? %s\n", data, intPtr->get(v), (intPtr->update(v, data) ? "YES" : "no"));
								intPtr->update(v, data);
							}
						}
					}
					else if (setting->getType() == SettingsOptionBase::WIFI_STATION)
					{
						SettingsOptionWiFiStations *intPtr = static_cast<SettingsOptionWiFiStations *>(setting);
						for (size_t v = 0; v <= intPtr->vector_size(); v++)
						{
							String fn_ssid = String(group_id) + "," + String(i) + "__" + fn + "_ssid_" + String(v);
							String fn_pass = String(group_id) + "," + String(i) + "__" + fn + "_pass_" + String(v);

							// 1,6__wifi_stations_ssid_0
							if (request->hasParam(fn_ssid, true) && request->hasParam(fn_pass, true))
							{
								// info_print("Found - ");
								AsyncWebParameter *_param1 = request->getParam(fn_ssid, true);
								String data1 = String(_param1->value().c_str());

								AsyncWebParameter *_param2 = request->getParam(fn_pass, true);
								String data2 = String(_param2->value().c_str());

								data1.trim();
								data2.trim();

								if (v == intPtr->vector_size())
								{
									// this is a new one, so we add it, assuming there is data to add
									if (!data1.isEmpty() && !data2.isEmpty())
										intPtr->add_station(data1, data2);
								}
								else
								{
									// We update all stations, even with empty data, to ensure we keep the vector intact
									// after the updates, we'll prune any that are empty.
									intPtr->update(v, data1, data2);
								}
							}
						}

						// remove empty entries
						intPtr->remove_if_empty();
					}
					else
					{
						String fn_indexed = String(group_id) + "," + String(i) + "__" + fn;

						// info_printf("Looking for id: %s - ", fn_indexed.c_str());

						if (request->hasParam(fn_indexed, true))
						{
							AsyncWebParameter *_param = request->getParam(fn_indexed, true);

							if (setting->getType() == SettingsOptionBase::BOOL)
							{
								bool data = (String(_param->value().c_str()) == "1");
								SettingsOptionBool *intPtr = static_cast<SettingsOptionBool *>(setting);

								// info_printf("Web data (new): %s, class data (current): %s - ", (data ? "T" : "F"), (intPtr->get() ? "T" : "F"));
								bool updated = intPtr->update(data);
								// info_printf("Now: %s - changed? %s\n", (intPtr->get() ? "T" : "F"), (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::FLOAT)
							{
								float data = String(_param->value().c_str()).toFloat();
								SettingsOptionFloat *intPtr = static_cast<SettingsOptionFloat *>(setting);
								// info_printf("Web data: %f, class data %f - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								// info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::STRING)
							{
								String data = String(_param->value().c_str());
								SettingsOptionString *intPtr = static_cast<SettingsOptionString *>(setting);
								// info_printf("Web data: %s, class data %s - ", data, intPtr->get());
								bool updated = intPtr->update(&data);
								// info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::INT)
							{
								int data = String(_param->value().c_str()).toInt();
								SettingsOptionInt *intPtr = static_cast<SettingsOptionInt *>(setting);
								// info_printf("Web data: %d, class data %d - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								// info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							else if (setting->getType() == SettingsOptionBase::INT_RANGE)
							{
								int data = String(_param->value().c_str()).toInt();
								SettingsOptionIntRange *intPtr = static_cast<SettingsOptionIntRange *>(setting);
								// info_printf("Web data: %d, class data %d - ", data, intPtr->get());
								bool updated = intPtr->update(data);
								// info_printf("changed? %s\n", (updated ? "YES" : "no"));
							}
							// info_printf("Data: %s\n", String(_param->value().c_str()));
							// 	settings.config.look_ahead = String(_set_reflow_lookahead->value().c_str()).toInt();
						}
					}
				}

				Buzzer({{2000, 20}});
				settings.save(true);
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

void WebServer::process_version(bool success, const String &response)
{
	try
	{
		json data = json::parse(response);

		uint16_t latest_version = data["latest_version"];

		info_printf("Latest Version: %d, Build Version: %d, Should notify? %s\n", latest_version, tinywatch.version_build, String(latest_version > tinywatch.version_build ? "YES!" : "no"));
		tinywatch.version_latest = latest_version;
	}
	catch (json::exception &e)
	{
		info_println("Verion Check parse error:");
		info_println(e.what());
	}
}

WebServer web_server;