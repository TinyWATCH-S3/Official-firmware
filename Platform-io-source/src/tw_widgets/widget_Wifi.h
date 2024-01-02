#include "tw_widgets/tw_widget.h"

class WidgetWifi : public tw_widget
{
	public:
		void draw(uint canvasid);
		bool click(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_double(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_long(uint16_t click_pos_x, uint16_t click_pos_y);

	private:
        String version = "1.0";

        uint32_t col_off = RGB(0x66, 0x00, 0x00);
        uint32_t col_on = RGB(0xd5, 0xa7, 0x00);

};
