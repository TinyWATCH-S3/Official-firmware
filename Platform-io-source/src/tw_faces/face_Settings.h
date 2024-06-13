#pragma once

#include "tw_faces/tw_face.h"

class FaceSettings : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
};

extern FaceSettings face_settings;
extern FaceSettings face_settings_Audio;
extern FaceSettings face_settings_Haptics;
