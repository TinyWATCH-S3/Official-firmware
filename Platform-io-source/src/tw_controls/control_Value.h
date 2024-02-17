#include "settings/settingsOption.h"
#include "tw_controls/tw_control.h"

class ControlValue : public tw_control
{
	public:
		void draw(uint _canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

		bool buttons_check_left(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool buttons_check_right(uint16_t touch_pos_x, uint16_t touch_pos_y);

		void set_data(SettingsOptionInt *sett);

	private:
		SettingsOptionInt *setting_option = nullptr;

		String version = "1.0";
		bool state = false;

		unsigned long next_click_update = 0;

		int value_min = 1;
		int value_max = 31;
		int value_inc = 1;

		int value = 1;

		int val_width = 90;
		int val_width_half = 0;
		int button_left_x = 0;
		int button_right_x = 0;
};