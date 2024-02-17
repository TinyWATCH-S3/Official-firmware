#include "tw_controls/control_Label.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlLabel::draw(uint8_t canvasid)
{
	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	canvas[canvasid].setFreeFont(RobotoMono_Light[7]);
	canvas[canvasid].setTextColor(TFT_WHITE);
	canvas[canvasid].setTextDatum(4); // Middle, Center
	canvas[canvasid].drawString(name, adjusted_pos_x + (width / 2) - 1, adjusted_pos_y + (height / 2) - 1);
}

bool ControlLabel::drag(int16_t drag_x, int16_t drag_y) { return false; }

void ControlLabel::drag_end() {}

bool ControlLabel::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlLabel::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlLabel::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
