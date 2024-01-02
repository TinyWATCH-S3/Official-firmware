# Battery Module Documentation

This module interacts with the MAX17048 fuel gauge to monitor the battery's state and voltage.

## External Dependencies

- **"peripherals/battery.h":** Header file for the battery module.
- **"utilities/logging.h":** Header file for logging utilities.
- **"tinywatch.h":** Header file for TinyWatch configuration.
- **"Wire.h":** Arduino Wire library for I2C communication.

## Class: Battery

### Member Functions

#### `init() -> bool`

- Initializes the MAX17048 fuel gauge.
- Enables debugging for MAX17048.
- Checks if a battery is connected.
- Configures alert thresholds, state of change alert, high voltage, and low voltage thresholds.
- Returns `true` on successful initialization, otherwise `false`.

#### `set_hibernate(state: bool) -> void`

- Enables or disables hibernate mode on the MAX17048 based on the specified state.

#### `update_stats(forced: bool) -> void`

- Updates battery statistics such as voltage, percentage, and change rate.
- Reads the MAX17048 every 500 milliseconds unless forced.

#### `get_rate(forced: bool) -> float`

- Gets the battery depletion or charging rate.
- Forces an update if specified (`forced`).

#### `get_voltage(forced: bool) -> float`

- Gets the battery voltage.
- Forces an update if specified (`forced`).

#### `get_percent(forced: bool) -> float`

- Gets the battery percentage.
- Forces an update if specified (`forced`).

#### `get_alert_status() -> uint8_t`

- Gets the MAX17048 alert status.
- Prints the alert status.
- Clears the alert status using `clear_alert_status`.
- Returns the alert status.

#### `clear_alert_status(status: uint8_t) -> void`

- Clears the MAX17048 alert status.
- Prints the alert status.

### Global Object: battery

- An instance of the `Battery` class for accessing battery-related functions and data.

### External Libraries

- **SFE_MAX1704X:** SparkFun MAX1704X Library for MAX17048 fuel gauge.

### Constants

- **MAX1704X_MAX17048:** Constant representing the MAX17048 device ID.

### External Objects

- **maxlipo:** An instance of the `SFE_MAX1704X` class for interfacing with the MAX17048.

### External Functions

- **info_print(), info_println():** Logging functions for informational messages.

> [Back to menu](tw_docs.md)
