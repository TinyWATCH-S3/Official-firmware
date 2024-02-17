#include "tw_widgets/tw_widget.h"

class WidgetESP32 : public tw_widget
{
	public:
		void draw(uint canvasid, uint8_t style_hint = 0);
		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
		unsigned long next_stat_cycle = 0;
};
