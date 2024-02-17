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

		bool process_touch(touch_event_t touch_event);

		void add_app(tw_app *app);
		bool icon_process_clicks(uint16_t touch_pos_x, uint16_t touch_pos_y);

		bool animate_app_in();
		void close_app();

	private:
		tw_app *current_app = nullptr;
		bool is_animating = false;

		uint16_t anim_icon_pos_x = 0;
		uint16_t anim_icon_pos_y = 0;
		uint16_t anim_icon_width = 0;
		uint16_t anim_icon_height = 0;
		uint8_t anim_corner_roundness = 6;
		uint8_t anim_backlight = 255;
};

extern FaceAppList face_applist;
