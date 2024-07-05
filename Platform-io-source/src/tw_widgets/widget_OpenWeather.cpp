#include "tw_widgets/widget_OpenWeather.h"
#include "bitmaps/bitmaps_ow.h"
#include "settings/settings.h"
#include "web/wifi_controller.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "fonts/Roboto_Regular18.h"
#include "tinywatch.h"

using json = nlohmann::json;

String WidgetOpenWeather::build_server_path()
{
	if (settings.config.city == "" || settings.config.country == "")
		return "";

	// return server_path + settings.config.city + "," + settings.config.country + "&APPID=" + settings.config.ow_api_key;
	return server_path + settings.config.city + "," + settings.config.country + "&APPID=" + settings.config.open_weather.api_key + "&units=" + (settings.config.open_weather.units_metric ? "metric" : "imperial");
}

void WidgetOpenWeather::load_icons()
{
	// Day Icons
	ow_icons["01d"] = ow_icon_01d;
	ow_icons["02d"] = ow_icon_02d;
	ow_icons["03d"] = ow_icon_03d;
	ow_icons["04d"] = ow_icon_04d;
	ow_icons["09d"] = ow_icon_09d;
	ow_icons["10d"] = ow_icon_10d;
	ow_icons["11d"] = ow_icon_11d;
	ow_icons["13d"] = ow_icon_13d;
	ow_icons["50d"] = ow_icon_50d;

	// Night Icons
	ow_icons["01n"] = ow_icon_01n;
	ow_icons["02n"] = ow_icon_02n;
	ow_icons["03n"] = ow_icon_03n;
	ow_icons["04n"] = ow_icon_04n;
	ow_icons["09n"] = ow_icon_09n;
	ow_icons["10n"] = ow_icon_10n;
	ow_icons["11n"] = ow_icon_11n;
	ow_icons["13n"] = ow_icon_13d; // same as day icon
	ow_icons["50n"] = ow_icon_50d; // same as day icon
}

void WidgetOpenWeather::process_weather_data(bool success, const String &response)
{
	try
	{
		_temp = -999;

		info_println(response);

		json data = json::parse(response);

		json main = data["main"];
		if (main.is_object())
		{
			_temp = (uint16_t)(main.value("temp", 0));
			_humidity = (uint16_t)(main.value("humidity", 0));
			info_printf("Temp: %d, Humidity: %d\n", _temp, _humidity);
		}

		json weather = data["weather"];
		if (weather.is_array())
		{
			_icon_name = weather[0]["icon"];
			_icon_desc = weather[0]["description"];
			_weather_desc = weather[0]["main"];
			_weather_desc.toUpperCase();
			info_println(_icon_name + " - " + _icon_desc + "(" + _weather_desc + ")");
		}
	}
	catch (json::exception &e)
	{
		info_println("OW Json parse error:");
		info_println(e.what());
		next_update = 0;
	}
}

void WidgetOpenWeather::draw(uint canvasid, uint8_t style_hint)
{
	// we want the poll_frequency to be (mins in millis, so mins * 60 * 1000)
	if (millis() - next_update > (settings.config.open_weather.poll_frequency * 60000) || next_update == 0 || deepsleep_data_stored)
	{

		if (settings.config.open_weather.api_key.isEmpty() || !settings.config.open_weather.enabled)
		{
			// No OW key assigned, or widget not enabled, so dont display anything and exit.
			next_update = millis();
			return;
		}

		if (!icons_loaded)
		{
			icons_loaded = true;
			load_icons();

			// deepsleep_data_stored = true;
			tinywatch.register_pre_ds_callback([this]() { this->store_values_for_deepsleep(); });
			tinywatch.register_post_ds_callback([this]() { this->retrieve_values_for_deepsleep(); });

			next_update = millis() + 5000;
			return;
		}

		// Let's see if we can get the data from openweather.org
		String url = build_server_path();
		if (!url.isEmpty() && !deepsleep_data_stored)
		{
			tinywatch.log_system_message("Getting OW data from openweather.org");
			tinywatch.log_system_message("next_update: " + String(next_update) + " millis(): " + String(millis()) + " deepsleep_data_stored? " + (deepsleep_data_stored ? "T" : "F"));
			tinywatch.log_system_message(url);
			wifi_controller.add_to_queue(url, [this](bool success, const String &response) { this->process_weather_data(success, response); });
		}

		deepsleep_data_stored = false;
		next_update = millis();
	}

	// Only show the widget visuals is the temp has been set (> -999)
	if (_temp > 0)
	{
		if (false)
		{
			canvas[canvasid].fillRoundRect(pos_x, pos_y, width, height, 8, 0);
			canvas[canvasid].drawRoundRect(pos_x, pos_y, width, height, 8, RGB(0x44, 0x44, 0x44));
			canvas[canvasid].setTextColor(RGB(0xAA, 0xAA, 0xCC));

			if (ow_icons.count(_icon_name) > 0)
				canvas[canvasid].pushImage(pos_x + (width / 2 - 36), pos_y + 2, 72, 72, ow_icons[_icon_name]);

			canvas[canvasid].setTextDatum(4); // Middle, Center
			if (stat == 0)
				canvas[canvasid].drawString(String(_temp) + "C", pos_x + (width / 2), pos_y + (height)-16);
			else if (stat == 1)
				canvas[canvasid].drawString(String(_humidity) + "%", pos_x + (width / 2), pos_y + (height)-16);
		}
		else
		{
			int center_y = 131;
			canvas[canvasid].setTextDatum(TR_DATUM);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[13]);
			canvas[canvasid].setTextColor(RGB(0x15, 0x15, 0x15), RGB(0x45, 0x45, 0x45));
			canvas[canvasid].drawString(_weather_desc.substring(0, 6), display.center_x - 10, center_y - 43);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[16]);
			canvas[canvasid].drawString(String(_humidity) + "%", display.center_x - 10, center_y - 21);
			canvas[canvasid].setFreeFont(&Roboto_Regular18);
			canvas[canvasid].setTextColor(RGB(0x00, 0x45, 0xaa), RGB(0x45, 0x45, 0x45));
			canvas[canvasid].drawString(String(_temp) + (settings.config.open_weather.units_metric ? "C" : "F"), display.center_x - 10, center_y + 8);
		}
	}
	else
	{
		// info_printf("DRAW - Temp: %d, Humidity: %d\n", _temp, _humidity);
	}
}

bool WidgetOpenWeather::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		stat++;
		if (stat == 2)
			stat = 0;
		return true;
	}

	return false;
}

void WidgetOpenWeather::store_values_for_deepsleep()
{
	tinywatch.ds_store_uint32("nupdate", next_update);
	tinywatch.ds_store_int32("temp", _temp);
	tinywatch.ds_store_int32("hum", _humidity);
	tinywatch.ds_store_string("i_desc", _icon_desc.c_str());
	tinywatch.ds_store_string("i_name", _icon_name.c_str());
	tinywatch.ds_store_string("w_desc", _weather_desc.c_str());

	info_println("Storing OW data in NVS pre going to sleep");
}

void WidgetOpenWeather::retrieve_values_for_deepsleep()
{
	next_update = millis() + tinywatch.ds_retrieve_uint32("nupdate");
	_temp = tinywatch.ds_retrieve_int32("temp");
	_humidity = tinywatch.ds_retrieve_int32("hum");
	_icon_desc = tinywatch.ds_retrieve_string("i_desc");
	_icon_name = tinywatch.ds_retrieve_string("i_name");
	_weather_desc = tinywatch.ds_retrieve_string("w_desc");

	info_println("Getting OW data from NVS after waking from sleep");
	tinywatch.log_system_message("Getting OW data from NVS after waking from sleep");
	tinywatch.log_system_message("next_update: " + String(next_update) + "stored temp: " + String(_temp));

	deepsleep_data_stored = true;
}

WidgetOpenWeather ow_widget;
