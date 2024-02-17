/**
 * @file tw_widget.cpp
 * @details TW Widgets are self-contained interactive apps designed for reuse across different watch faces. They can perform logic, connectivity tasks, and display specific information.

An example of a widget could be a weather app that connects to OpenWeather, downloads information, and displays it on the watch face.

Clicking on the widget could switch between showing the current temperature, humidity, chance of rain, etc.

A `Widget` differs from a `Control` in that a control is a single-use interactive item bound to a setting or callback on a specific face. You can place the same widget on multiple faces, each in a different location.
 *
 */
#include "tw_widgets/tw_widget.h"
#include "display.h"

extern TFT_eSPI tft;
extern TFT_eSprite canvas[];

class tw_face;

void tw_widget::create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height, uint _update_period)
{
	name = _name;
	update_period = _update_period;
	pos_x = _pos_x;
	pos_y = _pos_y;
	width = _width;
	height = _height;
	bounds_x[0] = constrain(pos_x - (int16_t)padding_x, 0, display.width);
	bounds_x[1] = constrain(pos_x + width + padding_x, 0, display.width);
	bounds_y[0] = constrain(pos_y - (int16_t)padding_y, 0, display.height);
	bounds_y[1] = constrain(pos_y + height + padding_y, 0, display.height);
}

void tw_widget::set_parent(tw_face *_parent) { parent = _parent; }

bool tw_widget::bounds_check(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	bool in_x = (touch_pos_x >= bounds_x[0] && touch_pos_x <= bounds_x[1]);
	bool in_y = (touch_pos_y >= bounds_y[0] && touch_pos_y <= bounds_y[1]);

	return (in_x && in_y);
}

uint tw_widget::get_height() { return (height + padding_y); }

void tw_widget::draw(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint)
{
	pos_x = _pos_x;
	pos_y = _pos_y;
	draw(canvasid, style_hint);
}
