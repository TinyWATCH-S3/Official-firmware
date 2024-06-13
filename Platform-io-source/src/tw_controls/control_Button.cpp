#include "tw_controls/control_Button.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlButton::set_target_face(tw_face *target)
{
	target_face = target;
}

void ControlButton::draw(uint8_t canvasid)
{
	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;
	// info_printf("dirty? %d\n", is_dirty);
	if (should_cache && is_dirty)
	{
		uint16_t color = canvas[canvasid].readPixel(adjusted_pos_x, adjusted_pos_y);
		my_sprite.fillSprite(color);

		my_sprite.fillRoundRect(0, 0, width, height, 4, RGB(0x22, 0x22, 0x22));
		my_sprite.fillRoundRect(1, 1, width - 2, height - 2, 4, RGB(0x44, 0x44, 0x44));
		my_sprite.drawRoundRect(1, 1, width - 2, height - 2, 4, RGB(0x66, 0x66, 0x66));
		my_sprite.setFreeFont(RobotoMono_Light[7]);
		my_sprite.setTextColor(TFT_WHITE);
		my_sprite.setTextDatum(4); // Middle, Center
		my_sprite.drawString(name, (width / 2) - 1, (height / 2) - 1);
		is_dirty = false;
	}

	my_sprite.pushToSprite(&canvas[canvasid], adjusted_pos_x, adjusted_pos_y);
}

bool ControlButton::drag(int16_t drag_x, int16_t drag_y) { return false; }

void ControlButton::drag_end() {}

bool ControlButton::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		if (bounds_check(touch_event.x, touch_event.y))
		{
			canvas[canvasid].drawRoundRect(pos_x, pos_y, width, height, 4, RGB(0xff, 0xff, 0xff));
			canvas[canvasid].pushSprite(0, 0);
			canvas[canvasid].drawRoundRect(pos_x, pos_y, width, height, 4, RGB(0x66, 0x66, 0x66));

			if (callbackFunction != nullptr)
				callbackFunction();

			if (target_face != nullptr)
				display.set_current_face(target_face);

			is_dirty = true;

			return true;
		}
	}

	return false;
}
