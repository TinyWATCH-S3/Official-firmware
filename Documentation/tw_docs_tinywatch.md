## TinyWATCH Class Documentation

### Overview

This is the main firmware starting point. It contains the `Arduino setup() and loop()` functions.

### External Dependencies

- **Libraries:**
  - `SPI`, `Wire`: Standard libraries for SPI and I2C communication.
  - `SdFat`: Library for SD card file system operations.
  - `PubSubClient`: Library for MQTT communication.
  - `TFT_eSPI`: Library for driving TFT displays.
  - `WiFi`, `ESPAsyncWebServer`: Libraries for Wi-Fi and asynchronous web server functionality.

### Hardware Configuration

- **Pin Configuration:**
  - `VBUS_SENSE`, `BUZZER`, `PWR_SHUTDOWN`, `USER_FLASH_CS`, `TFT_LED`: Pin definitions for various hardware components.

### CPU Speeds

- `CPU_LOW_FREQ`, `CPU_HIGH_FREQ`, `CPU_LOW_FREQ_WIFI`, `CPU_HIGH_FREQ_WIFI`: Definitions for CPU frequencies.

### Wake Reasons

- `WAKE_REASON_TOUCH`, `WAKE_REASON_RTC_ALARM`, `WAKE_REASON_FG_ALERT`, `WAKE_REASON_BMI270_WRIST`: Definitions for different wake-up reasons.

### TinyWATCH Class

#### CPU Frequency Control

- **Methods:**
  - `set_cpu_frequency(uint32_t freq, CPU_SPEED speed)`: Sets the CPU frequency based on the provided frequency and speed.
  - `was_sleeping()`: Checks if the watch was in sleep mode.
  - `woke_by()`: Determines the wake-up reason.

#### Sleep and Power Management

- **Methods:**
  - `go_to_sleep()`: Puts the watch into sleep mode.
  - `vbus_present()`: Checks if VBUS (power supply) is present.

### Setup Function

- Initializes various components, including pins, peripherals, and settings.

### Loop Function

- Handles continuous tasks such as navigation, display updates, and saving settings.

### Additional Information

- The code includes functionality for handling Wi-Fi setup, display initialization, and interacting with peripherals like the buzzer, battery, IMU, and RTC.

> [Back to menu](tw_docs.md)

