
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

			draw_navigation(canvasid, RGB(0x33, 0x33, 0x33));
		}

		update_screen();
	}
}

bool FaceEmpty::process_touch(touch_event_t touch_event) { return false; }

FaceEmpty face_empty;