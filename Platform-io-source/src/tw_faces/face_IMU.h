#pragma once

#include "tw_faces/tw_face.h"

class FaceIMU : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(int16_t touch_pos_x, int16_t touch_pos_y);
		bool click_double(int16_t touch_pos_x, int16_t touch_pos_y);
		bool click_long(int16_t touch_pos_x, int16_t touch_pos_y);

	private:
		String version = "1.0";
		bool showingGyro = false;
};

extern FaceIMU face_imu;