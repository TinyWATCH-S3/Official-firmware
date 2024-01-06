#pragma once

#include "tw_faces/tw_face.h"

class FaceWatch_DefaultDigital : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(int16_t pos_x, int16_t pos_y);
		bool click_double(int16_t pos_x, int16_t pos_y);
		bool click_long(int16_t pos_x, int16_t pos_y);

	private:
		String version = "1.0";
};

extern FaceWatch_DefaultDigital face_watch_default_digital;
