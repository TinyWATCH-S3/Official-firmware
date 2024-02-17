#include "tw_controls/control_ValueSlider.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlValueSlider::set_data(SettingsOptionInt *sett) { setting_option = sett; }

void ControlValueSlider::draw(uint _canvasid)
{
	canvasid = _canvasid;

	canvas[canvasid].setTextDatum(4); // Middle, Center

	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	val_width = 120;
	val_width_half = val_width / 2;
	arrow_left_x = adjusted_pos_x - val_width_half + 16;
	arrow_right_x = adjusted_pos_x + val_width_half - 16;

	canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
	canvas[canvasid].setTextColor(RGB(0xff, 0xc9, 0x00));
	canvas[canvasid].drawString(name, adjusted_pos_x, adjusted_pos_y - (height / 3));

	canvas[canvasid].setFreeFont(RobotoMono_Light[11]);
	canvas[canvasid].setTextColor(TFT_WHITE);

	// canvas[canvasid].fillRoundRect(pos_x-val_width_half, pos_y, val_width, height, 4, RGB(0x22, 0x22, 0x22));
	// canvas[canvasid].drawSmoothRoundRect(pos_x-val_width_half, pos_y, 4, 2, val_width, height, RGB(0x66, 0x66, 0x66));

	canvas[canvasid].fillRectHGradient(adjusted_pos_x - val_width_half, adjusted_pos_y, val_width_half, height, gradient_dark, gradient_bright);
	canvas[canvasid].fillRectHGradient(adjusted_pos_x, adjusted_pos_y, val_width_half, height, gradient_bright, gradient_dark);
	canvas[canvasid].drawRect(adjusted_pos_x - val_width_half, adjusted_pos_y, val_width, height, (selected ? col_selected : col_normal));

	canvas[canvasid].drawNumber(value, adjusted_pos_x, adjusted_pos_y + (height / 2));

	canvas[canvasid].setFreeFont(RobotoMono_Light[9]);
	canvas[canvasid].setTextColor(RGB(0x99, 0x99, 0x99));
	if (value > value_min)
		canvas[canvasid].drawString("<", arrow_left_x, adjusted_pos_y + (height / 2));
	if (value < value_max)
		canvas[canvasid].drawString(">", arrow_right_x, adjusted_pos_y + (height / 2));
}

bool ControlValueSlider::drag(int16_t drag_x, int16_t drag_y)
{
	if (can_scroll_x && abs(drag_x) > 0)
	{
		selected = true;
		if (millis() - next_click_update > (500 - (4 * abs(drag_x))))
		{
			next_click_update = millis();
			if ((drag_x > 10 && value <= value_max - value_step) || (drag_x < -10 && value >= value_min + value_step))
			{
				if (drag_x > 10)
					value = constrain(value + value_step, value_min, value_max);
				else if (drag_x < -10)
					value = constrain(value - value_step, value_min, value_max);

				draw(canvasid);
				return true;
			}
		}
	}
	else if (can_scroll_y && abs(drag_y) > 0)
	{
		info_println("Dragging Y " + String(drag_y));
	}

	return false;
}

void ControlValueSlider::drag_end() { selected = false; }

bool ControlValueSlider::click(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlValueSlider::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlValueSlider::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
