#pragma once

#include "tw_faces/tw_face.h"

class FaceTOTP : public tw_face
{
    public:
		void setup(void);
		void draw(bool force);
		bool click(uint pos_x, uint pos_y);
		bool click_double(uint pos_x, uint pos_y);
		bool click_long(uint pos_x, uint pos_y);

    private:
		//
		String totp_token = "";
};

extern FaceTOTP face_totp;
