# TW Widget Class

TW Widgets are self-contained interactive apps designed for reuse across different watch faces. They can perform logic, connectivity tasks, and display specific information.

An example of a widget could be a weather app that connects to OpenWeather, downloads information, and displays it on the watch face. 

Clicking on the widget could switch between showing the current temperature, humidity, chance of rain, etc.

A `Widget` differs from a `Control` in that a control is a single-use interactive item bound to a setting or callback on a specific face. You can place the same widget on multiple faces, each in a different location.

### Class Members
- **`name`** (Type: `String`): The name of the widget.
- **`update_period`** (Type: `uint`): The update period of the widget.
- **`pos_x`** (Type: `uint`): The x-coordinate position of the widget.
- **`pos_y`** (Type: `uint`): The y-coordinate position of the widget.
- **`width`** (Type: `uint`): The width of the widget.
- **`height`** (Type: `uint`): The height of the widget.
- **`parent`** (Type: `tw_face*`): A pointer to the parent face of the widget.

### Class Methods
#### `create(String _name, uint _pos_x, uint _pos_y, uint _width, uint _height, uint _update_period)`

- Initializes the widget with the provided parameters:
  - `_name`: Name of the widget.
  - `_pos_x`, `_pos_y`: X and Y coordinates of the widget.
  - `_width`, `_height`: Width and height of the widget.
  - `_update_period`: Update period of the widget.

#### `set_parent(tw_face *_parent)`

- Sets the parent face of the widget.

#### `bounds_check(uint click_pos_x, uint click_pos_y) -> bool`

- Checks if a given position is within the bounds of the widget.
- Parameters:
  - `click_pos_x`: X-coordinate of the click position.
  - `click_pos_y`: Y-coordinate of the click position.
- Returns `true` if the position is within bounds; otherwise, `false`.

#### `get_height() -> uint`

- Returns the total height of the widget, including padding.

#### `draw(uint canvasid, uint _pos_x, uint _pos_y)`

- Draws the widget on the specified canvas at the provided position.
- Parameters:
  - `canvasid`: Identifier of the canvas.
  - `_pos_x`, `_pos_y`: X and Y coordinates to draw the widget.

> [Back to menu](tw_docs.md)
