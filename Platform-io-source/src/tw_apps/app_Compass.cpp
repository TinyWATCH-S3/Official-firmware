
#include "tw_apps/app_Compass.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Regular_All.h"
#include "peripherals/imu.h"

#define RAD2DEG 0.0174532925
#define BACKGROUND TFT_BLACK

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
void AppCompass::pre_start() {}

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

void AppCompass::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();

		next_update = millis();
		angle = imu.get_yaw();

		drawCompass(120, 140, angle); // Draw centre of compass at 120,140
		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool AppCompass::click(int16_t pos_x, int16_t pos_y) { return false; }

bool AppCompass::click_double(int16_t pos_x, int16_t pos_y) { return false; }

void AppCompass::drawCompass(int x, int y, int angle)
{
	canvas[canvasid].setFreeFont(RobotoMono_Regular[12]);

	// TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
	// this allows it to be used in both 8 and 16 bit colour sprites.
	canvas[canvasid].fillSprite(TFT_TRANSPARENT);

#define NEEDLE_L 84 / 2 // Needle length is 84, we want radius which is 42
#define NEEDLE_W 12 / 2 // Needle width is 12, radius is then 6

	// Draw the old needle position in the screen background colour so
	// it gets erased on the TFT when the sprite is drawn
	canvas[canvasid].fillTriangle(lx1, ly1, lx3, ly3, lx4, ly4, BACKGROUND);
	canvas[canvasid].fillTriangle(lx2, ly2, lx3, ly3, lx4, ly4, BACKGROUND);

	// Set text coordinate datum to middle centre
	canvas[canvasid].setTextDatum(MC_DATUM);
	canvas[canvasid].setTextColor(TFT_WHITE);

	canvas[canvasid].drawString("N", 120, 140 - 42, 2);
	canvas[canvasid].drawString("E", 120 + 42, 140, 2);
	canvas[canvasid].drawString("S", 120, 140 + 42, 2);
	canvas[canvasid].drawString("W", 120 - 42, 140, 2);

	canvas[canvasid].drawCircle(120, 140, 30, TFT_DARKGREY);

	getCoord(x, y, &lx1, &ly1, NEEDLE_L, angle);
	getCoord(x, y, &lx2, &ly2, NEEDLE_L, angle + 180);
	getCoord(x, y, &lx3, &ly3, NEEDLE_W, angle + 90);
	getCoord(x, y, &lx4, &ly4, NEEDLE_W, angle - 90);

	canvas[canvasid].fillTriangle(lx1, ly1, lx3, ly3, lx4, ly4, TFT_RED);
	canvas[canvasid].fillTriangle(lx2, ly2, lx3, ly3, lx4, ly4, TFT_LIGHTGREY);

	canvas[canvasid].fillCircle(120, 140, 3, TFT_DARKGREY);
	canvas[canvasid].fillCircle(120, 140, 2, TFT_LIGHTGREY);
}

// Get coordinates of end of a vector, centre at x,y, length r, angle a
// Coordinates are returned to caller via the xp and yp pointers
void AppCompass::getCoord(int x, int y, int *xp, int *yp, int r, int a)
{
	float sx1 = cos((a - 90) * RAD2DEG);
	float sy1 = sin((a - 90) * RAD2DEG);
	*xp = sx1 * r + x;
	*yp = sy1 * r + y;
}

AppCompass app_compass;
