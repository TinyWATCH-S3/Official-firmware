#pragma once

#include "tw_apps/tw_app.h"

class AppCompass : public tw_app
{
	public:
		void setup(void);
		void pre_start(void);
		void draw(bool force);
		void draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint);
		bool process_touch(touch_event_t touch_event);

	private:
		#define BACKGROUND TFT_BLACK
		
		// needle values
		#define NEEDLE_L 84 / 2 // Needle length is 84, we want radius which is 42
		#define NEEDLE_W 12 / 2 // Needle width is 12, radius is then 6
		#define NESW_RADIUS 60 // radius that N E S W rotate around	
			
		// running states
		#define RUNNING_STATE_DRAW 1
		#define RUNNING_STATE_CALIBRATE 2
		
		float heading = 0;
		uint8_t running_state;

		uint8_t needle_N_x = 0;
		uint8_t needle_N_y = 0;
		uint8_t needle_E_x = 0;
		uint8_t needle_E_y = 0;
		uint8_t needle_S_x = 0;
		uint8_t needle_S_y = 0;
		uint8_t needle_W_x = 0;
		uint8_t needle_W_y = 0;

		uint8_t text_N_x = 0;
		uint8_t text_N_y = 0;
		uint8_t text_E_x = 0;
		uint8_t text_E_y = 0;
		uint8_t text_S_x = 0;
		uint8_t text_S_y = 0;
		uint8_t text_W_x = 0;
		uint8_t text_W_y = 0;

		void drawCompass();
		void drawCalibrate();
		void resetCalibration();

		//these are needed for calibration

		float mag_x_min;
		float mag_y_min;
		float mag_z_min;

		float mag_x_max;
		float mag_y_max;
		float mag_z_max;
		
		float hard_iron_x;
		float hard_iron_y;
		float hard_iron_z;

		float soft_iron_x;
		float soft_iron_y;
		float soft_iron_z;
};

extern AppCompass app_compass;
