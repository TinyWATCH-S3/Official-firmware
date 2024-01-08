#include "tw_faces/face_Watch_CustomWindows.h"
#include "bitmaps/bitmaps_general.h"
#include "peripherals/battery.h"
#include "peripherals/imu.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

void FaceWatch_CustomWindows::setup()
{
	if (!is_setup)
	{
		is_setup = true;
	}
}

// Pre calculate the trig required for the analog clock face as calculating trig on the fly is expensive on an MCU
void FaceWatch_CustomWindows::setup_trig()
{
	int b = 0;
	float i = -M_PI / 2.0;

	for (int tick = 0; tick < 60; tick++)
	{
		pos_hours[tick][0] = ((face_radius - 50) * cos(i)) + center_x;
		pos_hours[tick][1] = ((face_radius - 50) * sin(i)) + center_y;

		pos_mins[tick][0] = ((face_radius - 30) * cos(i)) + center_x;
		pos_mins[tick][1] = ((face_radius - 30) * sin(i)) + center_y;

		pos_secs[tick][0] = ((face_radius - 20) * cos(i)) + center_x;
		pos_secs[tick][1] = ((face_radius - 20) * sin(i)) + center_y;

		// Generate offsets for fancy hr/min hand
		int tick_lo = tick + 15;
		if (tick >= 45)
			tick_lo = tick - 45;

		int tick_ro = tick + 45;
		if (tick >= 15)
			tick_ro = tick - 15;

		int tick_rear = tick + 30;
		if (tick >= 30)
			tick_rear = tick - 30;

		pos_lo[tick_lo][0] = (4 * cos(i)) + center_x;
		pos_lo[tick_lo][1] = (4 * sin(i)) + center_y;
		pos_ro[tick_ro][0] = (4 * cos(i)) + center_x;
		pos_ro[tick_ro][1] = (4 * sin(i)) + center_y;

		pos_mins_rlo[tick_rear][0] = (16 * cos(i)) + center_x;
		pos_mins_rlo[tick_rear][1] = (16 * sin(i)) + center_y;
		pos_mins_rro[tick_rear][0] = (16 * cos(i)) + center_x;
		pos_mins_rro[tick_rear][1] = (16 * sin(i)) + center_y;

		pos_hours_rlo[tick_rear][0] = (10 * cos(i)) + center_x;
		pos_hours_rlo[tick_rear][1] = (10 * sin(i)) + center_y;
		pos_hours_rro[tick_rear][0] = (10 * cos(i)) + center_x;
		pos_hours_rro[tick_rear][1] = (10 * sin(i)) + center_y;

		i += M_PI * 2.0 / 60.0;
	}
}

void FaceWatch_CustomWindows::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			if (!cachedTrig)
			{
				cachedTrig = true;
				setup_trig();
			}

			uint8_t secs = rtc.get_seconds();

			// Only fetch the mins, hrs, and date once a minute.
			// Also do it for first run (year cant be 0, or 2000)
			if (secs == 0 || year == 0 || year == 2000)
			{
				mins = rtc.get_mins();
				hours = rtc.get_hours();
				day = rtc.get_day();
				month = rtc.get_month();
				year = rtc.get_year();

				// Offset the Date for single/multiple digits
				if (day > 9)
					day_offset = 0;
			}

			// Clock Face Starts Here
			WindowsClockSettings winclock;

			// Blank the display
			canvas[canvasid].fillSprite(winclock.color_displaybkg);
			canvas[canvasid].setTextColor(TFT_WHITE);

			uint16_t xo = (display.width - winclock.width) / 2;
			uint16_t yo = (display.height - winclock.height) / 2;

			//  x, y, w , h
			// Main Window
			canvas[canvasid].fillRect(0 + xo, 0 + yo, winclock.width, winclock.height, winclock.color_background);		   // Clock window background
			canvas[canvasid].drawRect(0 + xo, 0 + yo, winclock.width, winclock.height, winclock.color_borderedge);		   // Border outer edge
			canvas[canvasid].drawRect(3 + xo, 3 + yo, winclock.width - 6, winclock.height - 6, winclock.color_borderedge); // Border inner edge

			// Title Box
			canvas[canvasid].fillRect(3 + xo, 3 + yo, winclock.width - 6, 20, winclock.color_titlebar);	  // Border title edge
			canvas[canvasid].drawRect(3 + xo, 3 + yo, winclock.width - 6, 20, winclock.color_borderedge); // Border title edge

			// Control Box
			canvas[canvasid].fillRect(3 + xo, 3 + yo, 20, 20, winclock.color_background);	   // Control Box Background
			canvas[canvasid].drawRect(3 + xo, 3 + yo, 20, 20, winclock.color_borderedge);	   // Control Box Border
			canvas[canvasid].fillRect(7 + xo, 12 + yo, 13, 3, winclock.color_buttonshadow);	   // Control Box Minus Shadow
			canvas[canvasid].fillRect(6 + xo, 11 + yo, 13, 3, winclock.color_buttonhighlight); // Control Box Minus Fill
			canvas[canvasid].drawRect(6 + xo, 11 + yo, 13, 3, winclock.color_borderedge);	   // Control Box Minus Border

			// Maximise Box
			canvas[canvasid].fillRect((xo + winclock.width) - 23, 3 + yo, 20, 20, winclock.color_background); // Control Box Background
			canvas[canvasid].drawRect((xo + winclock.width) - 23, 3 + yo, 20, 20, winclock.color_borderedge); // Control Box Border

			canvas[canvasid].drawLine((xo + winclock.width) - 13, 11 + yo, (xo + winclock.width) - 13, 11 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 14, 12 + yo, (xo + winclock.width) - 12, 12 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 15, 13 + yo, (xo + winclock.width) - 11, 13 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 16, 14 + yo, (xo + winclock.width) - 10, 14 + yo, winclock.color_menutext);

			// Minimise Box
			canvas[canvasid].fillRect((xo + winclock.width) - 42, 3 + yo, 20, 20, winclock.color_background); // Control Box Background
			canvas[canvasid].drawRect((xo + winclock.width) - 42, 3 + yo, 20, 20, winclock.color_borderedge); // Control Box Border

			canvas[canvasid].drawLine((xo + winclock.width) - 32, 14 + yo, (xo + winclock.width) - 32, 14 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 33, 13 + yo, (xo + winclock.width) - 31, 13 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 34, 12 + yo, (xo + winclock.width) - 30, 12 + yo, winclock.color_menutext);
			canvas[canvasid].drawLine((xo + winclock.width) - 35, 11 + yo, (xo + winclock.width) - 29, 11 + yo, winclock.color_menutext);

			// Menu Box
			canvas[canvasid].fillRect(3 + xo, 23 + yo, winclock.width - 6, 20, winclock.color_menubar);	   // Border title edge
			canvas[canvasid].drawRect(3 + xo, 23 + yo, winclock.width - 6, 20, winclock.color_borderedge); // Border title edge

			// Title Bar Date
			canvas[canvasid].setTextDatum(MC_DATUM);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[7]);
			canvas[canvasid].setTextColor(winclock.color_titletext, winclock.color_titlebar);

			String toolbar_date = String(day) + "/" + String(month) + "/" + String(year);
			if (settings.config.time_dateformat == true) {
				toolbar_date = String(month) + "/" + String(day) + "/" + String(year);
			}

			canvas[canvasid].drawString(toolbar_date, 85 + xo, 12 + yo);

			// Menu Bar Text
			canvas[canvasid].setTextDatum(ML_DATUM);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[7]);
			canvas[canvasid].setTextColor(winclock.color_menutext, winclock.color_menubar);
			canvas[canvasid].drawString("Settings", 10 + xo, 32 + yo);
			canvas[canvasid].drawLine(10 + xo, 39 + yo, 18 + xo, 39 + yo, winclock.color_menutext);

			// Seconds Dots
			for (int x = 0; x < 60; x++)
			{
				canvas[canvasid].fillRect(pos_secs[x][0] - 1, pos_secs[x][1] - 1, 2, 2, winclock.color_buttonshadow);
				canvas[canvasid].fillRect(pos_secs[x][0], pos_secs[x][1], 2, 2, winclock.color_buttonhighlight);
			}

			// Hours Dots
			for (int x = 0; x < 12; x++)
			{
				canvas[canvasid].fillRect(pos_secs[x * 5][0] - 1, pos_secs[x * 5][1] - 1, 5, 5, winclock.color_buttonshadow);
				canvas[canvasid].fillRect(pos_secs[x * 5][0] - 2, pos_secs[x * 5][1] - 2, 5, 5, winclock.color_hourblock);
			}

			// Analog Clock Hands
			if (hours > 12)
				hours -= 12;

			// Move the hours hand with the precision of minutes
			int hr_index = (hours * 5) + (mins / 12);
			if (hr_index > 59)
				hr_index -= 60;

			if (mins > 59)
				mins -= 60;

			// Seconds Hand
			canvas[canvasid].drawWideLine(center_x, center_y, pos_secs[secs][0], pos_secs[secs][1], 1.0f, winclock.color_sechand);

			// Minutes Hand
			canvas[canvasid].drawWideLine(pos_lo[mins][0], pos_lo[mins][1], pos_mins[mins][0], pos_mins[mins][1], 1.0f, winclock.color_hrminhand);
			canvas[canvasid].drawWideLine(pos_ro[mins][0], pos_ro[mins][1], pos_mins[mins][0], pos_mins[mins][1], 1.0f, winclock.color_hrminhand);
		
			// Mins Hand Rear
			canvas[canvasid].drawWideLine(pos_lo[mins][0], pos_lo[mins][1], pos_mins_rlo[mins][0], pos_mins_rlo[mins][1], 1.0f, winclock.color_hrminhand);
			canvas[canvasid].drawWideLine(pos_ro[mins][0], pos_ro[mins][1], pos_mins_rro[mins][0], pos_mins_rro[mins][1], 1.0f, winclock.color_hrminhand);

			// Hours Hand
			canvas[canvasid].drawWideLine(pos_lo[hr_index][0], pos_lo[hr_index][1], pos_hours[hr_index][0], pos_hours[hr_index][1], 1.0f, winclock.color_hrminhand);
			canvas[canvasid].drawWideLine(pos_ro[hr_index][0], pos_ro[hr_index][1], pos_hours[hr_index][0], pos_hours[hr_index][1], 1.0f, winclock.color_hrminhand);
			// Hours Hand Rear
			canvas[canvasid].drawWideLine(pos_lo[hr_index][0], pos_lo[hr_index][1], pos_hours_rlo[hr_index][0], pos_hours_rlo[hr_index][1], 1.0f, winclock.color_hrminhand);
			canvas[canvasid].drawWideLine(pos_ro[hr_index][0], pos_ro[hr_index][1], pos_hours_rro[hr_index][0], pos_hours_rro[hr_index][1], 1.0f, winclock.color_hrminhand);

			// Clock Face Ends Here

			draw_children(false, 0);
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatch_CustomWindows::click(int16_t pos_x, int16_t pos_y)
{
	// Cycle through the colour pallette, after one full cycle, switch the style (square/circle)
	return false;
}

bool FaceWatch_CustomWindows::click_double(int16_t pos_x, int16_t pos_y)
{
	display.cycle_clock_face();

	is_dragging = false;
	// draw(true);
	return true;
}

bool FaceWatch_CustomWindows::click_long(int16_t pos_x, int16_t pos_y)
{
	// info_println("LOOOONG!");
	// TODO: Add display of watch specific settings here when the user long presses
	return true;
}

FaceWatch_CustomWindows face_watch_custom_windows;
