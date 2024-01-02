# TW Control Class

TW Controls are UI elements, such as buttons, toggles, and value sliders, that can be placed on faces and bound to user settings using the SettingsOption classes.

### Class Members
- **name (String):** The name of the control.
- **value (String):** The current value of the control.
- **option1 (String):** Additional option for the control.
- **option2 (String):** Additional option for the control.
- **pos_x (uint):** X-coordinate of the control on the face.
- **pos_y (uint):** Y-coordinate of the control on the face.
- **width (uint):** Width of the control.
- **height (uint):** Height of the control.
- **padding_x (int):** Horizontal padding of the control.
- **padding_y (int):** Vertical padding of the control.
- **offset_x (uint16_t):** X-offset for scrolling.
- **offset_y (uint16_t):** Y-offset for scrolling.
- **can_scroll_x (bool):** Flag indicating if the control is scrollable in the X-axis.
- **can_scroll_y (bool):** Flag indicating if the control is scrollable in the Y-axis.
- **parent (tw_face*):** Pointer to the parent face.

### Class Methods
- **create(String _name, String _option1, String _option2, uint _pos_x, uint _pos_y, uint _width, uint _height):**
  Initializes the control with the provided parameters.
- **create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height):**
  Initializes the control with the provided parameters.
- **create(String _name, uint _pos_x, uint _pos_y):**
  Initializes the control with the provided parameters.
- **set_callback(CallbackFunction callback):**
  Sets the callback function for the control.
- **set_values(String _option1, String _option2):**
  Sets the option values for the control.
- **set_parent(tw_face* _parent):**
  Sets the parent face of the control.
- **bounds_check(uint click_pos_x, uint click_pos_y) -> bool:**
  Checks if a given position is within the bounds of the control.
- **get_y_min() -> int16_t:**
  Returns the minimum Y-coordinate of the control.
- **get_y_max() -> int16_t:**
  Returns the maximum Y-coordinate of the control.
- **draw_scroll(uint canvasid, uint16_t off_x, uint16_t off_y):**
  Draws the control on a specified canvas with scrolling offsets.
- **draw(uint canvasid, uint _pos_x, uint _pos_y):**
  Draws the control on a specified canvas at the provided position.
- **set_scrollable(bool scroll_x, bool scroll_y):**
  Sets the scrollability of the control.
- **can_drag(uint click_pos_x, uint click_pos_y) -> bool:**
  Checks if the control can be dragged at a given position.
- **get_height_stacked() -> uint16_t:**
  Returns the total stacked height of the control.
- **get_height() -> uint16_t:**
  Returns the height of the control.

## External Dependencies
- **tw_controls/tw_control.h:**
  Header file for the TW Control class.
- **utilities/logging.h:**
  Logging utility header file.

> [Back to menu](tw_docs.md)
