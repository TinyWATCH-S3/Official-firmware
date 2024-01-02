#tw_face

## Overview

The `tw_face` class represents a face on the TinyWatch and includes methods for handling touch events, dragging, and managing controls on the face.

## Class Members

- `static std::vector<tw_face *> faces`: A static vector that holds instances of the `tw_face` class.
- `String name`: The name of the face.
- `uint update_period`: The update period for the face.
- `uint32_t required_cpu_speed`: The required CPU speed for the face.
- `std::vector<tw_control *> controls`: A vector to store controls associated with the face.
- `bool can_scroll_y`: A flag indicating whether the face can scroll in the y-direction.
- `int16_t _x, _y`: Current x and y positions of the face.
- `int16_t drag_start_x, drag_start_y`: Initial touch position for dragging.
- `bool drag_lock_x, drag_lock_y`: Flags to lock dragging along the x or y-axis.
- `bool can_swipe_dir[4]`: Flags indicating whether the face can be swiped in a particular direction.
- `int drag_dir`: Direction of the drag (0: up, 1: left, 2: down, 3: right).
- `bool is_dragging, is_scrolling`: Flags indicating whether the face is currently being dragged or scrolled.
- `int16_t scroll_start_y`: Initial y position for scrolling.
- `float scroll_pos`: Current scroll position.
- `int16_t drag_width`: Width of the drag-sensitive area.

## Methods

### `void add(String _name, uint _update_period, uint32_t req_cpu_speed)`

Adds a new face to the `faces` vector with the specified name, update period, and required CPU speed.

### `void reset_can_swipe_flags()`

Resets the flags indicating whether the face can be swiped in any direction.

### `uint32_t get_cpu_speed()`

Returns the required CPU speed for the face.

### `int check_can_swipe()`

Checks if the face can be swiped in any direction and returns the corresponding direction.

### `tw_control * find_draggable_control(int16_t click_pos_x, int16_t click_pos_y)`

Finds a draggable control at the specified click position.

### `void drag_begin(int16_t pos_x, int16_t pos_y)`

Initializes dragging by setting the start time, start position, and identifying the selected control.

### `void drag(int16_t drag_x, int16_t drag_y, int16_t pos_x, int16_t pos_y, int16_t t_pos_x, int16_t t_pos_y, bool current_face)`

Handles dragging behavior, including scrolling, swiping, and updating the face positions.

# tw_face.cpp - Part 2

## Methods (Continued)

### `bool tw_face::drag_end(int16_t drag_x, int16_t drag_y, bool current_face, int16_t distance, bool double_click, int16_t t_pos_x, int16_t t_pos_y, int16_t last_dir_x, int16_t last_dir_y)`

Handles the end of the drag operation, determining whether to switch faces, spring back, or perform other actions based on the drag parameters. It also processes long clicks, double clicks, and regular clicks.

### `void tw_face::draw_children(bool stacked, int16_t stacked_y_start)`

Draws the child widgets and controls of the face. If `stacked` is true, it arranges them in a stacked layout starting from the specified y-coordinate. Otherwise, it draws them without stacking.

### `void tw_face::draw_children_scroll(int16_t offset_x, int16_t offset_y)`

Draws the child widgets and controls of the face with the specified scrolling offsets.

### `void tw_face::set_navigation(tw_face *u, tw_face *r, tw_face *d, tw_face *l)`

Sets the neighboring faces for navigation in the up, right, down, and left directions.

### `void tw_face::set_single_navigation(Directions dir, tw_face *face)`

Sets a single navigation direction with the specified face. It also sets the reciprocal navigation for the other face.

### `tw_face * tw_face::changeFace(Directions dir)`

Changes the current face in the specified direction (`dir`). Returns the new face if the navigation in that direction exists; otherwise, returns nullptr.

### `void tw_face::add_widget(tw_widget *widget)`

Adds a widget to the face, setting its parent to the current face.

### `bool tw_face::widget_process_clicks(uint click_pos_x, uint click_pos_y)`

Processes clicks for widgets on the face, returning true if a widget is clicked.

### `void tw_face::add_control(tw_control *control)`

Adds a control to the face, setting its parent to the current face.

### `bool tw_face::control_process_clicks(uint click_pos_x, uint click_pos_y)`

Processes clicks for controls on the face, returning true if a control is clicked.

### `void tw_face::set_scrollable(bool scroll_x, bool scroll_y)`

Sets whether the face is scrollable in the x and y directions.

### `uint16_t tw_face::get_widget_height()`

Calculates and returns the total height of all widgets on the face.

### `uint16_t tw_face::get_control_height()`

Calculates and returns the total height of all controls on the face, considering the minimum and maximum y-coordinates of the controls.

### `void tw_face::debug_print()`

Prints debug information about the face, including its name, dragging status, caching status, and canvas ID.

### `bool tw_face::is_face_cached()`

Returns a boolean indicating whether the face is marked as cached.

### `void tw_face::reset_cache_status()`

Resets the caching status for all faces. It iterates through all faces, setting their dragging and caching statuses to false.

> [Back to menu](tw_docs.md)
