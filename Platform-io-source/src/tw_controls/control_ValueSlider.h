#include "settings/settingsOption.h"
#include "tw_controls/tw_control.h"

class ControlValueSlider : public tw_control
{
	public:
		void draw(uint _canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);
		void set_data(SettingsOptionInt *sett);

	private:
		String version = "1.0";
		bool state = false;
		bool selected = false;

		SettingsOptionInt *setting_option = nullptr;

		unsigned long next_click_update = 0;

		int value_min = 1;
		int value_max = 31;
		int value_step = 1;

		int value = 1;

		int val_width = 90;
		int val_width_half = 0;
		int arrow_left_x = 0;
		int arrow_right_x = 0;

		uint32_t gradient_bright = RGB(0x33, 0x33, 0x33);
		uint32_t gradient_dark = RGB(0x11, 0x11, 0x11);
		uint32_t col_selected = RGB(0xaa, 0xaa, 0xaa);
		uint32_t col_normal = RGB(0x66, 0x66, 0x66);
};