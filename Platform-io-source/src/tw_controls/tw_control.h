#pragma once

#include "display.h"

extern TFT_eSPI tft;
extern TFT_eSprite canvas[];

class tw_face;

class tw_control
{
	public:
		using CallbackFunction = void (*)();

		void create(String _name, String _option1, String _option2, uint _pos_x, uint _pos_y, uint _width, uint _height);
		void create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height);
		void create(String _name, uint _pos_x, uint _pos_y);
		void set_callback(CallbackFunction callback);
		void set_values(String _option1, String _option2);
		void set_parent(tw_face *_parent);
		bool bounds_check(uint16_t touch_pos_x, uint16_t touch_pos_y);
		uint16_t get_height();
		uint16_t get_height_stacked();

		int16_t get_y_min();
		int16_t get_y_max();

		void draw(uint canvasid, uint _pos_x, uint _pos_y);
		void draw_scroll(uint canvasid, uint16_t offset_x, uint16_t offset_y);

		void set_scrollable(bool scroll_x, bool scroll_y);
		bool can_drag(uint16_t touch_pos_x, uint16_t touch_pos_y);

		// Virtual functions
		virtual void draw(uint8_t canvasid) = 0;
		virtual bool drag(int16_t drag_x, int16_t drag_y) = 0;
		virtual void drag_end() = 0;
		virtual bool click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		virtual bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
		String name = "";

	protected:
		tw_face *parent = nullptr;

		// TFT_eSprite my_sprite = TFT_eSprite(&tft);

		CallbackFunction callbackFunction;

		String value = "";
		String option1 = "";
		String option2 = "";

		uint8_t canvasid = 0;
		uint width = 0;
		uint height = 0;
		uint pos_x = 0;
		uint pos_y = 0;
		uint padding_x = 20;
		uint padding_y = 20;

		bool requires_redraw = true;

		bool can_scroll_x = false;
		bool can_scroll_y = false;
		uint scroll_x = 0;
		uint scroll_y = 0;

		int16_t offset_x = 0;
		int16_t offset_y = 0;
		int16_t adjusted_pos_x = 0;
		int16_t adjusted_pos_y = 0;

	private:
		//
};
