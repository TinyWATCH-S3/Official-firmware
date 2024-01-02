## Display Class Documentation

### Overview

The `Display` class is responsible for managing the display and touch functionalities of the TinyWATCH S3 project. It includes methods for initializing the screen, handling touch interactions, managing faces, and controlling backlight levels.

#### Constants

- `RGB(uint8_t r, uint8_t g, uint8_t b) -> constexpr uint16_t`

  - **Description**: Converts RGB888 color values to RGB565 color representation for the display.

#### Enumerations

- `enum Directions`

  - **Description**: Represents directions for touch interactions.

- `enum display_states`

  - **Description**: Represents different states of the display.

### Public Instance Variables

- `tw_face *current_face`

  - **Type**: Pointer to `tw_face`
  - **Description**: Represents the currently active face on the display.

- `bool showing_loading_icon`

  - **Type**: Boolean
  - **Description**: Indicates whether the loading icon is currently displayed.

- `uint16_t loading_screen_rotation`

  - **Type**: 16-bit unsigned integer
  - **Description**: Represents the rotation angle of the loading icon.

- `const uint8_t backlight_settings_vbus[3]`

  - **Type**: Array of 3 constant 8-bit unsigned integers
  - **Description**: Backlight settings for VBUS power source.

- `const uint8_t backlight_settings_vbat[3]`

  - **Type**: Array of 3 constant 8-bit unsigned integers
  - **Description**: Backlight settings for VBAT power source.

- `uint backlight_period_vbus`

  - **Type**: 32-bit unsigned integer
  - **Description**: Backlight period for VBUS power source.

- `uint backlight_period_vbat`

  - **Type**: 32-bit unsigned integer
  - **Description**: Backlight period for VBAT power source.

- `unsigned long next_loading_rotation`

  - **Type**: 64-bit unsigned long
  - **Description**: Represents the time for the next loading icon rotation.

- `uint8_t last_backlight`

  - **Type**: 8-bit unsigned integer
  - **Description**: Represents the last backlight level.

- `uint8_t backlight_level`

  - **Type**: 8-bit unsigned integer
  - **Description**: Represents the current backlight level.

- `uint8_t backlight_current_val`

  - **Type**: 8-bit unsigned integer
  - **Description**: Represents the current value of the backlight.

- `uint8_t backlight_target_val`

  - **Type**: 8-bit unsigned integer
  - **Description**: Represents the target value for the backlight.

- `display_states current_display_state`

  - **Type**: Enumeration (`display_states`)
  - **Description**: Represents the current state of the display.

### Includes

- `Arduino.h`: Arduino core library.
- `TFT_eSPI.h`: TFT display library.
- `cst816t.h`: Capacitive touch library.
- `rtc.h`: Real-Time Clock library.
- `logging.h`: Logging utility.
- `wifi_control.h`: Wi-Fi control library.
- `Tween.h`: Tweening library for smooth transitions.
- `vector`: Standard C++ vector library.

### Class: `Display`

#### Public Methods

##### `init_screen()`

- **Description**: Initializes the screen, touchpad, and display buffers. Creates a separate task (`process_backlight`) for handling backlight control.

##### `createFaces(bool sleeping)`

- **Parameters**:
  - `sleeping`: Indicates whether the device was in sleep mode.
- **Description**: Creates various faces and widgets for different functionalities.

##### `set_backlight(int level)`

- **Parameters**:
  - `level`: The desired backlight level.
- **Description**: Sets the backlight level based on the provided level.

##### `update_current_face()`

- **Description**: Updates the current face on the screen.

##### `check_navigation()`

- **Description**: Handles touch interactions, including dragging, swiping, and navigation between faces.

##### `get_backlight_period()`

- **Description**: Determines the backlight period based on whether the device is on battery or external power.

##### `set_display_state(display_states state, String message)`

- **Parameters**:
  - `state`: The desired display state, e.g., `LOADING`.
  - `message`: Optional message to display along with the state.
- **Description**: Sets the current display state and optionally displays a message.

##### `show_loading_icon()`

- **Description**: Displays a loading icon on the screen.

##### `show_watch_from_boot()`

- **Description**: Displays the watch face after the boot screen.

##### `force_save()`

- **Description**: Forces the saving of settings.

##### `update_boot_face(wifi_states status)`

- **Parameters**:
  - `status`: The Wi-Fi status for updating the boot face.
- **Description**: Updates the boot face with the provided Wi-Fi status.

##### `adjust_backlight()`

- **Description**: Adjusts the backlight level gradually.

##### `get_current_backlight_val()`

- **Description**: Returns the current backlight value.

##### `get_current_display_state()`

- **Description**: Returns the current display state.

#### Private Methods

##### `update_rotation()`

- **Description**: Updates the display rotation based on the configuration settings.

#### Private Properties

- Various private properties for managing the display state, backlight settings, and brightness levels.

### Global Instances

- `tft`: An instance of the `TFT_eSPI` class for driving the TFT display.
- `canvas[]`: An array of two `TFT_eSprite` instances used as display buffers.
- `display`: An instance of the `Display` class, responsible for managing the TinyWATCH S3 display and touch functionalities.


> [Back to menu](tw_docs.md)
