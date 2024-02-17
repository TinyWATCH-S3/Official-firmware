
#include "tw_faces/face_BatteryEmpty.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "peripherals/battery.h"

// Called when face is set to current
void FaceBatteryEmpty::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceBatteryEmpty::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].fillSprite(TFT_BLACK);
			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
			canvas[canvasid].setTextColor(TFT_GREEN);

			canvas[canvasid].pushImage(104, 128, 32, 24, icon_batteryArray[0]);
			canvas[canvasid].drawSmoothCircle(120, 140, 30, RGB(0xFF, 0x00, 0x00), 0);

			float bat_volt = battery.get_voltage(true);
			canvas[canvasid].drawString(String(bat_volt) + "V", 120, 180);

			// canvas[canvasid].drawWideLine(95, 165, 145, 115, 4, RGB(0xFF, 0x00, 0x00));
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceBatteryEmpty::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceBatteryEmpty::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceBatteryEmpty::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

FaceBatteryEmpty face_batteryempty;