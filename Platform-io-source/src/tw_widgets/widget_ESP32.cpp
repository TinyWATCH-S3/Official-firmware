#include "tw_widgets/widget_ESP32.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"

void WidgetESP32::draw(uint canvasid, uint8_t style_hint)
{

	if (millis() - next_stat_cycle > 5000)
	{
		next_stat_cycle = millis();
	}

	canvas[canvasid].setFreeFont(RobotoMono_Light[8]);
	canvas[canvasid].setTextColor(RGB(0xff, 0x33, 0x33), 0);
	canvas[canvasid].drawString(String(getCpuFrequencyMhz()) + "MHz", pos_x, pos_y + 10);
}

bool WidgetESP32::process_touch(touch_event_t touch_event) { return false; }