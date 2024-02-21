#pragma once

#include "tw_faces/tw_face.h"
#include "RLEBitmap.h"

class FaceWatch_DefaultDigital : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:
		String version = "1.0";
		bool ledcolon_on = false;
		RLEBitmapInfo bitmapInfo;

		uint16_t TFT_LCD_BKG = RGB(0xD3, 0xDC, 0xDB);
		uint16_t TFT_LCD_OFF = RGB(0xC3, 0xCC, 0xCB);

		uint8_t time_ypos = 180;
		uint8_t time_h_xpos = 35;
		uint8_t time_m_xpos = 107;
		uint8_t time_s_xpos = 174;

		uint8_t date_day_xpos = 80;
		uint8_t date_day_ypos = 125;

		uint8_t date_dm_xpos = 140;
		uint8_t date_dm_ypos = 125;


};

extern FaceWatch_DefaultDigital face_watch_default_digital;

