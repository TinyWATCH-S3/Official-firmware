#include "tw_widgets/widget_ESP32.h"
#include "fonts/RobotoMono_Light_All.h"
#include "bitmaps/bitmaps_general.h"

void WidgetESP32::draw(uint canvasid)
{

	if (millis() - next_stat_cycle > 5000)
	{
		next_stat_cycle = millis();
	}

	canvas[canvasid].setFreeFont(RobotoMono_Light[8]);
	canvas[canvasid].setTextColor(RGB(0xff, 0x33, 0x33), 0);
	canvas[canvasid].drawString(String(getCpuFrequencyMhz())+"MHz", pos_x, pos_y+10);
}

bool WidgetESP32::click(uint16_t click_pos_x, uint16_t click_pos_y)
{
	return false;
}

bool WidgetESP32::click_double(uint16_t click_pos_x, uint16_t click_pos_y)
{
	return false;
}

bool WidgetESP32::click_long(uint16_t click_pos_x, uint16_t click_pos_y)
{
	return false;
}
