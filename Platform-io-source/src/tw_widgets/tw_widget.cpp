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

void tw_widget::set_parent(tw_face *_parent)
{
	parent = _parent;
}

bool tw_widget::bounds_check(uint click_pos_x, uint click_pos_y)
{
	bool in_x = (click_pos_x >= bounds_x[0] && click_pos_x <= bounds_x[1]);
	bool in_y = (click_pos_y >= bounds_y[0] && click_pos_y <= bounds_y[1]);

	return (in_x && in_y);
}

uint tw_widget::get_height()
{
	return (height + padding_y);
}

void tw_widget::draw(uint canvasid, uint _pos_x, uint _pos_y, uint8_t style_hint)
{
	pos_x = _pos_x;
	pos_y = _pos_y;
	draw(canvasid, style_hint);
}

