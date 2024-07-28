#include "tw_widgets/widget_ActivityRing.h"
#include "activity.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"
#include "peripherals/imu.h"
#include "peripherals/rtc.h"
#include "settings/settings.h"
#include "tinywatch.h"

void WidgetActivityRing::draw(uint canvasid)
{
	if (settings.config.imu_process_steps)
	{
		uint16_t day, month, year;
		rtc.get_step_date(day, month, year);
		uint32_t steps = imu.get_steps(day, month, year);
		uint8_t movement = imu.get_movement_activity_id();

		if (!activity.loaded || day == 0)
		{
			canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, 0, 360, themes.current().col_background_dull, 0);
			canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, arc_pos, arc_pos + 60, themes.current().col_primary, 0);
			canvas[canvasid].setTextColor(themes.current().col_background_bright);
			canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
			canvas[canvasid].drawString("STEPS", pos_x, pos_y + radius);
			arc_pos += 60;
			return;
		}

		canvas[canvasid].setFreeFont(RobotoMono_Light[(steps < 100) ? 8 : 6]);

		uint32_t ring_color = (movement > 0) ? themes.current().col_primary : themes.current().col_background_bright;

		canvas[canvasid].drawSmoothArc(pos_x, pos_y + radius, radius, radius - thickness, 0, 360, ring_color, themes.current().col_background_dull);

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

bool WidgetActivityRing::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		if (bounds_check(touch_event.x, touch_event.y))
		{
			info_println("Show activity face");
			return true;
		}
	}

	return false;
}
