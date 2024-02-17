#include "tw_widgets/widget_ActivityRing.h"
#include "activity.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"
#include "peripherals/imu.h"
#include "peripherals/rtc.h"
#include "settings/settings.h"
#include "tinywatch.h"

void WidgetActivityRing::draw(uint canvasid, uint8_t style_hint)
{
	if (settings.config.imu_process_steps)
	{
		uint16_t day, month, year;
		rtc.get_step_date(day, month, year);
		uint32_t steps = imu.get_steps(day, month, year);
		uint8_t movement = imu.get_movement_activity_id();

		if (!activity.loaded || day == 0)
		{
			canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, 0, 360, col_rings[0], 0);
			canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, arc_pos, arc_pos + 60, col_rings[1], 0);
			canvas[canvasid].setTextColor(RGB(0x88, 0x88, 0x88));
			canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
			canvas[canvasid].drawString("STEPS", pos_x, pos_y + radius);
			arc_pos += 60;
			return;
		}

		canvas[canvasid].setFreeFont(RobotoMono_Light[(steps < 100) ? 8 : 6]);

		uint32_t ring_color = col_rings[movement];

		if (style_hint == 0)
		{
			canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, 0, 360, col_rings[movement], 0);
		}
		else
		{
			canvas[canvasid].drawSmoothRoundRect(pos_x - radius, pos_y, thickness, thickness - 2, radius * 2, radius * 2, col_rings[movement], 0);
		}
		canvas[canvasid].setTextColor(RGB(0xff, 0xff, 0xff));
		canvas[canvasid].drawNumber(steps, pos_x + 1, pos_y + radius - 1);
	}
	else
	{
		canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, 0, 360, col_rings[0], 0);
		canvas[canvasid].setFreeFont(RobotoMono_Light[8]);
		canvas[canvasid].setTextColor(RGB(0x99, 0x99, 0x99));
		canvas[canvasid].drawString("-", pos_x, pos_y + radius);
	}
}

bool WidgetActivityRing::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (bounds_check(touch_pos_x, touch_pos_y))
	{
		info_println("Show activity face");
		return true;
	}

	return false;
}

bool WidgetActivityRing::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool WidgetActivityRing::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
