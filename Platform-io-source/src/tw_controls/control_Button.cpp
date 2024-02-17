#include "tw_controls/control_Button.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlButton::draw(uint8_t canvasid)
{
	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	canvas[canvasid].fillRoundRect(adjusted_pos_x, adjusted_pos_y, width, height, 4, RGB(0x44, 0x44, 0x44));
	canvas[canvasid].drawRoundRect(adjusted_pos_x, adjusted_pos_y, width, height, 4, RGB(0x66, 0x66, 0x66));
	canvas[canvasid].setFreeFont(RobotoMono_Light[7]);
	canvas[canvasid].setTextColor(TFT_WHITE);
	canvas[canvasid].setTextDatum(4); // Middle, Center
	canvas[canvasid].drawString(name, adjusted_pos_x + (width / 2) - 1, adjusted_pos_y + (height / 2) - 1);
}

bool ControlButton::drag(int16_t drag_x, int16_t drag_y) { return false; }

void ControlButton::drag_end() {}

bool ControlButton::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (bounds_check(touch_pos_x, touch_pos_y))
	{
		canvas[canvasid].drawRoundRect(pos_x, pos_y, width, height, 4, RGB(0xff, 0xff, 0xff));
		canvas[canvasid].pushSprite(0, 0);
		canvas[canvasid].drawRoundRect(pos_x, pos_y, width, height, 4, RGB(0x66, 0x66, 0x66));

		if (callbackFunction != nullptr)
			callbackFunction();

		return true;
	}

	return false;
}

bool ControlButton::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlButton::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
