
#include "tw_faces/face_WatchSettings.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

void FaceWatchSettings::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceWatchSettings::draw(bool force)
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
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceWatchSettings::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceWatchSettings::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceWatchSettings::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

FaceWatchSettings face_watchsettings;
