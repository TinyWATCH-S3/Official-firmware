# SettingsOption Classes

The `SettingsOption` classes are designed to bind user settings from the `Settings` class into `tw_Controls` for the purpose of allowing controls to read and modify the setting values. Different classes are provided for handling integer (`SettingsOptionInt`), float (`SettingsOptionFloat`), boolean (`SettingsOptionBool`), and string (`SettingsOptionString`) settings.

## Class Members

### `SettingsOptionInt`

#### Methods

- **`change(int dir)`**
  - **Parameters**
    - `dir`: Type: `int`. The direction of the change (positive or negative).
  - **Returns**
    - Type: `int`. The updated setting value after the change.
  - **Description**: Changes the integer setting value based on the specified direction (`dir`). Optionally wraps around if the `value_wrap` property is set.

- **`get()`**
  - **Returns**
    - Type: `int`. The current integer setting value.
  - **Description**: Retrieves the current integer setting value.

- **`get_str()`**
  - **Returns**
    - Type: `String`. The current integer setting value converted to a string.
  - **Description**: Retrieves the current integer setting value as a string.

### `SettingsOptionFloat`

#### Methods

- **`change(int dir)`**
  - **Parameters**
    - `dir`: Type: `int`. The direction of the change (positive or negative).
  - **Returns**
    - Type: `float`. The updated setting value after the change.
  - **Description**: Changes the float setting value based on the specified direction (`dir`). Optionally wraps around if the `value_wrap` property is set.

- **`get()`**
  - **Returns**
    - Type: `float`. The current float setting value.
  - **Description**: Retrieves the current float setting value.

- **`get_str()`**
  - **Returns**
    - Type: `String`. The current float setting value converted to a string.
  - **Description**: Retrieves the current float setting value as a string.

### `SettingsOptionBool`

#### Methods

- **`change()`**
  - **Returns**
    - Type: `bool`. The updated boolean setting value after the change.
  - **Description**: Inverts the boolean setting value.

- **`get()`**
  - **Returns**
    - Type: `bool`. The current boolean setting value.
  - **Description**: Retrieves the current boolean setting value.

- **`get_str()`**
  - **Returns**
    - Type: `String`. The current boolean setting value converted to a string.
  - **Description**: Retrieves the current boolean setting value as a string.

### `SettingsOptionString`

#### Methods

- **`change(String *val)`**
  - **Parameters**
    - `val`: Type: `String*`. The new string value for the setting.
  - **Description**: Changes the string setting value to the provided value.

- **`get()`**
  - **Returns**
    - Type: `String`. The current string setting value.
  - **Description**: Retrieves the current string setting value.

> [Back to menu](tw_docs.md)
