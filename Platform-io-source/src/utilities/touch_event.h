#pragma once

#include "Arduino.h"

enum TouchEventType
{
	TOUCH_TAP = 0,
	TOUCH_DOUBLE = 1,
	TOUCH_LONG = 2,
	TOUCH_SWIPE = 3,
	TOUCH_UNKNOWN = 4,
};

enum TouchSwipeDirection
{
	TOUCH_SWIPE_UP = 0,
	TOUCH_SWIPE_RIGHT = 1,
	TOUCH_SWIPE_DOWN = 2,
	TOUCH_SWIPE_LEFT = 3,
	TOUCH_SWIPE_UNKNOWN = 4,
};

struct touch_event_t
{
		uint16_t x = 0;
		uint16_t y = 0;
		TouchEventType type = TOUCH_UNKNOWN;
		TouchSwipeDirection dir;
		int16_t d_x = 0;
		int16_t d_y = 0;

	public:
		touch_event_t() : x(0), y(0), type(TOUCH_UNKNOWN), dir(TOUCH_SWIPE_UNKNOWN), d_x(0), d_y(0) {}
		touch_event_t(uint16_t _x, uint16_t _y, TouchEventType _type) : x(_x), y(_y), type(_type), dir(TOUCH_SWIPE_UNKNOWN), d_x(0), d_y(0) {}
		touch_event_t(uint16_t _x, uint16_t _y, TouchEventType _type, TouchSwipeDirection _dir, int16_t _d_x, int16_t _d_y) : x(_x), y(_y), type(_type), dir(_dir), d_x(_d_x), d_y(_d_y) {}
};