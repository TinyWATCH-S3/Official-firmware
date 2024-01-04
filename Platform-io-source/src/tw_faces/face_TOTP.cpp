
#include "tw_faces/face_TOTP.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "fonts/Clock_Digits.h"

// Called when face is set to current
void FaceTOTP::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceTOTP::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();
		
		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			// Get the TOTP and Time info
			uint8_t secs = rtc.get_seconds();

			canvas[canvasid].fillSprite(TFT_BLACK);
			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
			canvas[canvasid].setTextColor(TFT_GREEN);
			canvas[canvasid].drawString("TOTP", display.center_x, 75);
			canvas[canvasid].drawString("GMail", display.center_x, display.height - 75);

			if (secs == 0 || totp_token == "")
			{
				totp_token = "";
				for (int i = 0; i <= 5; ++i) 
				{
					int randomDigit = rand() % 10;
					totp_token += String(randomDigit);
				}
			}

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[1]);
			canvas[canvasid].setTextColor(TFT_WHITE);
			canvas[canvasid].drawString(totp_token, display.center_x, display.center_y);

			int8_t bar = 60 - secs;
			int radius = 120;
			int width = 15;

			uint16_t countdown_colour =      RGB(0x00,0xff,0x00);  // Green
			if (bar < 15) countdown_colour = RGB(0xff,0xff,0x00);  // Yellow
			if (bar < 5) countdown_colour =  RGB(0xff,0x00,0x00);  // Red

			display.fill_arc(canvasid, 120, 140, 0, constrain(60, 1, 60), radius, radius, width, RGB(0x10,0x10,0x10));
			display.fill_arc(canvasid, 120, 140, 0, constrain(bar, 1, 60), radius, radius, width, countdown_colour);
		}

		canvas[canvasid].pushSprite(_x,_y);
	}
}

bool FaceTOTP::click(uint pos_x, uint pos_y)
{
	return false;
}

bool FaceTOTP::click_double(uint pos_x, uint pos_y)
{
	return false;
}

bool FaceTOTP::click_long(uint pos_x, uint pos_y)
{
	return false;
}

FaceTOTP face_totp;