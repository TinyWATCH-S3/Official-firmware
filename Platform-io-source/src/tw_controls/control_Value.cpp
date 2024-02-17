#include "tw_controls/control_Value.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlValue::set_data(SettingsOptionInt *sett) { setting_option = sett; }

void ControlValue::draw(uint _canvasid)
{
	canvasid = _canvasid;

	canvas[canvasid].setTextDatum(4); // Middle, Center

	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	val_width = 90;
	val_width_half = val_width / 2;
	button_left_x = adjusted_pos_x - val_width_half - height + 4;
	button_right_x = adjusted_pos_x + val_width_half - 4;

	canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
	canvas[canvasid].setTextColor(RGB(0xff, 0xc9, 0x00));
	canvas[canvasid].drawString(name, adjusted_pos_x, adjusted_pos_y - (height / 3));

	canvas[canvasid].setFreeFont(RobotoMono_Light[11]);
	canvas[canvasid].setTextColor(TFT_WHITE);
	// Button Minus
	canvas[canvasid].fillRoundRect(button_left_x, adjusted_pos_y, height, height, 4, RGB(0x44, 0x44, 0x44));
	canvas[canvasid].drawRoundRect(button_left_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));
	canvas[canvasid].drawString("-", button_left_x + (height / 2), adjusted_pos_y + (height / 2));

	// Button Plus
	canvas[canvasid].fillRoundRect(button_right_x, adjusted_pos_y, height, height, 4, RGB(0x44, 0x44, 0x44));
	canvas[canvasid].drawRoundRect(button_right_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));
	canvas[canvasid].drawString("+", button_right_x + (height / 2), adjusted_pos_y + (height / 2));

	// canvas[canvasid].fillRoundRect(pos_x, pos_y, width, height, 5, RGB(0x22, 0x22, 0x22));
	canvas[canvasid].fillRoundRect(pos_x - val_width_half, adjusted_pos_y, val_width, height, 4, RGB(0x22, 0x22, 0x22));
	canvas[canvasid].drawRoundRect(pos_x - val_width_half, adjusted_pos_y, val_width, height, 4, RGB(0x66, 0x66, 0x66));

	canvas[canvasid].drawNumber(value, adjusted_pos_x, adjusted_pos_y + (height / 2));
}

bool ControlValue::drag(int16_t drag_x, int16_t drag_y) { return false; }

void ControlValue::drag_end() {}

bool ControlValue::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (millis() - next_click_update > 50)
	{
		next_click_update = millis();

		if (buttons_check_left(touch_pos_x, touch_pos_y))
		{
			if (value > value_min)
			{
				value = constrain(value - 1, value_min, value_max);
				// Push the visuals to the screen
				draw(canvasid);
				canvas[canvasid].pushSprite(0, 0);
				// Reset the button flash
				canvas[canvasid].drawRoundRect(button_left_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));

				return true;
			}
			else
			{
				// Flash red
				canvas[canvasid].drawRoundRect(button_left_x, adjusted_pos_y, height, height, 4, RGB(0xff, 0x00, 0x00));
				canvas[canvasid].pushSprite(0, 0);
				canvas[canvasid].drawRoundRect(button_left_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));
			}
		}
		else if (buttons_check_right(touch_pos_x, touch_pos_y))
		{
			if (value < value_max)
			{
				value = constrain(value + 1, value_min, value_max);

				draw(canvasid);
				canvas[canvasid].pushSprite(0, 0);
				canvas[canvasid].drawRoundRect(button_right_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));

				return true;
			}
			else
			{
				// Flash red
				canvas[canvasid].drawRoundRect(button_right_x, adjusted_pos_y, height, height, 4, RGB(0xff, 0x00, 0x00));
				canvas[canvasid].pushSprite(0, 0);
				canvas[canvasid].drawRoundRect(button_right_x, adjusted_pos_y, height, height, 4, RGB(0x66, 0x66, 0x66));
			}
		}
	}

	return false;
}

bool ControlValue::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlValue::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlValue::buttons_check_left(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	int min_x, min_y = 0;
	int max_x = 240;
	int max_y = 280;

	if ((int)pos_y - padding_y > 0)
		min_y = (int)pos_y - padding_y;

	if ((int)pos_y + height + padding_y < 280)
		max_y = (int)pos_y + height + padding_y;

	min_x = constrain(button_left_x - padding_x, 0, 240);
	max_x = constrain(button_left_x + height + padding_x, 0, 240);

	bool in_x = (touch_pos_x >= min_x && touch_pos_x <= max_x);
	bool in_y = (touch_pos_y >= min_y + offset_y && touch_pos_y <= max_y + offset_y);

	return (in_x && in_y);
}

bool ControlValue::buttons_check_right(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	int min_x, min_y = 0;
	int max_x = 240;
	int max_y = 280;

	if ((int)pos_y - padding_y > 0)
		min_y = (int)pos_y - padding_y;

	if ((int)pos_y + height + padding_y < 280)
		max_y = (int)pos_y + height + padding_y;

	min_x = constrain(button_right_x + padding_x, 0, 240);
	max_x = constrain(button_right_x + height + padding_x, 0, 240);

	bool in_x = (touch_pos_x >= min_x && touch_pos_x <= max_x);
	bool in_y = (touch_pos_y >= min_y + offset_y && touch_pos_y <= max_y + offset_y);

	return (in_x && in_y);
}
