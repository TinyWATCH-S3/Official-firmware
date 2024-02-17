#pragma once

#include "tw_faces/tw_face.h"

class FaceWatch_DefaultAnalog : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		void setup_trig(void);
		void draw_hand(int x, int y, int x1, int y1, uint16_t color);
		String version = "1.0";
		bool cachedTrig = false;

		float pos_secs[60][2];
		float pos_mins[60][2];
		float pos_hours[12][2];

		int center_x = 120;
		int center_y = 131;
		float face_radius = 107;
};

extern FaceWatch_DefaultAnalog face_watch_default_analog;
