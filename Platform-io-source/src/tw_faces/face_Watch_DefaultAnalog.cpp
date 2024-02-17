#include "tw_faces/face_Watch_DefaultAnalog.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "fonts/Roboto_Regular18.h"
#include "peripherals/battery.h"
#include "peripherals/imu.h"
#include "settings/settings.h"
#include "tinywatch.h"

void FaceWatch_DefaultAnalog::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceWatch_DefaultAnalog::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].fillSprite(RGB(0x00, 0x0, 0x0));
			canvas[canvasid].setTextColor(TFT_WHITE);

			canvas[canvasid].fillCircle(center_x, center_y, face_radius, RGB(0x45, 0x45, 0x45));
			canvas[canvasid].setTextDatum(0); // Middle, Center
			canvas[canvasid].setFreeFont(RobotoMono_Regular[16]);
			canvas[canvasid].setTextColor(0, RGB(0x45, 0x45, 0x45));
			canvas[canvasid].drawString(rtc.get_day_of_week(), center_x + 10, center_y - 43);
			canvas[canvasid].drawString(rtc.get_month_date(), center_x + 10, center_y - 21);
			canvas[canvasid].setFreeFont(&Roboto_Regular18);
			canvas[canvasid].setTextColor(RGB(0x00, 0x65, 0xff), RGB(0x45, 0x45, 0x45));
			canvas[canvasid].drawString(rtc.get_time_string(true, settings.config.time_24hour), center_x + 10, center_y + 8);

			if (!cachedTrig)
			{
				cachedTrig = true;
				setup_trig();
			}

			canvas[canvasid].setFreeFont(RobotoMono_Light[5]);
			canvas[canvasid].setTextDatum(4);

			int hours = rtc.get_hours();

			if (hours > 12)
				hours -= 12;

			canvas[canvasid].drawWideLine(center_x, center_y, pos_hours[hours][0], pos_hours[hours][1], 3.0f, 0);

			int mins = rtc.get_mins();

			canvas[canvasid].drawWideLine(center_x, center_y, pos_mins[mins][0], pos_mins[mins][1], 3.0f, 0);

			int secs = rtc.get_seconds();
			canvas[canvasid].fillCircle(pos_secs[secs][0], pos_secs[secs][1], 5, RGB(0x00, 0x65, 0xff));

			canvas[canvasid].fillCircle(center_x, center_y, 6, 0);

			draw_children(false, 0);
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatch_DefaultAnalog::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceWatch_DefaultAnalog::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	display.cycle_clock_face();
	is_dragging = false;
	// draw(true);
	return true;
}

bool FaceWatch_DefaultAnalog::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	// info_println("LOOOONG!");
	// TODO: Add display of watch specific settings here when the user long presses
	return true;
}

// Pre calculate the trig required for the analog clock face as calculating trig on the fly is expensive on an MCU
void FaceWatch_DefaultAnalog::setup_trig()
{
	int b = 0;
	float i = -M_PI / 2.0;

	for (int tick = 0; tick < 60; tick++)
	{
		if (tick % 5 == 0)
		{
			pos_hours[b][0] = ((face_radius - 60) * cos(i)) + center_x;
			pos_hours[b][1] = ((face_radius - 60) * sin(i)) + center_y;
			b++;
		}

		pos_mins[tick][0] = ((face_radius - 20) * cos(i)) + center_x;
		pos_mins[tick][1] = ((face_radius - 20) * sin(i)) + center_y;
		pos_secs[tick][0] = ((face_radius - 7) * cos(i)) + center_x;
		pos_secs[tick][1] = ((face_radius - 7) * sin(i)) + center_y;

		i += M_PI * 2.0 / 60.0;
	}
}

FaceWatch_DefaultAnalog face_watch_default_analog;