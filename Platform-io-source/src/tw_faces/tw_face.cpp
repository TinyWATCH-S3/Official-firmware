/**
 * @file tw_face.cpp
 * @details The `tw_face` class represents a face on the TinyWatch and includes methods for handling touch events, dragging, and managing controls on the face.
 *
 */

#include "tw_faces/tw_face.h"
#include "peripherals/buzzer.h"
#include "tinywatch.h"
#include "utilities/logging.h"

static std::map<String, tw_face *> faces;

class tw_app;

void tw_face::add(String _name, uint16_t _update_period, uint32_t req_cpu_speed)
{
	name = _name;
	update_period = _update_period;
	required_cpu_speed = req_cpu_speed;
	is_clock_face = false;
	// Only add the face to the MAP if there's no key for this name already.
	if (faces.find(name) == faces.end())
	{
		faces[name] = this;
	}
	else
	{
		error_print("ERROR ADDING FACE: ");
		error_print(name);
		error_println(" already exists!");
	}
}

void tw_face::add(String _name, uint _update_period)
{
	// Default to 40Mhz for this face
	add(_name, _update_period, 40);
}

void tw_face::add_clock(String _name, uint16_t _update_period, uint32_t req_cpu_speed)
{
	name = _name;
	update_period = _update_period;
	required_cpu_speed = req_cpu_speed;
	is_clock_face = true;
	// Only add the face to the MAP if there's no key for this name already.
	if (faces.find(name) == faces.end())
	{
		faces[name] = this;
	}
	else
	{
		error_print("ERROR ADDING FACE: ");
		error_print(name);
		error_println(" already exists!");
	}

	display.add_clock_face(this);
}

void tw_face::add_clock(String _name, uint _update_period)
{
	// Default to 40Mhz for this face
	add_clock(_name, _update_period, 40);
}

bool tw_face::is_face_clock_face() { return is_clock_face; }

void tw_face::reset_can_swipe_flags()
{
	for (int i = 0; i < 4; i++)
		can_drag_dir[i] = false;
}

uint32_t tw_face::get_cpu_speed() { return (required_cpu_speed); }

int tw_face::check_can_drag_in_dir()
{
	for (int i = 0; i < 4; i++)
	{
		if (can_drag_dir[i])
			return i;
	}

	return -1;
}

/**
 * @brief Searches through all of the controls added to the face to see if any are "draggable" and if so
 * returns the control
 *
 * @param touch_pos_x
 * @param touch_pos_y
 * @return tw_control*
 */
tw_control *tw_face::find_draggable_control(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	for (int w = 0; w < controls.size(); w++)
	{
		if (controls[w]->can_drag(touch_pos_x, touch_pos_y))
			return controls[w];
	}
	return nullptr;
}

/**
 * @brief Prevent dragging the face - Used for apps that cant be dragged.
 *
 * @param state
 */
void tw_face::prevent_dragging(bool state)
{
	block_dragging = state;
	info_printf("blocked dragging: %d for %s\n", block_dragging, name);
}

/**
 * @brief Return if the face is being blocked from dragging.
 *
 * @return bool
 */
bool tw_face::is_drag_blocked() { return block_dragging; }

/**
 * @brief Called on the face to notify it there was a finger down event on the face
 *
 * At this stage we don't know if this is a click, or the intent is to drag the face to a new one
 *
 * @param pos_x
 * @param pos_y
 */
void tw_face::drag_begin(int16_t pos_x, int16_t pos_y)
{
	if (block_dragging)
		return;

	drag_start_time = millis();
	drag_dir = -1;

	drag_start_x = pos_x;
	drag_start_y = pos_y;
	click_hold_start_timer = millis();
	next_click_update = millis();

	selectedControl = find_draggable_control(pos_x, pos_y);
}

/**
 * @brief Notifies teh face that the user is dragging their finder on the screen an passes in a bunch of info
 * for the method to calculate exactly what the users intent is, which is quite complicated.
 *
 * Clicks are now handled outside of the face (in display.cpp) so what we try to determine here is if the user intends
 * to swipe teh face to a new one, and what options there are to do that fom this faces navigation options.
 *
 * When a face is being dragged or swiped to a new face, as it's moving, the face is also responsible to moving the adjacent face.
 *
 * My plan is to clean this all up/optimise/refine this - eventually
 *
 * @param drag_x
 * @param drag_y
 * @param pos_x
 * @param pos_y
 * @param t_pos_x
 * @param t_pos_y
 * @param current_face
 */
void tw_face::drag(int16_t drag_x, int16_t drag_y, int16_t pos_x, int16_t pos_y, int16_t t_pos_x, int16_t t_pos_y, bool current_face)
{
	if (block_dragging)
		return;

	canvasid = current_face ? 1 : 0;

	// We only move the other faces if we are the current_face (current) face
	if (current_face)
	{
		// Let's see if we are holding our finger still
		if (drag_dir == -1 && selectedControl == nullptr && abs(drag_x) < 1 && abs(drag_y) < 1)
		{
			// Have we been holding still for half a second?
			if (millis() - click_hold_start_timer > 500)
			{
				// Are we over a control?
				if (control_process_clicks(t_pos_x, t_pos_y))
				{
					// draw(true);
					BuzzerUI({{2000, 2}});
					return;
				}
			}
		}
		else if (selectedControl != nullptr)
		{
			if (selectedControl->drag(drag_x, drag_y))
			{
				BuzzerUI({{2000, 2}});
			}
			return;
		}

		is_scrolling = false;

		// can the contents of the face scroll? If so we calculate
		if (can_scroll_y && _y == 0)
		{
			if (abs(drag_x) <= abs(drag_y))
			{
				int16_t wHeight = get_widget_height() + get_control_height();

				if (wHeight > 260)
				{
					if (pos_y != 0)
						inertia_y = pos_y;

					scroll_start_y += inertia_y;

					if (scroll_start_y > 0)
						scroll_start_y = 0;
					else if (scroll_start_y < -(wHeight - 280))
						scroll_start_y = -(wHeight - 280);

					int scroll_pos_int = map(scroll_start_y, -(wHeight - 280), 0, 0, 100);
					scroll_pos = (float)scroll_pos_int / 100;

					is_scrolling = true;

					if (scroll_start_y > -(wHeight - 281))
						return;
				}

				is_scrolling = false;
			}
		}

		if (drag_dir == -1 && (abs(drag_x) > 10 || abs(drag_y) > 10))
		{
			// pre calc if am able to swipe based on where I start my touch
			if (drag_start_x < drag_width && abs(drag_x) > abs(drag_y)) // swipe right, drag in from left if face exists
				can_drag_dir[3] = (navigation[3] != nullptr);
			else if (drag_start_x > 240 - drag_width && abs(drag_x) > abs(drag_y)) // swipe left, drag in from right if face exists
				can_drag_dir[1] = (navigation[1] != nullptr);
			else if (drag_start_y < drag_width && abs(drag_x) < abs(drag_y)) // swipe down, drag in from top if face exists
				can_drag_dir[2] = (navigation[2] != nullptr);
			else if (drag_start_y > 280 - drag_width && abs(drag_x) < abs(drag_y)) // swipe up, drag in from bottom if face exists
				can_drag_dir[0] = (navigation[0] != nullptr);

			drag_dir = check_can_drag_in_dir();

			drag_lock_y = (drag_dir == 0 || drag_dir == 2);
			drag_lock_x = (drag_dir == 1 || drag_dir == 3);
		}

		bool swipe_chance = (drag_dir >= 0);
		is_dragging = (drag_dir >= 0);

		// Lock the axis we are not dragging, and lock the max or min value based on the neighbour
		if (drag_lock_x)
		{
			if (swipe_chance || abs(pos_x) < 25)
			{
				_y = 0;
				_x = drag_x;
				if (drag_dir == 3)
					_x = constrain(_x, 0, 240);
				else if (drag_dir == 1)
					_x = constrain(_x, -240, 0);
			}
		}
		else if (drag_lock_y)
		{
			if (swipe_chance || abs(pos_y) < 25)
			{
				_x = 0;
				_y = drag_y;
				if (drag_dir == 2)
					_y = constrain(_y, 0, 280);
				else if (drag_dir == 0)
					_y = constrain(_y, -280, 0);
			}
		}

		// Draw the dragged face
		if (is_dragging)
			draw(true);

		// We only drag the neighbour face if we have one!
		if (drag_dir >= 0)
		{
			if (drag_lock_x)
				navigation[drag_dir]->drag(_x + (_x < 0 ? 240 : -240), _y, pos_x, pos_y, 0, 0, false);
			else if (drag_lock_y)
				navigation[drag_dir]->drag(_x, _y + (_y < 0 ? 280 : -280), pos_x, pos_y, 0, 0, false);
		}
	}
	else
	{
		// we are the not the current_face face, so just update the positions and draw
		is_dragging = true;
		_x = drag_x;
		_y = drag_y;
		draw(true);
	}
}

bool tw_face::drag_end(int16_t drag_x, int16_t drag_y, bool current_face, int16_t distance, int16_t t_pos_x, int16_t t_pos_y, int16_t last_dir_x, int16_t last_dir_y)
{
	if (block_dragging)
		return false;

	if (selectedControl != nullptr)
	{
		selectedControl->drag_end();
		selectedControl = nullptr;
		return false;
	}

	// Lets see if we should spring back or switch face
	bool switch_face = false;
	int16_t target = 0;

	if (drag_dir == 0 && last_dir_y > 0)
		drag_dir = -1;
	else if (drag_dir == 2 && last_dir_y < 0)
		drag_dir = -1;
	else if (drag_dir == 1 && last_dir_x > 0)
		drag_dir = -1;
	else if (drag_dir == 3 && last_dir_x < 0)
		drag_dir = -1;

	if (drag_lock_x)
	{
		switch_face = (abs(drag_x) > 110);
		target = (_x > 0) ? 240 : -240;
	}
	else if (drag_lock_y)
	{
		switch_face = (abs(drag_y) > 130);
		target = (_y > 0) ? 280 : -280;
	}

	// We only switch face (like a swipe) if there's a face on the other side
	// Otherwise we spring back
	if (switch_face && drag_dir >= 0)
	{
		if (navigation[drag_dir] != nullptr)
		{
			if (drag_lock_x)
			{
				int16_t delta = (target - _x) / 2;
				while (abs(delta) > 1)
				{
					_x += delta;
					delta = (target - _x) / 2;
					draw(true);
					if (current_face && navigation[drag_dir] != nullptr)
						navigation[drag_dir]->drag(_x - target, _y, t_pos_x, t_pos_y, 0, 0, false);
				}
				// We only move the other faces if we are the current_face (current) face
				if (current_face && navigation[drag_dir] != nullptr)
				{
					navigation[drag_dir]->drag(0, 0, t_pos_x, t_pos_y, 0, 0, false);
					// navigation[drag_dir]->reset_cache_status();
				}
			}
			else if (drag_lock_y)
			{
				// info_println("drag_dir "+String(drag_dir)+" target "+String(target));
				int16_t delta = (target - _y) / 2;
				while (abs(delta) > 1)
				{
					_y += delta;
					delta = (target - _y) / 2;

					draw(true);
					if (current_face && navigation[drag_dir] != nullptr)
						navigation[drag_dir]->drag(_x, _y - target, t_pos_x, t_pos_y, 0, 0, false);
				}
				// We only move the other faces if we are the current_face (current) face
				if (current_face && navigation[drag_dir] != nullptr)
				{
					navigation[drag_dir]->drag(0, 0, t_pos_x, t_pos_y, 0, 0, false);
					// navigation[drag_dir]->reset_cache_status();
				}
			}

			// Exit early and let the display code switch the current face
			drag_lock_x = false;
			drag_lock_y = false;

			canvasid = 0;
			is_dragging = false;
			reset_can_swipe_flags();

			return true;
		}
	}
	else
	{
		// Spring back to the 0,0 position
		if (drag_lock_x)
		{
			int16_t delta = _x / 2;
			while (abs(_x) > 1)
			{
				_x -= delta;
				delta = _x / 2;
				draw(true);
				if (current_face && drag_dir >= 0)
				{
					// We only move the other faces if we are the current_face (current) face
					if (navigation[drag_dir] != nullptr)
						navigation[drag_dir]->drag(_x - target, _y, t_pos_x, t_pos_y, 0, 0, false);
				}
			}
		}
		else if (drag_lock_y)
		{
			int16_t delta = _y / 2;
			while (abs(_y) > 1)
			{
				_y -= delta;
				delta = _y / 2;
				draw(true);
				if (current_face && drag_dir >= 0)
				{
					// We only move the other faces if we are the current_face (current) face
					if (navigation[drag_dir] != nullptr)
						navigation[drag_dir]->drag(_x, _y - target, t_pos_x, t_pos_y, 0, 0, false);
				}
			}
		}
	}

	// Set the current face back to the 0,0 position in case the drag didn't reach it
	_x = 0;
	_y = 0;
	draw(true);

	drag_lock_x = false;
	drag_lock_y = false;
	drag_dir = -1;
	canvasid = 0;
	is_dragging = false;

	reset_can_swipe_flags();
	return false;
}

void tw_face::draw_children(bool stacked, int16_t stacked_y_start, uint8_t style_hint)
{
	if (stacked)
	{
		int16_t _y = stacked_y_start;
		for (int w = 0; w < widgets.size(); w++)
		{
			widgets[w]->draw(canvasid, 10, _y, style_hint);
			_y -= widgets[w]->get_height();
		}

		for (int w = 0; w < controls.size(); w++)
		{
			controls[w]->draw(canvasid, 10, _y);
			_y -= controls[w]->get_height();
		}
	}
	else
	{
		for (int w = 0; w < widgets.size(); w++)
		{
			widgets[w]->draw(canvasid, style_hint);
		}

		for (int w = 0; w < controls.size(); w++)
		{
			controls[w]->draw(canvasid);
		}
	}
}

void tw_face::draw_children_scroll(int16_t offset_x, int16_t offset_y)
{
	for (int w = 0; w < widgets.size(); w++)
	{
		widgets[w]->draw(canvasid, offset_x, offset_y, 0);
	}

	for (int w = 0; w < controls.size(); w++)
	{
		controls[w]->draw_scroll(canvasid, offset_x, offset_y);
	}
}

void tw_face::set_navigation(tw_face *u, tw_face *r, tw_face *d, tw_face *l)
{
	navigation[0] = u;
	navigation[1] = r;
	navigation[2] = d;
	navigation[3] = l;
}

void tw_face::set_single_navigation(Directions dir, tw_face *face)
{
	navigation[(int)dir] = face;
	int alt_dir = ((int)dir - 2) & 3; // This is like doing modulo 4 - says Michael H - I dont believe him
	if (face != nullptr)
		face->navigation[(int)alt_dir] = this;
}

// tw_face * tw_face::changeFace(Directions dir)
// {
// 	if (dir < 0 || dir > 3)
// 	{
// 		return nullptr;
// 	}

// 	if (navigation[(int)dir] == nullptr)
// 	{
// 		return nullptr;
// 	}

// 	return navigation[(int)dir];
// }

void tw_face::add_widget(tw_widget *widget)
{
	if (widgets.size() < 4)
	{
		widgets.push_back(widget);
		widget->set_parent(this);
	}
}

bool tw_face::widget_process_clicks(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	for (int w = 0; w < widgets.size(); w++)
	{
		if (widgets[w]->click(touch_pos_x, touch_pos_y))
		{
			next_update = 0;
			return true;
		}
	}

	return false;
}

void tw_face::add_control(tw_control *control)
{
	if (controls.size() < 10)
	{
		controls.push_back(control);
		control->set_parent(this);
	}
}

bool tw_face::control_process_clicks(uint16_t touch_pos_x, uint16_t touch_pos_y)
{
	for (int w = 0; w < controls.size(); w++)
	{
		if (controls[w]->click(touch_pos_x, touch_pos_y))
		{
			next_update = 0;
			return true;
		}
	}

	return false;
}

void tw_face::set_scrollable(bool scroll_x, bool scroll_y)
{
	can_scroll_x = scroll_x;
	can_scroll_y = scroll_y;
}

// Calculate the vertical height of all of the widgets connected to this face
uint16_t tw_face::get_widget_height()
{
	uint16_t height = 0;
	for (int i = 0; i < widgets.size(); i++)
		height += widgets[i]->get_height();

	return height;
}

// Calculate the vertical height of all of the controls connected to this face
uint16_t tw_face::get_control_height()
{
	uint16_t height = 0;
	int16_t min_y = 2000;
	int16_t max_y = 0;
	for (int i = 0; i < controls.size(); i++)
	{
		// info_println(controls[i]->name+" "+String(controls[i]->get_height()));
		min_y = min(min_y, controls[i]->get_y_min());
		max_y = max(max_y, controls[i]->get_y_max());

		height += controls[i]->get_height();
	}

	// info_println("calc height: "+String(height)+", actual: "+String(max_y-min_y));
	return (max_y);
}

void tw_face::debug_print()
{
	info_print(name);
	info_print(" - dragging? ");
	info_print(is_dragging);
	info_print(" cached? ");
	info_print(is_cached);
	info_print(" canvis id:?");
	info_println(canvasid);
}

// Check if this face is currently cached
bool tw_face::is_face_cached() { return is_cached; }

// Iterate though all faces as reset their drag and cached state
void tw_face::reset_cache_status()
{
	for (auto _face : faces)
	{
		_face.second->is_dragging = false;
		_face.second->is_cached = false;
	}
}