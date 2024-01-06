
#include "tw_faces/face_AppList.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

// Apps
class tw_app;


static std::map<String, tw_app *> app_icons;

// Called when face is set to current
void FaceAppList::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}


void FaceAppList::add_app(tw_app *app)
{
    app_icons[app->name] = app;
}

bool FaceAppList::icon_process_clicks(uint click_pos_x, uint click_pos_y)
{
    for (auto _app : app_icons)
    {
        if (_app.second->click_icon(click_pos_x, click_pos_y))
        {
            current_app = _app.second;
            current_app->pre_start();
            prevent_dragging(true);
            next_update = 0;
            return true;
        }
    }

	return false;
}

/**
 * @brief Find a way to make the apps animate in and out nicely, so it's not so jaring.
 * 
 * @return true 
 * @return false 
 */
bool FaceAppList::animate_app_in()
{
    // uint16_t _width = 120;
    // uint16_t _height = 140;

    // while (_width < 120)
    // {
    //     canvas[1].setWindow(_width, _height, 240-_width, 280-_height);
    //     current_app->set_canvas(1);
    //     current_app->draw(force);
    //     canvas[1]
    // }
    return false;
}

/**
 * @brief Draw the Face
 * 
 * @param force 
 */
void FaceAppList::draw(bool force)
{
    // If we have launched an app, it becomes the thing we see, any messaging to the AppList Face goes to the app 
    if (current_app != nullptr)
    {
        current_app->draw(force);
        return;
    }

    // No app running, so we treat the face like a normal face
	if (force || millis() - next_update > update_period)
	{
		setup();
		
		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			canvas[canvasid].fillSprite(TFT_BLACK);
			canvas[canvasid].setTextDatum(4); // Middle, Center
			canvas[canvasid].setFreeFont(RobotoMono_Regular[15]);
			canvas[canvasid].setTextColor(TFT_GREEN);

            int16_t icon_x = 20;
            int16_t icon_y = 20;

            for (auto _app : app_icons)
            {
                _app.second->draw_icon(canvasid, icon_x, icon_y, 0);
                icon_x += 84;
                if (icon_x > 262)
                {
                    icon_x = 84;
                    icon_y += 84;
                }
            }

		}

		canvas[canvasid].pushSprite(_x,_y);
	}
}

bool FaceAppList::click(uint pos_x, uint pos_y)
{
    if (current_app != nullptr)
    {
        
        bool was_clicked = (current_app->click(pos_x, pos_y));
        // info_printf("Clicked app %s result %d\n",current_app->name, was_clicked);
        return (was_clicked);
    }

    if (icon_process_clicks(pos_x, pos_y))
    {
        return true;
    }
    return false;
}

bool FaceAppList::click_double(uint pos_x, uint pos_y)
{
	return false;
}

bool FaceAppList::click_long(uint pos_x, uint pos_y)
{
    if (current_app != nullptr)
    {
        current_app = nullptr;
        prevent_dragging(false);
        draw(true);
        return true;
    }
	return false;
}

void FaceAppList::close_app()
{
    if (current_app != nullptr)
    {
        current_app = nullptr;
        prevent_dragging(false);
        reset_cache_status();
        draw(true);
    }
}

FaceAppList face_applist;