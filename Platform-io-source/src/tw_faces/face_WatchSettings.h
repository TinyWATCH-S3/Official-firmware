#pragma once

#include "tw_faces/tw_face.h"

class FaceWatchSettings : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		//
};

extern FaceWatchSettings face_watchsettings;
