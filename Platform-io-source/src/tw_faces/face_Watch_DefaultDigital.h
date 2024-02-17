#pragma once

#include "tw_faces/tw_face.h"

class FaceWatch_DefaultDigital : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
};

extern FaceWatch_DefaultDigital face_watch_default_digital;
