#pragma once

#include "settings/settingsOption.h"
#include "tw_controls/tw_control.h"

class ControlButton : public tw_control
{
	public:
		void draw(uint8_t canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool process_touch(touch_event_t touch_event);
		void set_target_face(tw_face *target);

	private:
		String version = "1.0";
		tw_face *target_face = nullptr;
};
