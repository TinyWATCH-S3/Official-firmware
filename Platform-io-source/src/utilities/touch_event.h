#pragma once

#include "Arduino.h"

enum TouchEventType
{
	TOUCH_TAP = 0,
	TOUCH_DOUBLE = 1,
	TOUCH_LONG = 2,
	TOUCH_SWIPE = 3,
	TOUCH_UNKNOWN = 99,
};

struct touch_event_t
{
		uint16_t x = 0;
		uint16_t y = 0;
		TouchEventType type = TOUCH_UNKNOWN;
		uint8_t dir;
		int16_t d_x = 0;
		int16_t d_y = 0;

	public:
		touch_event_t() : x(0), y(0), type(TOUCH_UNKNOWN), dir(0), d_x(0), d_y(0) {}
		touch_event_t(uint16_t _x, uint16_t _y, TouchEventType _type) : x(_x), y(_y), type(_type), dir(0), d_x(0), d_y(0) {}
		touch_event_t(uint16_t _x, uint16_t _y, TouchEventType _type, uint8_t _dir, int16_t _d_x, int16_t _d_y) : x(_x), y(_y), type(_type), dir(_dir), d_x(_d_x), d_y(_d_y) {}
};