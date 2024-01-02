#include "tw_controls/tw_control.h"

class ControlLabel : public tw_control
{
	public:
	    void draw(uint canvasid);
		bool drag(int16_t drag_x, int16_t drag_y);
		void drag_end();
		bool click(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_double(uint16_t click_pos_x, uint16_t click_pos_y);
		bool click_long(uint16_t click_pos_x, uint16_t click_pos_y);

	private:
		String version = "1.0";

};
