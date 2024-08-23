#pragma once

#include "tw_apps/tw_app.h"

class AppEmpty : public tw_app
{
	public:
		// virtual methods
		void setup(void);
		void pre_start(void);
		void pre_close(void);
		void draw(bool force);
		void draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y);
		bool process_touch(touch_event_t touch_event);

		// local methods

	private:
		unsigned long shutdown_timer = 0;
};

extern AppEmpty app_empty;
