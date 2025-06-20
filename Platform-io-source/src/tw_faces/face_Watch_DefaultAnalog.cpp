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

#include "tw_faces/face_Notifications.h"

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

			canvas[canvasid].fillSprite(themes.current().col_background_dull);
			canvas[canvasid].setTextColor(TFT_WHITE);

			canvas[canvasid].fillCircle(center_x, center_y, face_radius, themes.current().col_background_bright);
			canvas[canvasid].setTextDatum(TL_DATUM); // Top, Left
			canvas[canvasid].setFreeFont(RobotoMono_Regular[16]);
			canvas[canvasid].setTextColor(themes.current().col_secondary, themes.current().col_background_bright);
			canvas[canvasid].drawString(rtc.get_day_of_week(), center_x + 10, center_y - 43);
			canvas[canvasid].drawString(rtc.get_month_date(), center_x + 10, center_y - 21);
			canvas[canvasid].setFreeFont(&Roboto_Regular18);
			canvas[canvasid].setTextColor(themes.current().col_primary, themes.current().col_background_bright);
			canvas[canvasid].drawString(rtc.get_time_string(true, settings.config.time_24hour), center_x + 10, center_y + 8);

			if (!cachedTrig)
			{
				cachedTrig = true;
				setup_trig();
			}

			canvas[canvasid].setFreeFont(RobotoMono_Light[5]);
			canvas[canvasid].setTextDatum(4);

			draw_children(false, 0);

			// revised code for calculating hours + extra % for mins for hour hand
			int hours = rtc.get_hours();
			int mins = rtc.get_mins();
			int secs = rtc.get_seconds();

			if (hours > 12)
				hours -= 12;

			if (cached_mins < 0 || cached_mins != mins)
			{
				cached_mins = mins;

				float mins_perc = (float)mins/60.0f;
				int hour_mins = hours * 5 + (int)(5.0f * mins_perc); 

				// Serial.printf("hours: %d, hour_mins: %d, mins_perc: %0.2f\n", hours, hour_mins, mins_perc);

				// recalc the hour position based on the hour plus min spercentage to 60.
				live_recalc_xy(hour_mins, face_radius - 7, face_radius - 60, &h_x, &h_y);

				// recalc the mins position.
				live_recalc_xy(mins, face_radius - 7, face_radius - 20, &m_x, &m_y);
			}
			// Draw hours hand
			canvas[canvasid].drawWideLine(center_x, center_y, h_x, h_y, 3.0f, 0);
			// Draw mins hands
			canvas[canvasid].drawWideLine(center_x, center_y, m_x, m_y, 3.0f, 0);

			// Blank out seconds ticks
			for (int i = 0; i < 59; i += 5)
				canvas[canvasid].fillCircle(pos_secs[i][0], pos_secs[i][1], 2, themes.current().col_background_dull);

			// Draw seconds
			canvas[canvasid].fillCircle(pos_secs[secs][0], pos_secs[secs][1], 5, themes.current().col_primary);

			// Draw seconds ghost ticks
			for (int i = 0; i < 5; i++)
			{
				secs--;
				if (secs < 0)
					secs = 59;
				canvas[canvasid].fillCircle(pos_secs[secs][0], pos_secs[secs][1], 2, themes.current().col_secondary);
			}

			canvas[canvasid].fillCircle(center_x, center_y, 6, 0);

			draw_navigation(canvasid);
		}

		update_screen();
	}
}

bool FaceWatch_DefaultAnalog::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_DOUBLE)
	{
		display.cycle_clock_face();
		is_dragging = false;
		// draw(true);
		return true;
	}
	else if (touch_event.type == TOUCH_LONG)
	{
		// TODO: Add display of watch specific settings here when the user long presses
		info_println("hello current face long press");
		return true;
	}

	return false;
}

// Pre calculate the trig required for the analog clock face as calculating trig on the fly is expensive on an MCU
void FaceWatch_DefaultAnalog::setup_trig()
{
	int b = 0;
	float i = -M_PI / 2.0;

	// Force first draw
	cached_mins = -1;

	for (int tick = 0; tick < 60; tick++)
	{
		// if (tick % 5 == 0)
		// {
		// 	pos_hours[b][0] = ((face_radius - 60) * cos(i)) + center_x;
		// 	pos_hours[b][1] = ((face_radius - 60) * sin(i)) + center_y;
		// 	b++;
		// }

		// pos_mins[tick][0] = ((face_radius - 20) * cos(i)) + center_x;
		// pos_mins[tick][1] = ((face_radius - 20) * sin(i)) + center_y;
		pos_secs[tick][0] = ((face_radius - 7) * cos(i)) + center_x;
		pos_secs[tick][1] = ((face_radius - 7) * sin(i)) + center_y;

		i += M_PI * 2.0 / 60.0;
	}
}

void FaceWatch_DefaultAnalog::live_recalc_xy(uint8_t mins, uint8_t old_len, uint8_t new_len, int *new_x, int *new_y)
{
    // Calculate delta from center
    int dx = pos_secs[mins][0] - center_x;
    int dy = pos_secs[mins][1] - center_y;

    // Scale vector to new length, using integer math
    // (cast to int for robustness with negative values)
    *new_x = center_x + ((int32_t)dx * new_len) / old_len;
    *new_y = center_y + ((int32_t)dy * new_len) / old_len;
}

FaceWatch_DefaultAnalog face_watch_default_analog;