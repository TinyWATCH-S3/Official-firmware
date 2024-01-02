# IMU Module Documentation

This module interacts with the Inertial Measurement Unit (IMU) to gather sensor data, track steps, and provide orientation information.

## External Dependencies

- **"imu.h":** Header file for the IMU module.
- **"rtc.h":** Header file for the Real-Time Clock module.
- **"activity.h":** Header file for the activity module.
- **"utilities/logging.h":** Header file for logging utilities.

## External Objects

- **`extern Activity activity;`**
  - External instance of the `Activity` class.
- **`extern RTC rtc;`**
  - External instance of the Real-Time Clock class.

## Class: IMU

### Member Functions

#### `init() -> void`

- Initializes the IMU, BMI270 accelerometer and gyroscope, and MMC5603 magnetometer.
- Configures and reorients the IMU.
- Sets up step counting and wrist wake gesture features.

#### `setup_step_counter() -> void`

- Configures the BMI270 for step counting and sets up interrupts.

#### `setup_wake_gesture() -> void`

- Configures the BMI270 for wrist wake-up gestures and sets up interrupts.

#### `set_hibernate(state: bool) -> void`

- Enables or disables the IMU features (accelerometer and gyroscope) based on the specified state.

#### `process_steps() -> void`

- Processes step counting using interrupts and tracks steps in the `Activity` module.

#### `setup_wake_gesture() -> void`

- Processes wrist gestures.

#### `update() -> void`

- Updates sensor data from the IMU.

#### `get_steps(day: uint8_t, month: uint8_t, year: uint16_t) -> uint32_t`

- Gets the number of steps for a specific day, month, and year from the `Activity` module.

#### `get_movement_activity() -> String`

- Gets the movement activity (e.g., STILL, WALKING, RUNNING) from the IMU.

#### `process_wrist_gestures() -> void`

- Processes wrist gestures (not yet implemented).

#### `get_accel_x() -> float`

- Gets the X-axis accelerometer reading.

#### `get_accel_y() -> float`

- Gets the Y-axis accelerometer reading.

#### `get_accel_z() -> float`

- Gets the Z-axis accelerometer reading.

#### `get_gyro_x() -> float`

- Gets the X-axis gyroscope reading.

#### `get_gyro_y() -> float`

- Gets the Y-axis gyroscope reading.

#### `get_gyro_z() -> float`

- Gets the Z-axis gyroscope reading.

#### `get_pitch() -> float`

- Calculates and gets the pitch angle.

#### `get_roll() -> float`

- Calculates and gets the roll angle.

#### `get_yaw() -> float`

- Calculates and gets the yaw angle using magnetometer readings.

#### `is_looking_at_face() -> bool`

- Checks if the watch is positioned to look at the wearer's face.

### Global Object: imu

- An instance of the `IMU` class for accessing IMU-related functions and data.

### External Libraries

- **Adafruit_BMI270:** Library for the BMI270 accelerometer and gyroscope.
- **Adafruit_MMC5603:** Library for the MMC5603 magnetometer.

### Constants

- **BMI2_AXIS_NEG_X, BMI2_AXIS_NEG_Y, BMI2_AXIS_NEG_Z:** Constants for axis orientation.
- **BMI270_STEP_ACTIVITY_STILL, BMI270_STEP_ACTIVITY_WALKING, BMI270_STEP_ACTIVITY_RUNNING:** Constants for movement activity types.

### External Objects

- **interrupt_happened:** A static boolean indicating whether an interrupt has occurred.

### External Functions

- **step_interrupt():**
  - An interrupt service routine (ISR) for step counting.
- **wake_interrupt():**
  - An ISR for wrist wake-up gesture.

> [Back to menu](tw_docs.md)
