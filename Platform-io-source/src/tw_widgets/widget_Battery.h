#include "tw_widgets/tw_widget.h"

class WidgetBattery : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		String version = "1.0";

		uint32_t col_low[2] = {RGB(0x66, 0x00, 0x00), RGB(0xbb, 0x00, 0x00)};
		uint32_t col_med[2] = {RGB(0x6c, 0x3a, 0x00), RGB(0xbb, 0x64, 0x00)};
		uint32_t col_high[2] = {RGB(0x00, 0x55, 0x00), RGB(0x00, 0xaa, 0x64)};

		uint32_t col_text[2] = {RGB(0xff, 0xff, 0xff), RGB(0x00, 0xaa, 0x64)};
};
