#pragma once

#include "tw_faces/tw_face.h"

class FaceEmpty : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		//
};

extern FaceEmpty face_empty;
