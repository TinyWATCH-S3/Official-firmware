#include "tw_widgets/widget_Wifi.h"
#include "bitmaps/bitmaps_general.h"
#include "fonts/RobotoMono_Light_All.h"
#include "web/webserver.h"
#include "wifi.h"

void WidgetWifi::draw(uint canvasid)
{
	canvas[canvasid].fillSmoothCircle(pos_x, pos_y + 23, 22, 0);

	if (WiFi.status() == WL_CONNECTED)
	{
		if (web_server.is_running())
		{
			// canvas[canvasid].fillSmoothRoundRect(pos_x, pos_y, 130, 14, 6, RGB(0x33, 0x33, 0x33));
			canvas[canvasid].setTextDatum(TC_DATUM); // Top, centert
			canvas[canvasid].setFreeFont(RobotoMono_Light[6]);
			canvas[canvasid].drawString(WiFi.localIP().toString(), display.center_x, pos_y + 5);
		}

		canvas[canvasid].pushImage(pos_x - 16, pos_y + 8, 32, 32, icon_wifi_on);
		// canvas[canvasid].drawSmoothArc(pos_x, pos_y + 23, 23, 19, 0, 360, col_on, 0);
	}
	else
	{
		canvas[canvasid].pushImage(pos_x - 16, pos_y + 8, 32, 32, icon_wifi_off);

	}
}

bool WidgetWifi::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		if (bounds_check(touch_event.x, touch_event.y))
		{
			if (!web_server.is_running())
				web_server.start();
			else
				web_server.stop(false);

			return true;
		}
	}

	return false;
}
