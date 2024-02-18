#pragma once

#include "tw_faces/tw_face.h"

class FaceNotifications : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

		void add_message(String message);

		void notify_new_message();

	private:
		String version = "1.0";
		String message_queue[4];
		uint8_t num_messages = 0;
		void pop_message();
};

extern FaceNotifications face_notifications;