
#include "tw_apps/app_Empty.h"
// #include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

/**
 * @brief Called the first time an app is opened
 *
 * Put any one off setup code in here
 *
 */
void AppEmpty::setup()
{
	if (!is_setup)
	{
		is_setup = true;
	}
}

/**
 * @brief Put anything in here that you want to have run every time the app is opened
 *
 * This is not the same as setup() above that only ever gets called the first time the app opens
 *
 */
void AppEmpty::pre_start() { shutdown_timer = millis(); }

/**
 * @brief Draw the icon that gets shown on the app menu face
 *
 * Icons are 64x64 with rounded corners as per the code below, but the inner content can be anything that represents the app well
 * @param canvasid
 * @param _pos_x
 * @param _pos_y
 * @param style_hint
 */
void AppEmpty::draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint)
{
	if (!is_icon_cached)
	{
		is_icon_cached = true;
		icon_sprite.fillSprite(0);
		icon_sprite.drawSmoothRoundRect(0, 0, 10, 6, icon_width, icon_width, RGB(0xCC, 0xAA, 0x77), 0);
		icon_sprite.setTextDatum(4); // Middle, Center
		icon_sprite.setFreeFont(RobotoMono_Regular[15]);
		icon_sprite.setTextColor(TFT_WHITE);
		icon_sprite.drawString("Hi", icon_width / 2, icon_width / 2);
	}

	icon_x = _pos_x;
	icon_y = _pos_y;
	icon_sprite.pushToSprite(&canvas[canvasid], _pos_x, _pos_y);
}

/**
 * @brief Usual draw code that draws the apps content, like a face draw it's content
 *
 * @param force
 */
void AppEmpty::draw(bool force)
{
	// Override the CPU settings for this app (if needed)
	// setCpuFrequencyMhz(160);

	setup();

	// Example of how to auto shut down the app after a period of time
	//
	// if (millis() - shutdown_timer > 5000)
	// {
	// 	// close without saving settings
	// 	close(false);
	// 	return;
	// }

	if (force || millis() - next_update > update_period)
	{
		next_update = millis();

		canvas[canvasid].fillSprite(TFT_BLACK);

		// Start Custom App Code

		canvas[canvasid].fillSprite(TFT_BLACK);
		canvas[canvasid].setTextDatum(MC_DATUM); // Middle, Center
		canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
		canvas[canvasid].setTextColor(TFT_GREEN);
		canvas[canvasid].drawString("Hello, World", display.center_x, display.center_y);

		// End Custom App Code
	}
	canvas[canvasid].pushSprite(_x, _y);
}

/**
 * @brief Did we click on the app? If so, where?
 *
 * @param touch_pos_x
 * @param touch_pos_y
 * @return true
 * @return false
 */
bool AppEmpty::click(uint16_t touch_pos_x, uint16_t touch_pos_y)
{

	return false;
}

/**
 * @brief Did we double click on the app? If so, where?
 *
 * @param touch_pos_x
 * @param touch_pos_y
 * @return true
 * @return false
 */

bool AppEmpty::click_double(uint16_t touch_pos_x, uint16_t touch_pos_y) { return false; }

bool AppEmpty::swipe(uint16_t touch_pos_x, uint16_t touch_pos_y, int8_t swipe_dir, int16_t dist_x, int16_t dist_y)
{
	String swipe_dir_names[4] = {"UP", "RIGHT", "DOWN", "LEFT"};
	info_printf("swipe dir: %s at touch pos (%d,%d) distance: (%d,%d)\n", swipe_dir_names[swipe_dir], touch_pos_x, touch_pos_y, dist_x, dist_y);

	return true;
}

		


AppEmpty app_empty;
