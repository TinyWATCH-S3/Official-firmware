# Activity Documentation

This module is responsible for managing and storing activity data, including steps tracking and *hopefully in the future* activity sessions like walking or running. 

## Class: Activity

### Member Functions

#### `load() -> bool`

- Loads activity data from the flash storage.
- If no data is found or an error occurs during loading, it creates new data.
- Returns `true` if successful, otherwise `false`.

#### `save(force: bool) -> bool`

- Saves activity data to flash storage.
- It attempts to save data every 5 minutes unless it's a forced save (`force` parameter is true).
- Returns `true` if the data is saved, otherwise `false`.
- If the data is the same as the last saved data, it skips the save operation.

#### `create() -> bool`

- Creates new activity data and saves it to flash storage.
- Returns `true` if successful.

#### `track_steps(steps: uint32_t, day: uint8_t, month: uint8_t, year: uint16_t) -> void`

- Tracks the number of steps for a specific day, month, and year.
- Updates the steps for the specified day, month, and year in the activity data.

### Data Structure: Activity_Data

- Structure representing the activity data.
- Members:
  - `steps_day`: 2D array tracking steps for each day of the month.
  - `steps_year`: Array tracking total steps for each year.
  - `imu_day`, `imu_month`, `imu_year`: Placeholder fields, not further documented.

### Global Object: activity

- An instance of the `Activity` class for accessing activity-related functions and data.

### External Libraries

- **nlohmann::json:** Used for JSON serialization and deserialization.

### External Dependencies

- **"activity.h":** Header file for the activity module.
- **"utilities/logging.h":** Header file for logging utilities.
- **"utilities/flash.h":** Header file for flash storage utilities.

> [Back to menu](tw_docs.md)
