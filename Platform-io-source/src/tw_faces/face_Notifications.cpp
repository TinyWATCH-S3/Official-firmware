#include "tw_faces/face_Notifications.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "tinywatch.h"

void FaceNotifications::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceNotifications::draw(bool force)
{
	if (force || millis() - next_update > update_period || inertia_y != 0)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached || is_scrolling)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
			canvas[canvasid].fillSprite(TFT_PURPLE);
			canvas[canvasid].setTextColor(TFT_YELLOW);
			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].drawString("MESSAGES", 120, 40);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[7]);

			if (num_messages == 0)
			{
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawString("No new messages!", 120, 80);
			}
			else
			{
				canvas[canvasid].setViewport(0, 60, 240, 200);

				if (!is_dragging && abs(inertia_y) > 1)
				{
					// info_println("*** "+String(scroll_start_y) +" "+ String(scroll_pos) +" "+ String(inertia_y));
					inertia_y = inertia_y / 2.0;
					scroll_start_y += inertia_y;

					int16_t wHeight = get_widget_height();

					if (scroll_start_y > wHeight)
						scroll_start_y = wHeight;
					else if (scroll_start_y < wHeight / 2)
						scroll_start_y = wHeight / 2;
				}

				draw_children(true, scroll_start_y);

				canvas[canvasid].setViewport(0, 0, 240, 280);

				if (can_scroll_y)
				{
					canvas[canvasid].fillRect(220, 60, 5, 200, RGB(0x33, 0x33, 0x33));
					uint16_t widget_length = get_widget_height();
					int16_t scroll_length = (widget_length <= 200) ? 200 : ((200.0 / (float)widget_length) * 200);
					int diff = (int)((200 - (float)scroll_length) * scroll_pos);

					canvas[canvasid].fillRect(220, 60 + (200 - scroll_length) - diff, 5, scroll_length, RGB(0x66, 0x66, 0x66));

					// info_println("widget length "+String(widget_length));
					// info_println("scroll length "+String(scroll_length));
				}
			}
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceNotifications::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	pop_message();
	draw(true);
	return true;
}

bool FaceNotifications::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceNotifications::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

void FaceNotifications::add_message(String message)
{
	if (num_messages < 3)
	{
		message_queue[num_messages++] = message;
	}
}

void FaceNotifications::pop_message()
{
	if (num_messages > 0)
	{
		num_messages--;
		for (uint8_t i = 1; i < 4; i++)
			message_queue[i - 1] = message_queue[i];
	}
}

FaceNotifications face_notifications;
