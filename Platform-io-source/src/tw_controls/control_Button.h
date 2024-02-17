#include "settings/settingsOption.h"
#include "tw_controls/tw_control.h"

class ControlButton : public tw_control
{
	public:
		void draw(uint8_t canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		String version = "1.0";
};
