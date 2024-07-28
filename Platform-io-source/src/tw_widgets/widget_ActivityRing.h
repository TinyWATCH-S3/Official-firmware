#include "tw_widgets/tw_widget.h"

class WidgetActivityRing : public tw_widget
{
	public:
		void draw(uint canvasid);
		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
		uint32_t col_rings[3] = {RGB(0x00, 0x00, 0x66), RGB(0x00, 0x64, 0xaa), RGB(0x00, 0x00, 0xff)};

		uint8_t radius = 31;
		uint8_t thickness = 6;
		uint16_t arc_pos = 0;
};
