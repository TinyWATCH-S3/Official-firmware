#include "settings/settingsOption.h"
#include "tw_controls/tw_control.h"

class ControlButton : public tw_control
{
	public:
		void draw(uint8_t canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
};
