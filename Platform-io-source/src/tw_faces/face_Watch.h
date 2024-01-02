#pragma once

#include "tw_faces/tw_face.h"

enum clock_face
{
    clock_face_analog = 0,
    clock_face_digital = 1,
    // New entries here for alternate face designs

    // last element here has to be called clock_face_element_count and must equal the number of elements without this element.
    // Cheat way to know the number of elements in the enum for iteration.
    clock_face_element_count = 2
};

class FaceWatch : public tw_face
{
    public:
		void setup(void);
		void draw(bool force);
		bool click(uint pos_x, uint pos_y);
		bool click_double(uint pos_x, uint pos_y);
		bool click_long(uint pos_x, uint pos_y);

    private:

		void setup_trig(void);
		void draw_hand(int x, int y, int x1, int y1, uint16_t color);
        bool check_clock_face(clock_face design);
        String version = "1.0";
		bool cachedTrig = false;

		float pos_secs[60][2];
		float pos_mins[60][2];
		float pos_hours[12][2];

		int center_x = 120;
		int center_y = 131;
		float face_radius = 107;
};

extern FaceWatch face_watch;
