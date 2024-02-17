#include "tw_widgets/widget_Message.h"

void WidgetMessage::draw(uint canvasid, uint8_t style_hint)
{
	canvas[canvasid].fillRoundRect(pos_x, pos_y, width, height, 3, RGB(0x44, 0x44, 0x44));
	canvas[canvasid].setTextColor(RGB(0xAA, 0xAA, 0xAA));
	canvas[canvasid].setTextDatum(4); // Middle, Center
	canvas[canvasid].drawString("MESSAGE", pos_x + (width / 2), pos_y + (height / 2));
}

bool WidgetMessage::process_touch(touch_event_t touch_event) { return false; }
