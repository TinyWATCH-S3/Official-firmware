
#include "tw_apps/app_Compass.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "peripherals/imu.h"
#include "settings/settings.h"

/**
 * 
 */
void AppCompass::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

/**
 * @brief Put anything in here that you want to have run every time the app is opened
 *
 * This is not the same as setup() above that only ever gets called the first time the app opens
 *
 */
void AppCompass::pre_start() 
{
	running_state = RUNNING_STATE_DRAW;
}

/**
 * @brief Draw the icon that gets shown on the app menu face
 *
 * Icons are 64x64 with rounded corners as per the code below, but the inner content can be anything that represents the app well
 * @param canvasid
 * @param _pos_x
 * @param _pos_y
 * @param style_hint
 */
void AppCompass::draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint)
{
	if (!is_icon_cached)
	{
		is_icon_cached = true;
		icon_sprite.fillSprite(0);
		icon_sprite.drawSmoothRoundRect(0, 0, 10, 6, icon_width, icon_width, RGB(0xdd, 0xa6, 0x00), 0);
		icon_sprite.setTextDatum(4); // Middle, Center
		icon_sprite.setFreeFont(RobotoMono_Regular[15]);
		icon_sprite.setTextColor(TFT_WHITE);
		icon_sprite.drawSmoothCircle(icon_width / 2, icon_height / 2, 20, RGB(0xff, 0xc9, 0x00), 0);
		icon_sprite.drawString("C", icon_width / 2, icon_width / 2);
	}

	icon_x = _pos_x;
	icon_y = _pos_y;
	icon_sprite.pushToSprite(&canvas[canvasid], _pos_x, _pos_y);
}

/**
 * 
 */
bool AppCompass::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_SWIPE)
	{
		return false;
	}
	else if (touch_event.type == TOUCH_TAP)
	{
		if (running_state == RUNNING_STATE_DRAW)
		{
			if (touch_event.x < 32u && touch_event.y < 32u)
			{
				info_println("Entering calibrating magnetometer");
				resetCalibration();
				running_state = RUNNING_STATE_CALIBRATE;
				return true;
			}
			else
				return false;
		}
		else if (running_state == RUNNING_STATE_CALIBRATE)
		{
			if (touch_event.x < 32u && touch_event.y > 248u)
			{				
				// https://github.com/kriswiner/MPU6050/wiki/Simple-and-Effective-Magnetometer-Calibration
				info_println("Saving compass calibration values");

				float soft_iron_x = (mag_x_max - mag_x_min) / 2;
				float soft_iron_y = (mag_y_max - mag_y_min) / 2;
				float soft_iron_z = (mag_z_max - mag_z_min) / 2;
			
				float soft_scale = (soft_iron_x + soft_iron_y + soft_iron_z) / 3;

				settings.config.compass.hard_iron_x = (mag_x_max + mag_x_min) / 2;
				settings.config.compass.hard_iron_y = (mag_y_max + mag_y_min) / 2;
				settings.config.compass.hard_iron_z = (mag_z_max + mag_z_min) / 2;

				settings.config.compass.soft_iron_x = soft_scale / soft_iron_x;
				settings.config.compass.soft_iron_y = soft_scale / soft_iron_y;
				settings.config.compass.soft_iron_z = soft_scale / soft_iron_z;

				settings.save(true);			

				running_state = RUNNING_STATE_DRAW;
				return true;
			}
			else if (touch_event.x > 208u && touch_event.y > 248u)
			{
				info_println("Canceled compass calibration");
				running_state = RUNNING_STATE_DRAW;
				return true;
			}
			else
			{
				info_println("Click " + String(touch_event.x) + "," + String(touch_event.y));
				canvas[canvasid].fillSmoothCircle(touch_event.x, touch_event.y, 3, TFT_ORANGE, TFT_ORANGE);
				return false;
			}
		}
	}
	return false;
}

/**
 * 
 */
void AppCompass::draw(bool force)
{
	// Override the CPU settings for this app (if needed)
	setCpuFrequencyMhz(required_cpu_speed);

	if (force || millis() - next_update > update_period)
	{
		setup();
		next_update = millis();

		switch(running_state)
		{
			case RUNNING_STATE_DRAW:
			{
				drawCompass(); // Draw centre of compass at 120,140
				break;
			}

			case RUNNING_STATE_CALIBRATE:
			{
				drawCalibrate();
				break;
			}
		}

		drawUI();
		canvas[canvasid].pushSprite(_x, _y);
	}
}

/**
 * 
 */
void AppCompass::resetCalibration()
{
	mag_x_min = 6000;
	mag_y_min = 6000;
	mag_z_min = 6000;

	mag_x_max = -6000;
	mag_y_max = -6000;
	mag_z_max = -6000;
	
	canvas[canvasid].fillSprite(TFT_BLACK);
}


/**
 * 
 */
void AppCompass::drawUI()
{
	switch(running_state)
	{
		case RUNNING_STATE_DRAW:
		{
			uint16_t bx = 10, by = 10;
			uint16_t BUTTON_BG_COLOUR = B_BLUE;

			canvas[canvasid].fillSmoothRoundRect(-20, -20, 55, 55, 16, BUTTON_BG_COLOUR, BUTTON_BG_COLOUR);
			
			canvas[canvasid].fillRect(bx + 3, by + 3, 15, 15, TFT_DARKGREY);

			canvas[canvasid].fillRect(bx +  3, by +  3, 2, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 16, by +  3, 2, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx +  3, by + 16, 2, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 16, by + 16, 2, 2, BUTTON_BG_COLOUR);

			canvas[canvasid].fillRect(bx +  6, by +  3, 3, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 12, by +  3, 3, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx +  6, by + 16, 3, 2, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 12, by + 16, 3, 2, BUTTON_BG_COLOUR);

			canvas[canvasid].fillRect(bx +  3, by +  6, 2, 3, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 16, by +  6, 2, 3, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx +  3, by + 12, 2, 3, BUTTON_BG_COLOUR);
			canvas[canvasid].fillRect(bx + 16, by + 12, 2, 3, BUTTON_BG_COLOUR);

			canvas[canvasid].fillRect(bx +  8, by + 8, 5, 5, BUTTON_BG_COLOUR);

			canvas[canvasid].drawPixel(bx +  5, by +  3, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 15, by +  3, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  3, by +  5, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  7, by +  5, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 13, by +  5, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 17, by +  5, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  5, by +  7, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 10, by +  7, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 15, by +  7, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  7, by + 10, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 13, by + 10, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  5, by + 13, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 10, by + 13, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 15, by + 13, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  3, by + 15, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  7, by + 15, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 13, by + 15, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 17, by + 15, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx +  5, by + 17, BUTTON_BG_COLOUR);
			canvas[canvasid].drawPixel(bx + 15, by + 17, BUTTON_BG_COLOUR);
			break;
		}

		case RUNNING_STATE_CALIBRATE:
		{
			canvas[canvasid].fillSmoothRoundRect(-32, display.height - 32, 64, 64, 16, B_GREEN, B_GREEN);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[10]);
			canvas[canvasid].setTextColor(TFT_GREEN);
			canvas[canvasid].setTextDatum(CC_DATUM);
			canvas[canvasid].drawString("S", 16, display.height - 19);

			canvas[canvasid].fillSmoothRoundRect(display.width - 32, display.height - 32, 64, 64, 16, B_RED, B_RED);
			canvas[canvasid].setFreeFont(RobotoMono_Regular[10]);
			canvas[canvasid].setTextColor(TFT_RED);
			canvas[canvasid].setTextDatum(CC_DATUM);
			canvas[canvasid].drawString("X", display.width - 18, display.height - 19);
			break;
		}
	}
}

/**
 *
 */
void AppCompass::drawCalibrate()
{
	float mag_x;
	float mag_y;
	float mag_z;

	imu.get_magnetic(&mag_x, &mag_y, &mag_z, false);

	mag_x_min = min(mag_x, mag_x_min);
	mag_y_min = min(mag_y, mag_y_min);
	mag_z_min = min(mag_z, mag_z_min);

	mag_x_max = max(mag_x, mag_x_max);
	mag_y_max = max(mag_y, mag_y_max);
	mag_z_max = max(mag_z, mag_z_max);

	canvas[canvasid].fillSprite(TFT_BLACK);

	canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
	
	canvas[canvasid].setTextColor(TFT_WHITE);
	canvas[canvasid].setTextDatum(CC_DATUM);
	canvas[canvasid].drawString("Calibrating", display.width / 2, 20);		
	
	//canvas[canvasid].drawPixel(display.center_x + mag_x, display.center_y + mag_y, TFT_RED);
	//canvas[canvasid].drawPixel(display.center_x + mag_y, display.center_y + mag_z, TFT_GREEN);
	//canvas[canvasid].drawPixel(display.center_x + mag_z, display.center_y + mag_x, TFT_BLUE);
	
	//float xx = (mag_x_max + mag_x_min) / 2;
	//float yy = (mag_y_max + mag_y_min) / 2;
	//float zz = (mag_z_max + mag_z_min) / 2;

	//canvas[canvasid].drawSmoothCircle(display.center_x + xx, display.center_y + yy, abs(mag_x_max - mag_x_min) / 2, TFT_RED, TFT_TRANSPARENT);
	//canvas[canvasid].drawSmoothCircle(display.center_x + yy, display.center_y + zz, abs(mag_y_max - mag_y_min) / 2, TFT_GREEN, TFT_TRANSPARENT);
	//canvas[canvasid].drawSmoothCircle(display.center_x + zz, display.center_y + xx, abs(mag_z_max - mag_z_min) / 2, TFT_BLUE, TFT_TRANSPARENT);

	canvas[canvasid].drawSmoothCircle(display.center_x, display.center_y, abs(mag_x_max - mag_x_min) / 2, TFT_RED, TFT_TRANSPARENT);
	canvas[canvasid].drawSmoothCircle(display.center_x, display.center_y, abs(mag_y_max - mag_y_min) / 2, TFT_GREEN, TFT_TRANSPARENT);
	canvas[canvasid].drawSmoothCircle(display.center_x, display.center_y, abs(mag_z_max - mag_z_min) / 2, TFT_BLUE, TFT_TRANSPARENT);
}

/**
 * 
 */
void AppCompass::drawCompass()
{
	float newHeading = imu.get_yaw();

	float a = newHeading - heading;
	float b = newHeading < heading
		? newHeading + 360.0 - heading 
		: newHeading - 360.0 - heading
	;
	heading = abs(a) < abs(b)
		? heading + a * 0.5 // 0.5 dampen change in heading
		: heading + b * 0.5 // 0.5 dampen change in heading
	;

	if (heading >= 360.0)
		heading -= 360.0;
	if (heading < 0.0)
		heading += 360.0;

	canvas[canvasid].fillSprite(TFT_BLACK);

	/*
	 Rotate a unit 1 vector pointing down (Cartesian coordinate) [x: 0, y: 1] around the origin at the given angle
	 We can reduce the calcs in the rotation matrix due to the x component being 0 and y being 1
	 
	 xp = x(0) * cos(a) - y(1) * sin(a)
	 yp = y(1) * cos(a) + x(0) * sin(a)
	 
	 This recuces down to
	 
	 xp = -y * sin(a)
	 yp =  y * cos(a)
	 
	 The result is a normalized rotated vector that can then be multiplied by a radius
	*/
	
	float heading_in_rad = heading * DEG_TO_RAD;
	
	float normal_x, normal_y, normal_x_90, normal_y_90;	
	
	// get the rotated normalised vector
	normal_x = -sin(heading_in_rad);
	normal_y =  cos(heading_in_rad);

	// trick that rotates the normal 90 deg for E and W
	normal_x_90 = -normal_y;
	normal_y_90 =  normal_x;

	uint8_t text_N_x = display.center_x - NESW_RADIUS * normal_x;
	uint8_t text_N_y = display.center_y - NESW_RADIUS * normal_y;
	uint8_t text_S_x = display.center_x + NESW_RADIUS * normal_x;
	uint8_t text_S_y = display.center_y + NESW_RADIUS * normal_y;

	uint8_t text_E_x = display.center_x - NESW_RADIUS * normal_x_90 * 0.6;
	uint8_t text_E_y = display.center_y - NESW_RADIUS * normal_y_90 * 0.6;	
	uint8_t text_W_x = display.center_x + NESW_RADIUS * normal_x_90 * 0.6;
	uint8_t text_W_y = display.center_y + NESW_RADIUS * normal_y_90 * 0.6;

	canvas[canvasid].drawCircle(120, 140, 30, TFT_DARKGREY);

	canvas[canvasid].setFreeFont(RobotoMono_Regular[12]);
	canvas[canvasid].setTextColor(TFT_RED);
	canvas[canvasid].setTextDatum(CC_DATUM);
	canvas[canvasid].drawString("N", text_N_x, text_N_y);
	canvas[canvasid].setTextColor(TFT_WHITE);
	canvas[canvasid].drawString("E", text_E_x, text_E_y);
	canvas[canvasid].drawString("S", text_S_x, text_S_y);
	canvas[canvasid].drawString("W", text_W_x, text_W_y);
	canvas[canvasid].setTextColor(TFT_ORANGE);
	uint8_t l = canvas[canvasid].drawString(String(heading, 0), display.center_x, 240);
	canvas[canvasid].drawSmoothCircle(display.center_x + l * .5 + 7, 236, 3, TFT_ORANGE, TFT_BLACK);

	uint8_t needle_N_x = display.center_x - NEEDLE_L * normal_x;
	uint8_t needle_N_y = display.center_y - NEEDLE_L * normal_y;
	uint8_t needle_S_x = display.center_x + NEEDLE_L * normal_x;
	uint8_t needle_S_y = display.center_y + NEEDLE_L * normal_y;

	uint8_t needle_E_x = display.center_x - NEEDLE_W * normal_x_90;
	uint8_t needle_E_y = display.center_y - NEEDLE_W * normal_y_90;
	uint8_t needle_W_x = display.center_x + NEEDLE_W * normal_x_90;
	uint8_t needle_W_y = display.center_y + NEEDLE_W * normal_y_90;

	canvas[canvasid].fillTriangle(needle_N_x, needle_N_y, needle_E_x, needle_E_y, needle_W_x, needle_W_y, TFT_RED);
	canvas[canvasid].fillTriangle(needle_S_x, needle_S_y, needle_E_x, needle_E_y, needle_W_x, needle_W_y, TFT_LIGHTGREY);
	canvas[canvasid].fillSmoothCircle(120, 140, 3, TFT_DARKGREY, TFT_LIGHTGREY);
}

AppCompass app_compass;
