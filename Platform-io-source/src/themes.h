#pragma once

#include "Arduino.h"
#include "display.h"
#include "json.h"
#include "json_conversions.h"
#include "utilities/logging.h"
#include <vector>

using json = nlohmann::json;

struct theme
{
		String name;
		uint32_t col_background_dull = 0;
		uint32_t col_background_bright = 0;
		uint32_t col_control_back = 0;
		uint32_t col_primary = 0;
		uint32_t col_secondary = 0;
		uint32_t col_low_intensity = 0;
		uint32_t col_warning = 0;
		uint32_t col_error = 0;
		uint8_t widget_style = 0;
};

struct WatchThemes
{
		std::vector<theme> themes;
		json last_saved_data;
};

class Themes
{
	public:
		Themes()
		{
			default_theme.name = "TinyWATCH";
			default_theme.col_background_bright = RGB(0x45, 0x45, 0x45);
			default_theme.col_background_dull = RGB(0x20, 0x20, 0x20);
			default_theme.col_control_back = RGB(0x66, 0x66, 0x66);
			default_theme.col_primary = RGB(0x00, 0x65, 0xff);
			default_theme.col_secondary = RGB(0xFF, 0x84, 0x11);
			default_theme.col_low_intensity = RGB(0x00, 0x00, 0x00);
			// default_theme.col_highlight = RGB(0x99, 0xFF, 0x99);
			default_theme.col_warning = RGB(0xff, 0x65, 0x00);
			default_theme.col_error = RGB(0xff, 0x00, 0x00);
		};

		theme &current();

		bool load();
		bool save();
		void print_file(void);

	private:
		WatchThemes watch_themes;
		theme default_theme;
		static constexpr const char *filename = "/themes.json";
		static constexpr const char *tmp_filename = "/tmp_themes.json";
};

extern Themes themes;