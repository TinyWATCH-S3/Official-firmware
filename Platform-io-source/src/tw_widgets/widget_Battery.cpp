#include "tw_widgets/widget_Battery.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"
#include "peripherals/battery.h"
#include "tinywatch.h"

extern Battery battery;

void WidgetBattery::draw(uint canvasid, uint8_t style_hint)
{
	float bat_perc = (int)battery.get_percent(false);

	canvas[canvasid].setFreeFont(RobotoMono_Light[(bat_perc < 100) ? 8 : 6]);

	uint32_t col_back = 0;
	uint32_t col_front = 0;

	if (bat_perc > 80)
	{
		col_back = col_high[0];
		col_front = col_high[1];
	}
	else if (bat_perc >= 60)
	{
		col_back = col_med[0];
		col_front = col_med[1];
	}
	else
	{
		col_back = col_low[0];
		col_front = col_low[1];
	}

	uint16_t arc_length = (360.0 * (bat_perc / 100.0));

	if (bat_perc < 100)
	{
		canvas[canvasid].drawSmoothArc(pos_x, pos_y + 23, 23, 19, 0, 360, col_back, 0);
	}

	if (arc_length <= 180)
	{
		canvas[canvasid].drawSmoothArc(pos_x, pos_y + 23, 23, 19, 180, 180 + arc_length, col_front, 0);
	}
	else
	{
		canvas[canvasid].drawSmoothArc(pos_x, pos_y + 23, 23, 19, 180, 360, col_front, 0);
		canvas[canvasid].drawSmoothArc(pos_x, pos_y + 23, 23, 19, 0, arc_length - 180, col_front, 0);
	}

	if (tinywatch.vbus_present() && bat_perc < 98)
		canvas[canvasid].setTextColor(col_text[1]);
	else
		canvas[canvasid].setTextColor(col_text[0]);
	canvas[canvasid].drawString(String((int)bat_perc), pos_x, pos_y + 21);
}

bool WidgetBattery::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (bounds_check(touch_pos_x, touch_pos_y))
	{
		info_println("Battery stats? Not sure we can track any");
		return true;
	}

	return false;
}

bool WidgetBattery::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool WidgetBattery::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

// tft.setTextColor(ST77XX_YELLOW);
// tft.setCursor(15, 15);
// tft.print("BAT ");
// tft.print(battery.get_voltage(false), 2);
// tft.print("V ");
// tft.print(battery.get_percent(false), 1);
// tft.print("%");
