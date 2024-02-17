#pragma once

#include "tw_faces/tw_face.h"

class FaceIMU : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
		bool showingGyro = false;
};

extern FaceIMU face_imu;