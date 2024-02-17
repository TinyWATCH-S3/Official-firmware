#include "json.h"
#include "json_conversions.h"
#include "tw_widget.h"
#include <map>

class WidgetOpenWeather : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

		String build_server_path();
		void decode_json(String _data);
		void load_icons();

		void process_weather_data(bool success, const String &response);

	private:
		String version = "1.0";
		String server_path = "http://api.openweathermap.org/data/2.5/weather?q=";
		unsigned long next_update = 0;

		bool icons_loaded = false;

		// Cached weather data
		int16_t _temp = -999;
		int16_t _humidity = 0;
		String _icon_name = "";
		String _icon_desc = "";
		uint8_t stat = 0;

		std::map<String, const uint16_t *> ow_icons;
};

extern WidgetOpenWeather ow_widget;