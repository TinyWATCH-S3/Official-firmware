#pragma once

#include "tw_apps/tw_app.h"
#include "tw_faces/tw_face.h"
#include <map>

class FaceAppList : public tw_face
{
	public:
		// Virtual
		void setup(void);
		void draw(bool force);
		bool click(int16_t pos_x, int16_t pos_y);
		bool click_double(int16_t pos_x, int16_t pos_y);
		bool click_long(int16_t pos_x, int16_t pos_y);

		void add_app(tw_app *app);
		bool icon_process_clicks(int16_t click_pos_x, int16_t click_pos_y);

		bool animate_app_in();
		void close_app();

	private:
		tw_app *current_app = nullptr;
		// int16_t icon_x = 30;
		// int16_t icon_y = 30;

		// int16_t icon_space_x = 90;
		// int16_t icon_space_y = 90;
};

extern FaceAppList face_applist;
