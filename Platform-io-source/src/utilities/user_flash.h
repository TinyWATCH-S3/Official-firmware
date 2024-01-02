#pragma once

#include <FS.h>

class UserFlash
{
	public:
		bool init();
		bool deinit();
		bool is_busy();
};

extern FS flashFS;
extern UserFlash user_flash;
