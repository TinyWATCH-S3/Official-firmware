#include "tw_faces/face_System.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "tinywatch.h"

void FaceSystem::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceSystem::draw(bool force)
{
	if (force || millis() - next_update > update_period || inertia_y != 0)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached || is_scrolling)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].fillSprite(RGB(0x55, 0x55, 0x55));
			canvas[canvasid].setTextDatum(4); // Middle, Center

			int16_t widgets_height = tinywatch.messages.size() * 20;

			if (!is_dragging && abs(inertia_y) > 1)
			{
				// info_println("*** "+String(scroll_start_y) +" "+ String(scroll_pos) +" "+ String(inertia_y));
				inertia_y = inertia_y / 2.0;
				scroll_start_y += inertia_y;

				if (scroll_start_y > widgets_height)
					scroll_start_y = widgets_height;
				else if (scroll_start_y < widgets_height / 2)
					scroll_start_y = widgets_height / 2;
			}

			canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
			canvas[canvasid].setTextColor(TFT_YELLOW);
			canvas[canvasid].drawString("SYSTEM INFO", display.center_x, 20 + scroll_start_y);
			canvas[canvasid].setTextColor(TFT_WHITE);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[7]);

			// Display all system messages
			for (int i = 0; i < tinywatch.messages.size(); i++)
				canvas[canvasid].drawString(tinywatch.messages[i], 120, (scroll_start_y + 50 + i * 20));

			canvas[canvasid].fillRect(220, 30, 5, 200, RGB(0x33, 0x33, 0x33));
			int16_t scroll_length = (widgets_height <= 220) ? 220 : ((220.0 / (float)widgets_height) * 220);
			int diff = (int)((220 - (float)scroll_length) * scroll_pos);

			canvas[canvasid].fillRect(220, 30 + (220 - scroll_length) - diff, 5, scroll_length, RGB(0x66, 0x66, 0x66));
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceSystem::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceSystem::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceSystem::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

FaceSystem face_system;
