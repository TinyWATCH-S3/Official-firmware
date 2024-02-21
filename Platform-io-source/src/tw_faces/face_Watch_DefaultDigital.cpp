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
#include "RLEBitmap.h"
#include "bitmaps/bitmaps_watchface.h"

void FaceWatch_DefaultDigital::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
		// Get the bitmap information
		get_Use_this_one_RLEBM(bitmapInfo);
		TFTeSPIGraphicsContext context(&tft); 
	}
}

//#include <TFT_eSPI.h>

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

			renderRLEBitmap(bitmapInfo, 0, 0, &canvas[canvasid]);
			
			canvas[canvasid].setTextDatum(BR_DATUM); // Bottom, Right
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[7]);
			canvas[canvasid].setTextColor(TFT_LCD_OFF, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(time_h_xpos, time_ypos);
			canvas[canvasid].print("88");
			canvas[canvasid].setCursor(time_m_xpos, time_ypos);
			canvas[canvasid].print("88");
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[6]);
			canvas[canvasid].setCursor(time_s_xpos, time_ypos);
			canvas[canvasid].print("88");

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[7]);
			canvas[canvasid].setTextColor(TFT_LCD_OFF, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(time_h_xpos, time_ypos);
			if (rtc.get_hours() < 10)
				canvas[canvasid].print("8");
			
			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
			canvas[canvasid].print(rtc.get_hours_string(false, settings.config.time_24hour));
			canvas[canvasid].setCursor(time_m_xpos, time_ypos);
			canvas[canvasid].print(rtc.get_mins_string(true));

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[5]);
			canvas[canvasid].setCursor(time_s_xpos, time_ypos);
			canvas[canvasid].print(rtc.get_secs_string(true));

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[6]);
			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(date_dm_xpos, date_dm_ypos);
			canvas[canvasid].print(String(rtc.get_day()) + "/" + String(rtc.get_month()));

			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(date_day_xpos, date_day_ypos);
			canvas[canvasid].print(rtc.get_day_of_week());

			if(ledcolon_on) 
			{
				canvas[canvasid].setFreeFont(Clock_Digit_7SEG[2]);
				canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
				canvas[canvasid].setCursor(time_m_xpos - 5, time_ypos + 5);
				canvas[canvasid].print(":");	
				ledcolon_on = false;
			} 
			else 
				ledcolon_on = true;	
			
			draw_children(false, 0);
		}

		// Lets draw the screen, finally.
		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatch_DefaultDigital::process_touch(touch_event_t touch_event)
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
	}

	return false;
}

FaceWatch_DefaultDigital face_watch_default_digital;