#pragma once

#include "tw_faces/tw_face.h"

class FaceWatchSettings : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(int16_t touch_pos_x, int16_t touch_pos_y);
		bool click_double(int16_t touch_pos_x, int16_t touch_pos_y);
		bool click_long(int16_t touch_pos_x, int16_t touch_pos_y);

	private:
		//
};

extern FaceWatchSettings face_watchsettings;
