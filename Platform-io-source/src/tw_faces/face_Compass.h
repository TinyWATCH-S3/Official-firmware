#pragma once

#include "tw_faces/tw_face.h"

class FaceCompass : public tw_face
{
    public:
		void setup(void);
		void draw(bool force);
		bool click(uint pos_x, uint pos_y);
		bool click_double(uint pos_x, uint pos_y);
		bool click_long(uint pos_x, uint pos_y);

		void drawCompass(int x, int y, int angle);
		void getCoord(int x, int y, int *xp, int *yp, int r, int a);

    private:
        String version = "1.0";
		bool showingGyro = false;

		int number = 0;
		int angle  = 0;

		int lx1 = 0;
		int ly1 = 0;
		int lx2 = 0;
		int ly2 = 0;
		int lx3 = 0;
		int ly3 = 0;
		int lx4 = 0;
		int ly4 = 0;

		// Test only
		uint16_t  n = 0;
		uint32_t dt = 0;

};

extern FaceCompass face_compass;
