#include "tw_faces/face_Watch_CustomBinary.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "peripherals/battery.h"
#include "peripherals/imu.h"
#include "settings/settings.h"
#include "tinywatch.h"

void FaceWatch_CustomBinary::setup()
{
	if (!is_setup)
	{
		is_setup = true;
	}
}

void FaceWatch_CustomBinary::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			// Blank the display
			canvas[canvasid].fillSprite(RGB(0x00, 0x0, 0x0));
			canvas[canvasid].setTextColor(TFT_WHITE);

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

			uint8_t posmul = 32;
			uint8_t x_space = 5;
			uint8_t y_space = 5;
			uint8_t digit_xoffset = 14;
			uint8_t digit_yoffset = 16;

			int x_offset = (display.width - (6 * (posmul + x_space))) / 2;
			int y_offset = 176; // i cant for the life of me figure out how to fix this!

			// Set Colours
			int16_t on_color = on_colors[settings.config.custom_binary.binary_clockcolour];
			int16_t off_color = off_colors[settings.config.custom_binary.binary_clockcolour];
			int16_t bdr_color = RGB(0x0A, 0x0A, 0x0A);
			int16_t tim_color = RGB(0xff, 0xff, 0xff);
			int8_t clock_style = settings.config.custom_binary.binary_clockstyle;

			int box_positions[6] = {5, 4, 3, 2, 1, 0};												 // Start with seconds, then minutes, then hours
			int box_counts[6] = {4, 3, 4, 3, 4, 2};													 // Not all columns need 4 boxes
			int digit_num[6] = {secs % 10, secs / 10, mins % 10, mins / 10, hours % 10, hours / 10}; // this returns the individual digit for each column

			int half_posmul = posmul / 2;

			for (int digit = 0; digit < 6; ++digit)
			{
				int xOffset = x_offset + (posmul * box_positions[digit]) + (x_space * box_positions[digit]);
				for (int box_y = 0; box_y < box_counts[digit]; ++box_y)
				{
					// if (is_onoff(, ))
					if ((digit_num[digit] & (1 << box_y)) != 0)
						if (clock_style == 0)
							canvas[canvasid].fillRect(xOffset, y_offset - (box_y * (posmul + y_space)), posmul, posmul, on_color);

						else
							canvas[canvasid].fillSmoothCircle(xOffset + (half_posmul), y_offset - (box_y * (posmul + y_space)) + (half_posmul), half_posmul, on_color);

					else if (clock_style == 0)
						canvas[canvasid].fillRect(xOffset, y_offset - (box_y * (posmul + y_space)), posmul, posmul, off_color);

					else
						canvas[canvasid].fillSmoothCircle(xOffset + (half_posmul), y_offset - (box_y * (posmul + y_space)) + (half_posmul), half_posmul, off_color);
					if (show_borders)
					{
						if (clock_style == 0)
							canvas[canvasid].drawRect(xOffset, y_offset - (box_y * (posmul + y_space)), posmul, posmul, bdr_color);

						else
							canvas[canvasid].drawCircle(xOffset + (half_posmul), y_offset - (box_y * (posmul + y_space)) + (half_posmul), half_posmul, bdr_color);
					}
				}
			}

			// Set the text colour to make it readable
			int16_t digit_backcolor[6] = {off_color, off_color, off_color, off_color, off_color, off_color};
			for (int i = 0; i < 6; ++i)
			{
				if ((digit_num[i] & (1 << 0)) != 0)
					digit_backcolor[i] = on_color;
			}

			// Show time in bottom boxes for convenience
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[0]);
			for (int i = 0; i < 6; ++i)
			{
				canvas[canvasid].setTextColor(tim_color, digit_backcolor[i]);
				canvas[canvasid].drawString(String(digit_num[i]), x_offset + (posmul * box_positions[i]) + (x_space * box_positions[i]) + digit_xoffset, y_offset - (0 * (posmul + y_space)) + digit_yoffset);
			}

			// Show date below the clock
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[2]);
			canvas[canvasid].setTextColor(tim_color, RGB(0x00, 0x00, 0x00));
			canvas[canvasid].drawString(String(day), display.center_x - day_offset, display.height - 35);

			// Display Year and Month
			canvas[canvasid].setFreeFont(RobotoMono_Regular[16]);
			canvas[canvasid].setTextColor(tim_color, RGB(0x00, 0x00, 0x00));
			canvas[canvasid].drawString(String(year), display.center_x + 72, display.height - 50);
			canvas[canvasid].drawString(months[month - 1], display.center_x + 64, display.height - 25);

			draw_children(false, 0, abs(1 - clock_style));
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatch_CustomBinary::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	// Cycle through the colour pallette, after one full cycle, switch the style (square/circle)
	settings.config.custom_binary.binary_clockcolour++;
	if (settings.config.custom_binary.binary_clockcolour >= 3)
	{
		settings.config.custom_binary.binary_clockstyle = !(settings.config.custom_binary.binary_clockstyle);
		settings.config.custom_binary.binary_clockcolour = 0;
	}
	draw(true);
	return true;
}

bool FaceWatch_CustomBinary::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	display.cycle_clock_face();

	is_dragging = false;
	// draw(true);
	return true;
}

bool FaceWatch_CustomBinary::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	// info_println("LOOOONG!");
	// TODO: Add display of watch specific settings here when the user long presses
	return true;
}

FaceWatch_CustomBinary face_watch_custom_binary;
