#include "tw_faces/face_Watch_DefaultDigital.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "fonts/Roboto_Regular18.h"
#include "fonts/Dialog_Fonts.h"
#include "peripherals/battery.h"
#include "peripherals/imu.h"
#include "settings/settings.h"
#include "tinywatch.h"
#include "RLEBitmap.h"
#include "bitmaps/bitmaps_watchface.h"
#include "web/webserver.h"
#include "wifi.h"

void FaceWatch_DefaultDigital::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
		// Get the bitmap information
		get_digital_wide_RLEBM(digitalwideInfo);

		get_batt00_RLEBM(battLCD_00);
		get_batt20_RLEBM(battLCD_20);
		get_batt40_RLEBM(battLCD_40);
		get_batt60_RLEBM(battLCD_60);
		get_batt80_RLEBM(battLCD_80);
		get_batt100_RLEBM(battLCD_100);

		get_wifi100_RLEBM(wifiLCD_100);
		get_wifi75_RLEBM(wifiLCD_75);
		get_wifi50_RLEBM(wifiLCD_50);
		get_wifi25_RLEBM(wifiLCD_25);
		get_wifi00_RLEBM(wifiLCD_00);

		get_act_00_RLEBM(activitybar_00);
		get_act_01_RLEBM(activitybar_01);
		get_act_02_RLEBM(activitybar_02);
		get_act_03_RLEBM(activitybar_03);
		get_act_04_RLEBM(activitybar_04);
		get_act_05_RLEBM(activitybar_05);
		get_act_06_RLEBM(activitybar_06);
		get_act_07_RLEBM(activitybar_07);
		get_act_08_RLEBM(activitybar_08);
		get_act_09_RLEBM(activitybar_09);
		get_act_10_RLEBM(activitybar_10);
		get_act_11_RLEBM(activitybar_11);
		get_act_12_RLEBM(activitybar_12);
		get_act_13_RLEBM(activitybar_13);

		get_v_RLEBM(icon_v);
		get_pcnt_RLEBM(icon_pcnt);
		get_db_RLEBM(icon_db);
		get_walking_RLEBM(activityLCD);

	}
}


void FaceWatch_DefaultDigital::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			face_iswide = true;

			const RLEBitmapInfo activitybar[14] = {activitybar_00, activitybar_01, activitybar_02, activitybar_03, activitybar_04, activitybar_05, activitybar_06, activitybar_07, activitybar_08, activitybar_09, activitybar_10, activitybar_11, activitybar_12, activitybar_13};
			const RLEBitmapInfo battLCD[6] = {battLCD_00, battLCD_20, battLCD_40, battLCD_60, battLCD_80, battLCD_100};
			const RLEBitmapInfo activity_icons[4] = {activityLCD,icon_v,icon_pcnt,icon_db};

			// Draw Face Background
			renderRLEBitmap(digitalwideInfo, 0, 0, &canvas[canvasid]);

			// Draw Off IP Box
			canvas[canvasid].drawRoundRect(40, 125, 160, 20, 10, TFT_LCD_OFF);

			// Wide Positions
			battpos_x = 36;
			battpos_y = 148;
			digital_yoffset = 37;
	
			// Activity Box
			canvas[canvasid].drawRoundRect(110, 73, 90, 50, 6, TFT_BLACK);
			// canvas[canvasid].fillRect(185, 93, 20, 30, TFT_LCD_BKG); // Looks silly.
			bool imu_movement = imu.get_movement_activity();


			if (imu_movement = "STILL")
			{
				//Do cycling

				display_cyclepause++;
				if (display_cyclepause > cycledelay_secs)
				{
					display_cyclepause = 0;
					display_actcycle++;

					uint8_t cycleoptions = 3; // exclude wifi by default, unless it's on
					if (WiFi.status() == WL_CONNECTED)
						cycleoptions = 4;

					if (display_actcycle > cycleoptions)
						display_actcycle = 0;
				}
			}
			else
			{
				// activity only
				display_actcycle = 0;
				display_cyclepause = 0;
			}

				canvas[canvasid].setFreeFont(Clock_Digit_7SEG[5]); //5 or 1 might be better
				canvas[canvasid].setTextDatum(MR_DATUM);
				canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);

			String modetext = "";
			RLEBitmapInfo step_bar_bmp;

			// Push the numbers to the right if the icon isn't shown. Looks neater
			uint8_t pushactright = 0;
			if (hide_activityicon) 
				pushactright = 10;
				
			if (display_actcycle == 0) // Step Counter
			{
				uint16_t day, month, year;
				rtc.get_step_date(day, month, year);
				uint32_t steps = imu.get_steps(day, month, year);
				step_bar_bmp = activitybar[map(steps, 0, 1000, 0, 13)];
				canvas[canvasid].drawString(String(steps), 185 + pushactright , 99);
				
				// Activity Window Icon
				if (!hide_activityicon) 
					renderRLEBitmap(activity_icons[0], 187, 95, &canvas[canvasid]);

				modetext = "STEPS";
			}
			else if (display_actcycle == 1) // Voltage
			{
				float bat_vo = battery.get_voltage(false);
				canvas[canvasid].drawString(String(bat_vo), 185 + pushactright, 99);
				step_bar_bmp = activitybar[maplim(bat_vo, 3.5, 4.2, 0, 13, true)];
				
				// Activity Window Icon
				if (!hide_activityicon) 
					renderRLEBitmap(activity_icons[1], 190, 100, &canvas[canvasid]);

				modetext = "BATT (V)";
			}
			else if (display_actcycle == 2) // Change Rate
			{
				float bat_cr = battery.get_rate(false);
				canvas[canvasid].drawString(String(bat_cr), 185 + pushactright, 99);
				step_bar_bmp = activitybar[maplim(bat_cr, -15.0, 15.0, 0, 13, true)];

				// Activity Window Icon
				if (!hide_activityicon) 
					renderRLEBitmap(activity_icons[2], 187, 99, &canvas[canvasid]);

				modetext = "CH/RATE (%)";
			}
			else if (display_actcycle == 3) // Battery Time Remain
			{
				float b_remain = battery.get_time_remaining(false);
				canvas[canvasid].drawString(String(b_remain), 185 + pushactright, 99);
				step_bar_bmp = activitybar[maplim(b_remain, 0, 24, 0, 13, true)];
				
				// Activity Window Icon
				if (!hide_activityicon) 
					renderRLEBitmap(activity_icons[1], 190, 100, &canvas[canvasid]);

				modetext = "BAT LIFE (H)";

			}
			else if (display_actcycle == 4) // Wifi Strength
			{
				int32_t wifi_rssi = WiFi.RSSI();
				canvas[canvasid].drawString(String(wifi_rssi) , 185 + pushactright, 99);

				// Signal Strength
				step_bar_bmp = activitybar[maplim(wifi_rssi, -90, -30, 0, 13, true)];
				
				// Activity Window Icon
				if (!hide_activityicon) 
					renderRLEBitmap(activity_icons[3], 186, 101, &canvas[canvasid]);

				modetext = "SIGNAL (dBm)";
			}

			// Activity Bar
			renderRLEBitmap(step_bar_bmp, stepbarpos_x, stepbarpos_y, &canvas[canvasid]);

			// Mode Text
			canvas[canvasid].setFreeFont(&Dialog_plain_9);
			canvas[canvasid].setTextDatum(BL_DATUM);
			canvas[canvasid].drawString(modetext, 114, 122);

			// Box Headers
			canvas[canvasid].setTextDatum(MR_DATUM);
			canvas[canvasid].drawRoundRect(40, 73, 60, 50, 6, TFT_BLACK);
			canvas[canvasid].setFreeFont(&Roboto_Mono_17);

			// Wifi Control and IP Address
			if (WiFi.status() == WL_CONNECTED)
			{
				canvas[canvasid].drawRoundRect(40, 125, 160, 20, 10, TFT_BLACK);
				canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
				canvas[canvasid].setTextDatum(TC_DATUM);
				canvas[canvasid].drawString(WiFi.localIP().toString(), 120, 129);

				renderRLEBitmap(wifiLCD_100, 44, 80, &canvas[canvasid]);
			}
			else
				renderRLEBitmap(wifiLCD_00, 44, 80, &canvas[canvasid]);

			canvas[canvasid].setTextDatum(BR_DATUM); // Bottom, Right
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[7]);
			canvas[canvasid].setTextColor(TFT_LCD_OFF, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(time_h_xpos, time_ypos + digital_yoffset);
			canvas[canvasid].print("88");
			canvas[canvasid].setCursor(time_m_xpos, time_ypos + digital_yoffset);
			canvas[canvasid].print("88");
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[5]);
			canvas[canvasid].setCursor(time_s_xpos, time_ypos + digital_yoffset);
			canvas[canvasid].print("88");

			// This only needs to happen once a day. Don't forget to optimise this
			String ddpad = "8";
			String dmpad = "8";

			if (rtc.get_day() > 9)
				ddpad = "88";

			if (rtc.get_month() > 9)
				dmpad = "88";

			String dateoffstring = ddpad + "/" + dmpad;

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[6]);
			canvas[canvasid].setTextDatum(MR_DATUM);
			canvas[canvasid].setTextColor(TFT_LCD_OFF, TFT_LCD_BKG, false);
			canvas[canvasid].drawString(dateoffstring, date_dm_xpos, date_dm_ypos + digital_yoffset);
			canvas[canvasid].drawString("888", date_day_xpos, date_day_ypos + digital_yoffset);

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[7]);
			canvas[canvasid].setTextColor(TFT_LCD_OFF, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(time_h_xpos, time_ypos + digital_yoffset);
			if (rtc.get_hours() < 10)
				canvas[canvasid].print("8");
			
			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);

			uint8_t offset_twelvehr = 0;

			// We can't use the padding offset as we shouldn't see a zero, but instead a space. 
			if (settings.config.time_24hour)
			{
				canvas[canvasid].setCursor(time_h_xpos, time_ypos + digital_yoffset);
				canvas[canvasid].print(rtc.get_hours_string(true, settings.config.time_24hour));
			}
			else
			{			
				if ((rtc.get_hours() < 10) || (((rtc.get_hours()) > 12 && (rtc.get_hours() < 22))))
				{
					offset_twelvehr = 34;
				}
				canvas[canvasid].setCursor(time_h_xpos + offset_twelvehr, time_ypos + digital_yoffset);
				canvas[canvasid].print(rtc.get_hours_string(false, settings.config.time_24hour));

				// Show AM/PM
				canvas[canvasid].setFreeFont(Clock_Digit_7SEG[5]);
				canvas[canvasid].setCursor(time_s_xpos + 3, time_ypos + digital_yoffset - 22);

				if (rtc.get_hours() > 11)
					canvas[canvasid].print("P");
				else
					canvas[canvasid].print("A");
			}
			
			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[7]);
			canvas[canvasid].setCursor(time_m_xpos, time_ypos + digital_yoffset);
			canvas[canvasid].print(rtc.get_mins_string(true));

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[5]);
			canvas[canvasid].setCursor(time_s_xpos, time_ypos + digital_yoffset);
			canvas[canvasid].print(rtc.get_secs_string(true));

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[6]);
			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
			canvas[canvasid].setCursor(date_dm_xpos, date_dm_ypos + digital_yoffset);

			

			canvas[canvasid].setFreeFont(Clock_Digit_7SEG[6]);
			canvas[canvasid].setTextDatum(MR_DATUM);
			canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
			canvas[canvasid].drawString(String(rtc.get_day()) + "/" + String(rtc.get_month()), date_dm_xpos, date_dm_ypos + digital_yoffset);
			canvas[canvasid].drawString(rtc.get_day_of_week(), date_day_xpos, date_day_ypos + digital_yoffset);


			if(ledcolon_on) 
			{
				canvas[canvasid].setFreeFont(Clock_Digit_7SEG[2]);
				canvas[canvasid].setTextColor(TFT_BLACK, TFT_LCD_BKG, false);
				canvas[canvasid].setCursor(time_m_xpos - 5, time_ypos + 5 + digital_yoffset);
				canvas[canvasid].print(":");	
				ledcolon_on = false;
			} 
			else 
				ledcolon_on = true;	
			
			// Battery
			float bat_pc = battery.get_percent(false);
			float bat_cr = battery.get_rate(false);
			float bat_vo = battery.get_voltage(false);

			if (!tinywatch.vbus_present()) // Discharging
			{
				// Show Battery Capacity
				if (bat_pc == 100)					{ renderRLEBitmap(battLCD_100, battpos_x, battpos_y, &canvas[canvasid]); }
				if (bat_pc >= 80 && bat_pc < 100)	{ renderRLEBitmap(battLCD_80,  battpos_x, battpos_y, &canvas[canvasid]); }
				if (bat_pc >= 60 && bat_pc < 80)	{ renderRLEBitmap(battLCD_60,  battpos_x, battpos_y, &canvas[canvasid]); }
				if (bat_pc >= 40 && bat_pc < 60)	{ renderRLEBitmap(battLCD_40,  battpos_x, battpos_y, &canvas[canvasid]); }
				if (bat_pc >= 20 && bat_pc < 40)	{ renderRLEBitmap(battLCD_20,  battpos_x, battpos_y, &canvas[canvasid]); }
				if (bat_pc >= 0  && bat_pc < 20)	{ renderRLEBitmap(battLCD_00,  battpos_x, battpos_y, &canvas[canvasid]); }
			}
			else // VBUS Present, Charging
			{
				// Cycle Battery LCD Segments to indicate the watch is charging. (USB Connected)

				RLEBitmapInfo batt_bmp;
				batt_bmp = battLCD[(battlcd_pos - 1) % 6];
				renderRLEBitmap(batt_bmp, battpos_x, battpos_y, &canvas[canvasid]);
				battlcd_pos = (battlcd_pos % 6) + 1;
			}

			draw_children(false, 0);
		}

		// Lets draw the screen, finally.
		canvas[canvasid].pushSprite(_x, _y);
		
	}
}

long FaceWatch_DefaultDigital::maplim(long x, long in_min, long in_max, long out_min, long out_max, bool limit) {
    const long run = in_max - in_min;
    if(run == 0){
        log_e("map(): Invalid input range, min == max");
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;

	long output = (delta * rise) / run + out_min;

	if (limit)
	{
		if (output < out_min)
			output = out_min;
		else if (output > out_max)
			output = out_max;
	}
    return output;
}

bool FaceWatch_DefaultDigital::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_DOUBLE)
	{
		display.cycle_clock_face();
		is_dragging = false;
		// draw(true);
		return true;
	}
	else if (touch_event.type == TOUCH_LONG)
	{
		// TODO: Add display of watch specific settings here when the user long presses


	}
	else if (touch_event.type == TOUCH_TAP && touch_event.x >= 40 && touch_event.x <= 100 && touch_event.y >= 73 && touch_event.y <=123)
	{
			// Wifi Touch
			//Serial.println("Wifi Touch");
			if (!web_server.is_running())
				web_server.start();

			else
				web_server.stop(false);
			return true;

	}
	else if (touch_event.type == TOUCH_TAP)
	{
		
	}

	return false;
}

FaceWatch_DefaultDigital face_watch_default_digital;