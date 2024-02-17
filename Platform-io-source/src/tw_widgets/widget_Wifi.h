#include "tw_widgets/tw_widget.h"

class WidgetWifi : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";

		uint32_t col_off = RGB(0x66, 0x00, 0x00);
		uint32_t col_on = RGB(0xd5, 0xa7, 0x00);
};
