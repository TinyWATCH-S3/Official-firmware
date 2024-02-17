/**
 * @file tw_control.cpp
 * @details TW Controls are UI elements lik buttons, toggles, value sliders etc.
 * They can be placed on FACES and can also be bound to user settings using the SettingsOption classes.
 *
 */
#include "tw_controls/tw_control.h"
#include "utilities/logging.h"

void tw_control::create(String _name, String _option1, String _option2, uint _pos_x, uint _pos_y, uint _width, uint _height)
{
	name = _name;
	value = "";
	option1 = _option1;
	option2 = _option2;
	pos_x = _pos_x;
	pos_y = _pos_y;
	width = _width;
	height = _height;

	// my_sprite.setSwapBytes(true);
	// my_sprite.createSprite(width, height);
}

void tw_control::create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height)
{
	name = _name;
	value = "";
	option1 = "";
	option2 = "";
	pos_x = _pos_x;
	pos_y = _pos_y;
	width = _width;
	height = _height;
}

void tw_control::create(String _name, uint _pos_x, uint _pos_y)
{
	name = _name;
	value = "";
	option1 = "";
	option2 = "";
	pos_x = _pos_x;
	pos_y = _pos_y;
	width = 0;
	height = 0;
}

void tw_control::set_callback(CallbackFunction callback) { callbackFunction = callback; }

void tw_control::set_values(String _option1, String _option2)
{
	option1 = _option1;
	option2 = _option2;
}

void tw_control::set_parent(tw_face *_parent) { parent = _parent; }

bool tw_control::bounds_check(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	int min_x, min_y = 0;
	if (pos_x - padding_x > 0)
		min_x = pos_x - padding_x;
	if ((int)pos_y - padding_y > 0)
		min_y = (int)pos_y - padding_y;

	bool in_x = (touch_pos_x >= min_x && touch_pos_x <= pos_x + width + padding_x);
	bool in_y = (touch_pos_y >= min_y + offset_y && touch_pos_y <= pos_y + height + padding_y + offset_y);

	// info_println(String(in_x)+" - "+String(in_y)+" ("+String(touch_pos_x)+","+String(touch_pos_y)+") in ("+String(min_x)+","+String(min_y)+")-("+String(pos_x + width + padding_x)+","+String(pos_y + height + padding_y)+")");

	return (in_x && in_y);
}

int16_t tw_control::get_y_min() { return pos_y; }

int16_t tw_control::get_y_max() { return pos_y + height + padding_y; }

void tw_control::draw_scroll(uint canvasid, uint16_t off_x, uint16_t off_y)
{
	offset_x = off_x;
	offset_y = off_y;
	draw(canvasid);
}

void tw_control::draw(uint canvasid, uint _pos_x, uint _pos_y)
{
	pos_x = _pos_x;
	pos_y = _pos_y;
	draw(canvasid);
}

void tw_control::set_scrollable(bool scroll_x, bool scroll_y)
{
	can_scroll_x = scroll_x;
	can_scroll_y = scroll_y;
}

bool tw_control::can_drag(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (bounds_check(touch_pos_x, touch_pos_y))
	{
		return (can_scroll_x || can_scroll_y);
	}
	return false;
}

uint16_t tw_control::get_height_stacked() { return (height + padding_y * 2); }

uint16_t tw_control::get_height() { return (height); }
