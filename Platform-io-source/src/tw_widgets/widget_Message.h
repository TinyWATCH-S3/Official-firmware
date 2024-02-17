#include "tw_widget.h"

class WidgetMessage : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		String version = "1.0";
};