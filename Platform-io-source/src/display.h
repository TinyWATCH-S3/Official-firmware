#pragma once

#include "Arduino.h"
#include <vector>

enum Directions
{
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
	CLICK = 4,
	CLICK_DBL = 5,
	CLICK_LONG = 6,
	NONE = 99,
};

constexpr uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
	//   return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
	return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}

#include "cst816t.h" // capacitive touch
#include "peripherals/rtc.h"
#include "utilities/logging.h"
#include "web/wifi_controller.h"
#include <TFT_eSPI.h>
#include <Tween.h>

enum display_states
{
	LOADING = 0,
	NORMAL = 1,
};

class tw_face;

class Display
{
	public:
		void init_screen();
		void createFaces(bool sleeping);
		void set_backlight(int level, bool force);
		void update_current_face();
		void process_touch();
		uint get_backlight_period();
		void set_display_state(display_states state, String message);
		void show_loading_icon();
		void show_watch_from_boot();
		void force_save();
		void update_boot_face(wifi_states status);
		bool adjust_backlight();
		void set_backlight_val_direct(uint8_t val);
		void kill_backlight_task();
		uint8_t get_current_backlight_val();

		void add_clock_face(tw_face *face);
		void cycle_clock_face();

		void set_current_face(tw_face *face);
		tw_face *set_current_clock_face(bool should_draw);
		void show_low_battery();

		display_states get_current_display_state();

		static void update_rotation();

		// some additional drawing routines
		void fill_arc(uint8_t canvasid, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);

		const uint16_t width = 240;
		const uint16_t height = 280;
		const uint8_t center_x = 120;
		const uint8_t center_y = 140;

	private:
		tw_face *current_face = nullptr;

		bool showing_loading_icon = false;
		uint16_t loading_screen_rotation = 0;

		const uint8_t backlight_settings_vbus[3] = {200, 150, 100};
		const uint8_t backlight_settings_vbat[3] = {200, 100, 50};
		unsigned long next_loading_rotation = 0;

		// Screen Brightness levels
		uint8_t last_backlight = 99;
		uint8_t backlight_level = 0;
		uint8_t backlight_current_val = 0;
		uint8_t backlight_target_val = 0;

		display_states current_display_state = NORMAL;
};

extern TFT_eSPI tft;
extern TFT_eSprite canvas[];
extern Display display;
