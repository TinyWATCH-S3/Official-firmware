#pragma once

#include "display.h"

class tw_face;

class tw_widget
{
	public:
		tw_widget() {}
		~tw_widget() {}

		void create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height, uint _update_period);
		void set_parent(tw_face *_parent);
		bool bounds_check(uint16_t touch_pos_x, uint16_t touch_pos_y);
		uint get_height();
		void draw(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint);

		virtual void draw(uint canvasid, uint8_t style_hint) = 0;
		virtual bool click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

	protected:
		tw_face *parent = nullptr;
		String name = "";
		uint16_t update_period = 0;
		unsigned long next_update = 0;
		uint width = 0;
		uint height = 0;
		int16_t pos_x = 0;
		int16_t pos_y = 0;
		uint padding_x = 10;
		uint padding_y = 20;
		uint16_t bounds_x[2] = {0};
		uint16_t bounds_y[2] = {0};
};
