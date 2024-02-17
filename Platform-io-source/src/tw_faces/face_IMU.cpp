
#include "tw_faces/face_IMU.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "peripherals/imu.h"

void FaceIMU::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceIMU::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			if (true)

				canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
			canvas[canvasid].fillSprite(RGB(0x22, 0x22, 0x22));
			canvas[canvasid].setTextColor(TFT_WHITE);
			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].drawString("IMU + MAG", 120, 40);

			canvas[canvasid].setFreeFont(RobotoMono_Regular[5]);

			imu.update();
			if (showingGyro)
			{
				canvas[canvasid].setTextColor(TFT_YELLOW);
				canvas[canvasid].drawString("GYRO", 55, 100);
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].fillRoundRect(10, 120, 105, 38, 8, RGB(0x55, 0x22, 0x22));
				canvas[canvasid].fillRoundRect(10, 175, 105, 38, 8, RGB(0x55, 0x22, 0x22));
				canvas[canvasid].fillRoundRect(10, 230, 105, 38, 8, RGB(0x55, 0x22, 0x22));
				canvas[canvasid].setCursor(15, 144);
				canvas[canvasid].print("X " + String(imu.get_gyro_x()));
				canvas[canvasid].setCursor(15, 199);
				canvas[canvasid].print("Y " + String(imu.get_gyro_y()));
				canvas[canvasid].setCursor(15, 254);
				canvas[canvasid].print("Z " + String(imu.get_gyro_z()));
			}
			else
			{
				canvas[canvasid].setTextColor(TFT_YELLOW);
				canvas[canvasid].drawString("ACCEL", 55, 100);
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].fillRoundRect(10, 120, 105, 38, 8, RGB(0x22, 0x22, 0x55));
				canvas[canvasid].fillRoundRect(10, 175, 105, 38, 8, RGB(0x22, 0x22, 0x55));
				canvas[canvasid].fillRoundRect(10, 230, 105, 38, 8, RGB(0x22, 0x22, 0x55));
				canvas[canvasid].setCursor(15, 144);
				canvas[canvasid].print("X " + String(imu.get_accel_x()));
				canvas[canvasid].setCursor(15, 199);
				canvas[canvasid].print("Y " + String(imu.get_accel_y()));
				canvas[canvasid].setCursor(15, 254);
				canvas[canvasid].print("Z " + String(imu.get_accel_z()));
			}

			canvas[canvasid].setTextColor(TFT_YELLOW);
			canvas[canvasid].drawString("PITCH", 185, 80);
			canvas[canvasid].drawString("ROLL", 185, 150);
			canvas[canvasid].drawString("YAW", 185, 220);

			canvas[canvasid].fillRoundRect(135, 95, 100, 40, 8, RGB(0x44, 0x44, 0x44));
			canvas[canvasid].fillRoundRect(135, 165, 100, 40, 8, RGB(0x44, 0x44, 0x44));
			canvas[canvasid].fillRoundRect(135, 235, 100, 40, 8, RGB(0x44, 0x44, 0x44));
			canvas[canvasid].setTextColor(TFT_WHITE);
			canvas[canvasid].drawNumber(imu.get_pitch(), 185, 115);
			canvas[canvasid].drawNumber(imu.get_roll(), 185, 185);
			canvas[canvasid].drawNumber(imu.get_yaw(), 185, 255);

			for (int w = 0; w < widgets.size(); w++)
			{
				widgets[w]->draw(canvasid, 0);
			}
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceIMU::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	showingGyro = !showingGyro;
	return true;
}

bool FaceIMU::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool FaceIMU::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

FaceIMU face_imu;