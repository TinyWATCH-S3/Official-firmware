#pragma once

#include "tw_apps/tw_app.h"

class AppCompass : public tw_app
{
	public:
		void setup(void);
		void pre_start(void);
		void draw(bool force);
		void draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint);
		bool process_touch(touch_event_t touch_event);

		void drawCompass(int x, int y, int angle);
		void getCoord(int x, int y, int *xp, int *yp, int r, int a);

	private:
		String version = "1.0";
		bool showingGyro = false;

		int number = 0;
		int angle = 0;

		int lx1 = 0;
		int ly1 = 0;
		int lx2 = 0;
		int ly2 = 0;
		int lx3 = 0;
		int ly3 = 0;
		int lx4 = 0;
		int ly4 = 0;

		// Test only
		uint16_t n = 0;
		uint32_t dt = 0;
};

extern AppCompass app_compass;
