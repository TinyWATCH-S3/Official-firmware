#include "tw_widget.h"

class WidgetMessage : public tw_widget
{
	public:
		void draw(uint canvasid);
		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
};