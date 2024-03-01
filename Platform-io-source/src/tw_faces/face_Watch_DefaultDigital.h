#pragma once

#include "tw_faces/tw_face.h"
#include "RLEBitmap.h"
#include "activity.h"

class FaceWatch_DefaultDigital : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);

		bool process_touch(touch_event_t touch_event);

	private:

		void draw_battery(uint8_t battpos_x, uint8_t battpos_y, TFT_eSprite canvasid);

		String version = "1.0";
		bool ledcolon_on = false;
		bool face_iswide = false;

		RLEBitmapInfo digitalwideInfo;
		RLEBitmapInfo battLCD_00;
		RLEBitmapInfo battLCD_20;
		RLEBitmapInfo battLCD_40;
		RLEBitmapInfo battLCD_60;
		RLEBitmapInfo battLCD_80;
		RLEBitmapInfo battLCD_100;

		RLEBitmapInfo wifiLCD_00;
		RLEBitmapInfo wifiLCD_25;
		RLEBitmapInfo wifiLCD_50;
		RLEBitmapInfo wifiLCD_75;
		RLEBitmapInfo wifiLCD_100;

		RLEBitmapInfo activityLCD;
		RLEBitmapInfo activitybar_13;
		RLEBitmapInfo activitybar_12;
		RLEBitmapInfo activitybar_11;
		RLEBitmapInfo activitybar_10;
		RLEBitmapInfo activitybar_09;
		RLEBitmapInfo activitybar_08;
		RLEBitmapInfo activitybar_07;
		RLEBitmapInfo activitybar_06;
		RLEBitmapInfo activitybar_05;
		RLEBitmapInfo activitybar_04;
		RLEBitmapInfo activitybar_03;
		RLEBitmapInfo activitybar_02;
		RLEBitmapInfo activitybar_01;
		RLEBitmapInfo activitybar_00;

		RLEBitmapInfo activitybar[14];
		RLEBitmapInfo battLCD[6];


		uint8_t battlcd_pos = 1;

		uint16_t TFT_LCD_BKG = RGB(0xD3, 0xDC, 0xDB);
		uint16_t TFT_LCD_OFF = RGB(0xC3, 0xCC, 0xCB);

		uint8_t time_ypos = 180;
		uint8_t time_h_xpos = 35;
		uint8_t time_m_xpos = 107;
		uint8_t time_s_xpos = 174;

		uint8_t date_day_xpos = 80;
		uint8_t date_day_ypos = 127;

		uint8_t date_dm_xpos = 140;
		uint8_t date_dm_ypos = 127;

		uint8_t digital_yoffset = 0;

		uint8_t battpos_x = 0;
		uint8_t battpos_y = 0;


};

extern FaceWatch_DefaultDigital face_watch_default_digital;

