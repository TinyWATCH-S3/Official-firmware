
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

			canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
			canvas[canvasid].fillSprite(themes.current().col_background_bright);
			canvas[canvasid].setTextColor(themes.current().col_primary);
			canvas[canvasid].setTextDatum(MC_DATUM); // Middle, Center
			canvas[canvasid].drawString("IMU + MAG", 120, 30);

			canvas[canvasid].setFreeFont(RobotoMono_Regular[6]);

			imu.update();

			int pos_y = 65;

			canvas[canvasid].fillRoundRect(15, pos_y + 15, 90, 38, 8, themes.current().col_background_dull);
			canvas[canvasid].fillRoundRect(15, pos_y + 85, 90, 38, 8, themes.current().col_background_dull);
			canvas[canvasid].fillRoundRect(15, pos_y + 155, 90, 38, 8, themes.current().col_background_dull);

			canvas[canvasid].setTextColor(themes.current().col_secondary);

			if (showingGyro)
			{
				canvas[canvasid].drawString("GYRO X", 62, pos_y);
				canvas[canvasid].drawString("GYRO Y", 62, pos_y + 70);
				canvas[canvasid].drawString("GYRO Z", 62, pos_y + 140);

				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawNumber(imu.get_gyro_x(), 62, pos_y + 34);
				canvas[canvasid].drawNumber(imu.get_gyro_y(), 62, pos_y + 104);
				canvas[canvasid].drawNumber(imu.get_gyro_z(), 62, pos_y + 174);
			}
			else
			{
				canvas[canvasid].drawString("ACCEL X", 62, pos_y);
				canvas[canvasid].drawString("ACCEL Y", 62, pos_y + 70);
				canvas[canvasid].drawString("ACCEL Z", 62, pos_y + 140);

				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawFloat(imu.get_accel_x(), 2, 62, pos_y + 34);
				canvas[canvasid].drawFloat(imu.get_accel_y(), 2, 62, pos_y + 104);
				canvas[canvasid].drawFloat(imu.get_accel_z(), 2, 62, pos_y + 174);
			}

			canvas[canvasid].setTextColor(themes.current().col_secondary);
			canvas[canvasid].drawString("PITCH", 180, pos_y);
			canvas[canvasid].drawString("ROLL", 180, pos_y + 70);
			canvas[canvasid].drawString("YAW", 180, pos_y + 140);

			canvas[canvasid].fillRoundRect(135, pos_y + 15, 90, 40, 8, themes.current().col_background_dull);
			canvas[canvasid].fillRoundRect(135, pos_y + 85, 90, 40, 8, themes.current().col_background_dull);
			canvas[canvasid].fillRoundRect(135, pos_y + 155, 90, 40, 8, themes.current().col_background_dull);
			canvas[canvasid].setTextColor(TFT_WHITE);
			canvas[canvasid].drawNumber(imu.get_pitch(), 180, pos_y + 34);
			canvas[canvasid].drawNumber(imu.get_roll(), 180, pos_y + 104);
			canvas[canvasid].drawNumber(imu.get_yaw(), 180, pos_y + 174);

			for (int w = 0; w < widgets.size(); w++)
			{
				widgets[w]->draw(canvasid);
			}

			draw_navigation(canvasid);
		}

		update_screen();
	}
}

bool FaceIMU::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		showingGyro = !showingGyro;
		return true;
	}
	return false;
}

FaceIMU face_imu;