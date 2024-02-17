#pragma once

#include "display.h"
#include <map>

class tw_app
{
	public:
		tw_app() {}
		~tw_app() {}

		void add(String _name, uint16_t _update_period, uint32_t req_cpu_speed);
		void add(String _name, uint _update_period);
		bool click_icon(uint16_t touch_pos_x, uint16_t touch_pos_y);
		void set_canvas(uint _canvasid);
		void close(bool save);

		void get_icon_pos(uint16_t &_icon_x, uint16_t &_icon_y, uint16_t &_icon_width, uint16_t &_icon_height);

		// Virtual
		virtual void pre_start() = 0;
		virtual void draw(bool force) = 0;
		virtual void draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint) = 0;
		virtual bool click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool swipe(uint16_t touch_pos_x, uint16_t touch_pos_y, int8_t swipe_dir, int16_t dist_x, int16_t dist_y) { return false; }

		String name = "";

	protected:
		tw_face *owner;

		bool is_setup = false;
		uint8_t canvasid = 0;
		uint8_t required_cpu_speed = 80;
		uint16_t update_period = 0;
		unsigned long next_update = 0;
		int16_t _x = 0;
		int16_t _y = 0;

		// Icon stuff
		int8_t icon_width = 64;
		int8_t icon_height = 64;
		int16_t icon_x = 0;
		int16_t icon_y = 0;

		bool is_icon_cached = false;
		TFT_eSprite icon_sprite = TFT_eSprite(&tft);
};
