#pragma once

#include "tw_faces/tw_face.h"
#include "web/wifi_setup.h"

class FaceBoot : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

		void wifi_connect_status(wifi_states status);
		void update_wifisetup_status(String txt, uint32_t color);

	private:
		String version = "1.0";
		String wifi_connection_strings[7] = {"BOOT", "AP MODE", "CONNECTING", "UPDATING TIME", "DONE", "ERROR, RETRY...", "RESET"};
		wifi_states wifi_status = BOOT;
};

extern FaceBoot face_boot;
