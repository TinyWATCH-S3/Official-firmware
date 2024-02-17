#include "tw_widgets/tw_widget.h"

class WidgetESP32 : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		String version = "1.0";
		unsigned long next_stat_cycle = 0;
};
