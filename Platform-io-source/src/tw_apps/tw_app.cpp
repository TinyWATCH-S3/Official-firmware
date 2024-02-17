#include "tw_apps/tw_app.h"
#include "settings/settings.h"
#include "tw_faces/face_AppList.h"

extern TFT_eSPI tft;
extern TFT_eSprite canvas[];

static std::map<String, tw_app *> apps;

/**
 * @brief Setup the app and add it to a MAP of all apps
 *
 * @param _name
 * @param _update_period
 * @param req_cpu_speed
 */
void tw_app::add(String _name, uint16_t _update_period, uint32_t req_cpu_speed)
{
	name = _name;
	update_period = _update_period;
	required_cpu_speed = req_cpu_speed;

	// Only add the face to the MAP if there's no key for this name already.
	if (apps.find(name) == apps.end())
	{
		apps[name] = this;
	}
	else
	{
		error_print("ERROR ADDING APP: ");
		error_print(name);
		error_println(" already exists!");
	}

	// icons (for now) are 64x64 and get cached in the icon sprite, so only ever drawn once to save power
	// but at the cost of memory
	// This is important if we are going to have a face of scrollable app icons.
	// If we only ever have a single non scrollable page, then we can remove this as the app icon face itself will cache the
	icon_sprite.setSwapBytes(true);
	icon_sprite.createSprite(64, 64);
}

/**
 * @brief Quick override for Add above
 *
 * @param _name
 * @param _update_period
 */
void tw_app::add(String _name, uint _update_period)
{
	// Default to 40Mhz for this face
	add(_name, _update_period, 40);
}

/**
 * @brief Set the canvas ID for the app to draw into
 *
 * @param _canvasid
 */
void tw_app::set_canvas(uint _canvasid) { canvasid = _canvasid; }

/**
 * @brief Close the app and go back to the app list
 *
 * @param save save settings on exit
 */
void tw_app::close(bool save)
{
	if (save)
		settings.save(true);

	face_applist.close_app();
}

void tw_app::get_icon_pos(uint16_t &_icon_x, uint16_t &_icon_y, uint16_t &_icon_width, uint16_t &_icon_height)
{
	_icon_x = icon_x;
	_icon_y = icon_y;
	_icon_width = icon_width;
	_icon_height = icon_height;
}

/**
 * @brief Check if we have clicked on an app icon
 *
 * Needs to have better bounds checking with padding.
 *
 * @param touch_pos_x
 * @param touch_pos_y
 * @return true
 * @return false
 */
bool tw_app::click_icon(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	if ((touch_pos_x >= icon_x && touch_pos_x <= icon_x + icon_width) && (touch_pos_y >= icon_y && touch_pos_y <= icon_y + icon_height))
		return true;

	return false;
}
