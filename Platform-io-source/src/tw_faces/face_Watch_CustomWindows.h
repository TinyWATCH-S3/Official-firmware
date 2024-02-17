#pragma once

#include "tw_faces/tw_face.h"

class FaceWatch_CustomWindows : public tw_face
{
	public:
		void setup(void);
		void draw(bool force);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_long(uint16_t touch_pos_x, uint16_t touch_pos_y);

	private:
		String version = "3.11";

		struct WindowsClockSettings
		{
				uint16_t width = display.width - 50;
				uint16_t height = display.height - 50;
				uint16_t color_background = RGB(0xc3, 0xc7, 0xcb);		// Light Grey
				uint16_t color_borderedge = RGB(0x00, 0x00, 0x00);		// Black
				uint16_t color_titlebar = RGB(0x00, 0x00, 0xAA);		// Blue
				uint16_t color_titletext = RGB(0xFF, 0xFF, 0xFF);		// Blue
				uint16_t color_buttonshadow = RGB(0x86, 0x8A, 0x8E);	// Dark Grey
				uint16_t color_buttonhighlight = RGB(0xFF, 0xFF, 0xFF); // White
				uint16_t color_displaybkg = RGB(0x11, 0x11, 0x11);		// Black
				uint16_t color_menubar = RGB(0xFF, 0xFF, 0xFF);			// Blue
				uint16_t color_menutext = RGB(0x00, 0x00, 0x00);		// Blue
				uint16_t color_hrminhand = RGB(0x00, 0x00, 0x00);		// Black
				uint16_t color_sechand = RGB(0x88, 0x88, 0x88);			// Grey
				uint16_t color_hourblock = RGB(85, 170, 170);			// Aqua
		};

		const String months[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

		// Settings
		bool show_borders = true;

		// Kept for Caching
		uint8_t hours = 0;
		uint8_t mins = 0;
		uint16_t day = 0;
		uint16_t month = 0;
		uint16_t year = 0;
		int8_t day_offset = -16;
		float bat_perc = 0.0;

		// Windows Clock Stuff
		void setup_trig(void);
		void draw_batteryicon(uint16_t x, uint16_t y, WindowsClockSettings winclock);
		void draw_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h, WindowsClockSettings winclock);
		void draw_window_objects(uint16_t x, uint16_t y, uint16_t w, uint16_t h, WindowsClockSettings winclock);
		void draw_hand(int x, int y, int x1, int y1, uint16_t color);
		bool cachedTrig = false;
		float pos_secs[60][2];
		float pos_mins[60][2];
		float pos_lo[60][2];
		float pos_ro[60][2];
		float pos_mins_rlo[60][2];
		float pos_mins_rro[60][2];
		float pos_hours_rlo[60][2];
		float pos_hours_rro[60][2];
		float pos_hours[60][2];

		// Window Positioning
		int center_x = 120;
		int center_y;
		int center_y_wtoolbar = 157; // Clock Position with Toolbar
		int center_y_ntoolbar = 137; // Clock Position without Toolbar

		float face_radius = 100;

		bool show_toolbars = true;
};

extern FaceWatch_CustomWindows face_watch_custom_windows;
