#include "tw_controls/tw_control.h"
#include "settings/settingsOption.h"

class ControlToggle : public tw_control
{
	public:
	    void draw(uint canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool click(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_double(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_long(uint16_t click_pos_x, uint16_t click_pos_y);
		void set_data(SettingsOptionBool *sett);

		

	private:
		SettingsOptionBool *setting_option = nullptr;
		String version = "1.0";

};