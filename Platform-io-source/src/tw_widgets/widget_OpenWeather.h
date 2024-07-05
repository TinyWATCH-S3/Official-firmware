#include "json.h"
#include "json_conversions.h"
#include "tw_widget.h"
#include <map>

class WidgetOpenWeather : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool process_touch(touch_event_t touch_event);

		String build_server_path();
		void decode_json(String _data);
		void load_icons();

		void process_weather_data(bool success, const String &response);

		void store_values_for_deepsleep();
		void retrieve_values_for_deepsleep();

	private:
		String version = "1.0";
		String server_path = "http://api.openweathermap.org/data/2.5/weather?q=";
		unsigned long next_update = millis() + 5000;

		bool icons_loaded = false;

		// Cached weather data
		int16_t _temp = -999;
		int16_t _humidity = 0;
		String _icon_name = "";
		String _icon_desc = "";
		String _weather_desc = "";
		uint8_t stat = 0;

		std::map<String, const uint16_t *> ow_icons;

		bool should_store_values_for_deepsleep()
		{
			return (_temp > -999);
		}

		bool deepsleep_data_stored = false;
};

extern WidgetOpenWeather ow_widget;