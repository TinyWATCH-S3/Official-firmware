
#include "tw_controls/control_Toggle.h"
#include "fonts/RobotoMono_Light_All.h"

void ControlToggle::set_data(SettingsOptionBool *sett)
{
	setting_option = sett;
	value = setting_option->get() ? option2 : option1;
}

// Disabled for now, but the plan is to cache the drawn state of a control
// To speed up redraw and to reduce battery consumption
/*
void ControlToggle::draw(uint8_t canvasid)
{
	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	if (requires_redraw)
	{
		// requires_redraw = false;
		my_sprite.fillSprite(TFT_TRANSPARENT);
		my_sprite.fillRoundRect(0, 0, width, height, 4, RGB(0x22, 0x22, 0x22));
		my_sprite.setFreeFont(RobotoMono_Light[7]);
		my_sprite.setTextDatum(4); // Middle, Center
		my_sprite.setTextColor(TFT_WHITE);
		my_sprite.drawString(name, (width/2), -10);


		if (setting_option->get())
			my_sprite.fillRoundRect((width/2)+2, 2, width/2-2, height-4, 4, RGB(0x99, 0xFF, 0x99));
		else
			my_sprite.fillRoundRect(2, 2, width/2-2, height-4, 4, RGB(0x66, 0x66, 0x66));

		my_sprite.setTextColor(TFT_WHITE, RGB(0x22, 0x22, 0x22));
		my_sprite.setFreeFont(RobotoMono_Light[8]);
		my_sprite.drawString(value, (width/2), (height/2) -2);
	}

	my_sprite.pushToSprite(&canvas[canvasid], adjusted_pos_x, adjusted_pos_y);
}
*/

void ControlToggle::draw(uint8_t canvasid)
{
	adjusted_pos_x = pos_x + offset_x;
	adjusted_pos_y = pos_y + offset_y;

	canvas[canvasid].fillRoundRect(adjusted_pos_x, adjusted_pos_y, width, height, 4, RGB(0x22, 0x22, 0x22));
	canvas[canvasid].setFreeFont(RobotoMono_Light[7]);
	canvas[canvasid].setTextDatum(4); // Middle, Center
	canvas[canvasid].setTextColor(TFT_WHITE);
	canvas[canvasid].drawString(name, adjusted_pos_x + (width / 2), adjusted_pos_y - 10);

	if (setting_option->get())
		canvas[canvasid].fillRoundRect(adjusted_pos_x + (width / 2) + 2, adjusted_pos_y + 2, width / 2 - 2, height - 4, 4, RGB(0x99, 0xFF, 0x99));
	else
		canvas[canvasid].fillRoundRect(adjusted_pos_x + 2, adjusted_pos_y + 2, width / 2 - 2, height - 4, 4, RGB(0x66, 0x66, 0x66));

	canvas[canvasid].setTextColor(TFT_WHITE, RGB(0x22, 0x22, 0x22));
	canvas[canvasid].setFreeFont(RobotoMono_Light[8]);
	canvas[canvasid].drawString(value, adjusted_pos_x + (width / 2), adjusted_pos_y + (height / 2) - 2);
}

bool ControlToggle::drag(int16_t drag_x, int16_t drag_y) { return false; }

void ControlToggle::drag_end() {}

bool ControlToggle::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	// Did we click within the bounds of this control?
	if (bounds_check(touch_pos_x, touch_pos_y))
	{
		// If we have bound a SettingOption to this control, fire the change method
		if (setting_option != nullptr)
			value = setting_option->change() ? option2 : option1;
		// If we have assigned a callback on this control, fire it
		if (callbackFunction != nullptr)
			callbackFunction();
		// Yes, we did click this control
		// requires_redraw = true;
		return true;
	}

	return false;
}

bool ControlToggle::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool ControlToggle::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }
