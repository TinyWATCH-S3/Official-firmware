
#include "tw_faces/face_Empty.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

// Called when face is set to current
void FaceEmpty::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceEmpty::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			// Start Custom Face Code

			canvas[canvasid].fillSprite(TFT_BLACK);
			canvas[canvasid].setTextDatum(MC_DATUM); // Middle, Center
			canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
			canvas[canvasid].setTextColor(TFT_GREEN);
			canvas[canvasid].drawString("Hello, World", display.center_x, display.center_y);

			// End Custom Face Code
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceEmpty::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceEmpty::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceEmpty::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

FaceEmpty face_empty;