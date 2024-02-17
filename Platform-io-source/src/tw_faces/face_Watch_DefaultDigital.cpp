#include "tw_faces/face_Watch_DefaultDigital.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "fonts/Roboto_Regular18.h"
#include "peripherals/battery.h"
#include "peripherals/imu.h"
#include "settings/settings.h"
#include "tinywatch.h"

void FaceWatch_DefaultDigital::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceWatch_DefaultDigital::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[3]);
			canvas[canvasid].fillSprite(RGB(0x00, 0x0, 0x0));
			canvas[canvasid].setTextColor(TFT_WHITE);

			canvas[canvasid].setCursor(2, display.center_y);
			canvas[canvasid].setTextColor(RGB(0x22, 0x22, 0x22), 0);
			canvas[canvasid].print("888888");

			canvas[canvasid].setCursor(2, display.center_y);
			canvas[canvasid].setTextColor(RGB(0xEE, 0xEE, 0xFF), 0);
			canvas[canvasid].print(rtc.get_hours_string(true, settings.config.time_24hour));
			canvas[canvasid].setTextColor(RGB(0xBB, 0xBB, 0xDD), 0);
			canvas[canvasid].print(rtc.get_mins_string(true));
			canvas[canvasid].setTextColor(RGB(0x88, 0x88, 0xAA), 0);
			canvas[canvasid].print(rtc.get_secs_string(true));

			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[0]);
			canvas[canvasid].setTextColor(RGB(0x00, 0x65, 0xff), RGB(0x12, 0x12, 0x12));
			canvas[canvasid].drawString(rtc.get_day_date(), display.center_x, display.center_y - 70);

			draw_children(false, 0);
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatch_DefaultDigital::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceWatch_DefaultDigital::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	display.cycle_clock_face();

	is_dragging = false;
	// draw(true);
	return true;
}

bool FaceWatch_DefaultDigital::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	// info_println("LOOOONG!");
	// TODO: Add display of watch specific settings here when the user long presses
	return true;
}

FaceWatch_DefaultDigital face_watch_default_digital;