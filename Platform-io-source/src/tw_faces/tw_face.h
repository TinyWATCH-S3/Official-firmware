#pragma once

#include "themes.h"
#include "tw_apps/tw_app.h"
#include "tw_controls/tw_control.h"
#include "tw_widgets/tw_widget.h"
#include "utilities/logging.h"
#include "utilities/touch_event.h"
#include "settings/settings.h"
#include <map>

// Controls
#include "tw_controls/control_Button.h"
#include "tw_controls/control_Label.h"
#include "tw_controls/control_Toggle.h"
#include "tw_controls/control_Value.h"
#include "tw_controls/control_ValueSlider.h"

class tw_face
{
		using CallbackFunction = void (*)();

	public:
		void add(String _name, uint16_t _update_period, uint32_t req_cpu_speed);
		void add(String _name, uint _update_period);
		void add_clock(String _name, uint16_t _update_period, uint32_t req_cpu_speed);
		void add_clock(String _name, uint _update_period);
		void set_navigation(tw_face *l, tw_face *r, tw_face *u, tw_face *d);
		void set_single_navigation(Directions dir, tw_face *face, bool reverse = true);
		// tw_face * changeFace(Directions dir);
		void add_widget(tw_widget *widget);
		bool widget_process_clicks(touch_event_t touch_event);
		void add_control(tw_control *control);
		void add_setting_bool(SettingsOptionBool *sett, uint _pos_x, uint _pos_y, uint _width, uint _height, CallbackFunction callback = nullptr);
		bool control_process_clicks(touch_event_t touch_event);
		bool is_face_clock_face();

		void draw_children(bool stacked, int16_t stacked_y_start);
		void draw_children_scroll(int16_t offset_x, int16_t offset_y);
		uint32_t get_cpu_speed();

		// Dragging
		void drag_begin(int16_t pos_x, int16_t pos_y);
		void drag(int16_t drag_x, int16_t drag_y, int16_t pos_x, int16_t pos_y, int16_t t_pos_x, int16_t t_pos_y, bool current_face);
		bool drag_end(int16_t drag_x, int16_t drag_y, bool current_face, int16_t distance, int16_t t_pos_x, int16_t t_pos_y, int16_t last_dir_x, int16_t last_dir_y);
		int drag_dir = -1;
		void reset_can_swipe_flags();
		int check_can_drag_in_dir();
		void prevent_dragging(bool state);
		bool is_drag_blocked();

		// graphics caching
		void reset_cache_status();
		bool is_face_cached();
		void debug_print();

		// Content scrolling
		void set_scrollable(bool scroll_x, bool scroll_y);
		uint16_t get_widget_height();
		uint16_t get_control_height();

		// control dragging
		tw_control *find_draggable_control(uint16_t touch_pos_x, uint16_t touch_pos_y);

		void update_screen();

		String name = "";

		// virtual methods
		virtual void setup(void) = 0;
		virtual void draw(bool force) = 0;
		virtual bool process_touch(touch_event_t touch_event) { return false; }

		tw_face *navigation[4] = {nullptr, nullptr, nullptr, nullptr};
		void draw_navigation(uint8_t canvasid);

	protected:
		bool is_setup = false;
		uint8_t canvasid = 0;
		uint16_t update_period = 0;
		uint8_t required_cpu_speed = 80;
		bool is_clock_face = false;
		unsigned long next_update = 0;
		unsigned long next_click_update = 0;
		unsigned long click_hold_start_timer = 0;
		bool should_animate = false;
		bool drag_lock_x = false;
		bool drag_lock_y = false;
		bool is_dragging = false;
		bool is_cached = false;
		bool can_drag_dir[4] = {false, false, false, false};
		int16_t drag_start_x = 0;
		int16_t drag_start_y = 0;
		int16_t _x = 0;
		int16_t _y = 0;
		int drag_width = 90;		 // threshold from screen edge to detect a swipe might happen
		bool block_dragging = false; // used for app screens where dragging is not possible (yet, maybe)

		unsigned long drag_start_time = 0;

		// Scrolling content?
		bool can_scroll_x = false;
		bool can_scroll_y = false;
		float inertia_x = 0;
		float inertia_y = 0;
		int16_t scroll_start_y = 0;
		float scroll_pos = 0;
		bool is_scrolling = false;

		// dragging controls?
		tw_control *selectedControl = nullptr;

		std::vector<tw_widget *> widgets;
		std::vector<tw_control *> controls;
};