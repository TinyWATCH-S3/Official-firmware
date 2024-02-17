#pragma once

#include "tw_faces/tw_face.h"

class FaceSystem : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

		void add_message(String message);

	private:
		String version = "1.0";
};

extern FaceSystem face_system;