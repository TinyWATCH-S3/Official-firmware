
#include "tw_faces/face_Boot.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "settings/settings.h"
#include "tinywatch.h"

void FaceBoot::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceBoot::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].fillSprite(TFT_BLACK);
			canvas[canvasid].setRotation(0);
			canvas[canvasid].setTextDatum(4); // Middle, Center

			canvas[canvasid].setFreeFont(RobotoMono_Light[9]);
			canvas[canvasid].setTextColor(RGB(0xAA, 0xAA, 0xAA));
			canvas[canvasid].drawString(tinywatch.version_year + " Unexpected Maker", 120, 250);

			canvas[canvasid].setFreeFont(RobotoMono_Light[7]);
			canvas[canvasid].drawString("Firmware " + tinywatch.version_firmware, 120, 270);

			if (wifi_status == BOOT)
			{
				// Not trying to connect to WiFi - so just show the standard boot screen
				canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
				canvas[canvasid].setTextColor(TFT_GREEN);
				canvas[canvasid].drawString("TinyWATCH S3", 120, 100);

				canvas[canvasid].drawBitmap(75, 140, UM_Logo, 90, 49, TFT_WHITE);
			}
			else if (wifi_status == WIFI_SETUP)
			{
				canvas[canvasid].pushImage(104, 10, 32, 24, icon_wifi);
				canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
				canvas[canvasid].setTextColor(TFT_SKYBLUE);
				canvas[canvasid].drawString("WiFi", 120, 60);

				canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawString("Join the TinyWATCH", 120, 90);
				canvas[canvasid].drawString("hotspot on your phone", 120, 110);
				canvas[canvasid].drawString("then select your WiFi", 120, 130);
				canvas[canvasid].drawString("network, enter your", 120, 150);
				canvas[canvasid].drawString("password and press", 120, 170);
				canvas[canvasid].drawString("CONNECT!", 120, 190);

				// Cycle the status if we have connected and are scanning - can't thing of a nicer way to do this atm :(
				if (wifiSetup.wifi_ap_messages == "SCANNING...")
					wifi_status = WIFI_SETUP_STEP_2;

				// This is nasty hacky for now - to let people skip the wifi setup - they wont get time, but they'll get to play
				// with the watch.
				canvas[canvasid].setTextColor(TFT_BLUE);
				canvas[canvasid].drawString("TAP SCREEN TO SKIP!", 120, 220);
			}
			else if (wifi_status == WIFI_SETUP_STEP_2)
			{
				canvas[canvasid].pushImage(104, 10, 32, 24, icon_wifi);
				canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
				canvas[canvasid].setTextColor(TFT_SKYBLUE);
				canvas[canvasid].drawString("WiFi", 120, 60);

				canvas[canvasid].setFreeFont(RobotoMono_Regular[9]);
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawString("WiFi Router Setup", 120, 90);

				if (wifiSetup.wifi_ap_messages != "")
				{
					canvas[canvasid].setFreeFont(RobotoMono_Regular[11]);
					canvas[canvasid].setTextColor(wifiSetup.wifi_ap_message_color);
					canvas[canvasid].drawString(wifiSetup.wifi_ap_messages, 120, 140);
				}
			}
			else
			{
				canvas[canvasid].pushImage(104, 10, 32, 24, icon_wifi);
				canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
				canvas[canvasid].setTextColor(TFT_SKYBLUE);
				canvas[canvasid].drawString("WiFi", 120, 60);

				canvas[canvasid].setFreeFont(RobotoMono_Regular[12]);
				canvas[canvasid].setTextColor(TFT_WHITE);
				canvas[canvasid].drawString(wifi_connection_strings[(int)wifi_status], 120, 90);
			}

			if (wifi_status == WIFI_SETUP || wifi_status == WIFI_SETUP_STEP_2)
			{
				// info_println(getCpuFrequencyMhz());
				wifiSetup.process();
				if (wifiSetup.is_done())
				{
					settings.update_wifi_credentials(wifiSetup.get_ssid(), wifiSetup.get_pass());
					// Delay required so the wifi client can land on the connected page
					delay(1000);
					wifiSetup.stop(true);
					wifi_status = BOOT;
				}
			}
		}

		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceBoot::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if (wifi_status == WIFI_SETUP || wifi_status == WIFI_SETUP_STEP_2)
	{
		wifi_status = BOOT;
		draw(true);
		wifiSetup.stop(false);
		return true;
	}
	return false;
}

bool FaceBoot::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return true; }

bool FaceBoot::click_long(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

void FaceBoot::wifi_connect_status(wifi_states status)
{
	wifi_status = status;
	info_println("Setting wifi status to " + wifi_connection_strings[(int)wifi_status]);
	// Only force draw the screen is we are not resetting the status to 0
	if ((int)wifi_status > 0)
		draw(true);
}

FaceBoot face_boot;
