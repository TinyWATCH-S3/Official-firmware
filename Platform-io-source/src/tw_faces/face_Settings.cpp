#include "tw_faces/face_Settings.h"
#include "fonts/RobotoMono_Regular_All.h"

void FaceSettings::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceSettings::draw(bool force)
{
	if (millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached || is_scrolling)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
			canvas[canvasid].fillSprite(RGB(0x44, 0x44, 0x44));
			canvas[canvasid].setTextColor(RGB(0x44, 0x44, 0xFF));
			canvas[canvasid].setTextDatum(4); // Middle, Center

			if (can_scroll_y)
			{
				// canvas[canvasid].setViewport(0, 40, 240, 240);

				// if ( !is_dragging && abs(inertia_y) > 1)
				// {
				// 	// info_println("*** "+String(scroll_start_y) +" "+ String(scroll_pos) +" "+ String(inertia_y));
				// 	inertia_y = inertia_y/2.0;
				// 	scroll_start_y +=inertia_y;

				// 	int16_t wHeight = get_control_height();

				// 	if (scroll_start_y > wHeight)
				// 		scroll_start_y = wHeight;
				// 	else if (scroll_start_y < wHeight / 2)
				// 		scroll_start_y = wHeight / 2;
				// }
				canvas[canvasid].drawString(name, 120, 30 + scroll_start_y);
				draw_children_scroll(0, scroll_start_y);

				// canvas[canvasid].setViewport(0, 0, 240, 280);

				canvas[canvasid].fillRect(230, 60, 5, 160, RGB(0x33, 0x33, 0x33));
				uint16_t widget_length = get_control_height();
				int16_t scroll_length = (widget_length <= 160) ? 160 : ((160.0 / (float)widget_length) * 200);
				int diff = (int)((160 - (float)scroll_length) * scroll_pos);

				canvas[canvasid].fillRect(230, 60 + (160 - scroll_length) - diff, 5, scroll_length, RGB(0x66, 0x66, 0x66));
			}
			else
			{
				canvas[canvasid].drawString(name, 120, 30);
				draw_children(false, 0);
			}

			draw_navigation(canvasid, RGB(0x77, 0x77, 0x77));
		}

		update_screen();
	}
}

bool FaceSettings::process_touch(touch_event_t touch_event) { return false; }

FaceSettings face_settings;
FaceSettings face_settings_Audio;
FaceSettings face_settings_Haptics;