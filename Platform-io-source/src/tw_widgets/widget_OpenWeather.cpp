#include "tw_widgets/widget_OpenWeather.h"
#include "bitmaps/bitmaps_ow.h"
#include "settings/settings.h"
#include "web/wifi_controller.h"

using json = nlohmann::json;

String WidgetOpenWeather::build_server_path()
{
	if (settings.config.city == "" || settings.config.country == "")
		return "";

	// return server_path + settings.config.city + "," + settings.config.country + "&APPID=" + settings.config.ow_api_key;
	return server_path + settings.config.city + "," + settings.config.country + "&APPID=" + settings.config.open_weather.api_key + "&units=metric";
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

		json data = json::parse(response);

		json main = data["main"];
		if (main.is_object())
		{
			_temp = (uint16_t)(main.value("temp", 0));
			_humidity = (uint16_t)(main.value("humidity", 0));
		}

		json weather = data["weather"];
		if (weather.is_array())
		{
			_icon_name = weather[0]["icon"];
			_icon_desc = weather[0]["description"];
			info_println(_icon_name + " - " + _icon_desc);
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
	if (millis() - next_update > settings.config.open_weather.poll_frequency || next_update == 0)
	{
		next_update = millis();

		if (!icons_loaded)
		{
			icons_loaded = true;
			load_icons();
		}

		if (settings.config.open_weather.api_key.isEmpty())
		{
			// No OW key assigned, so dont display anything and exit.
			return;
		}

		// Let's see if we can get the data from openweather.org
		String url = build_server_path();
		if (!url.isEmpty())
		{
			wifi_controller.add_to_queue(url, [](bool success, const String &response) { ow_widget.process_weather_data(success, response); });
		}
	}

	// Only show the widget visuals is the temp has been set (> -999)
	if (_temp > 0)
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
}

bool WidgetOpenWeather::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	stat++;
	if (stat == 2)
		stat = 0;
	return true;
}

bool WidgetOpenWeather::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool WidgetOpenWeather::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
