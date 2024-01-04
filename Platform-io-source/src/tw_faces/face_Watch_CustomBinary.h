#pragma once

#include "tw_faces/tw_face.h"

class FaceWatch_CustomBinary : public tw_face
{
    public:
		void setup(void);
		void draw(bool force);
		bool click(uint pos_x, uint pos_y);
		bool click_double(uint pos_x, uint pos_y);
		bool click_long(uint pos_x, uint pos_y);

    private:
        String version = "1.0";

		// Binary Watch Colour Pallette - On
		const uint16_t on_colors[9] = 
		{
			RGB(127, 0, 0),
			RGB(127, 63, 0),
			RGB(127, 127, 0),
			RGB(127, 255, 0),
			RGB(0, 255, 0),
			RGB(0, 255, 128),
			RGB(0, 255, 255),
			RGB(0, 128, 255),
			RGB(0, 0, 255),
		};

		// Binary Watch Colour Pallette - Off
		const uint16_t off_colors[9] = 
		{
			RGB(32, 0, 0),
			RGB(32, 24, 0),
			RGB(32, 48, 0),
			RGB(24, 48, 0),
			RGB(0, 48, 0),
			RGB(0, 48, 24),
			RGB(0, 48, 48),
			RGB(0, 24, 48),
			RGB(0, 0, 48),
		};

		const String months[12] = 
		{
			"JAN", "FEB", "MAR",
			"APR", "MAY", "JUN",
			"JUL", "AUG", "SEP",
			"OCT", "NOV", "DEC"
		};
			

		// Kept for Caching
		uint8_t hours = 0;
		uint8_t mins = 0;
		uint16_t day = 0;
		uint16_t month = 0;
		uint16_t year = 0;

		uint16_t m_start = 0;
		uint16_t m_end = 0;

};

extern FaceWatch_CustomBinary face_watch_custom_binary;
