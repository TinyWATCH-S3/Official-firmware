# RTC Peripheral

## Global Variables

- **interruptTriggered:** A boolean variable indicating whether an interrupt has been triggered.
- **ntpServer:** A constant string containing the NTP (Network Time Protocol) server address.

## String Arrays

- **days:** An array of strings representing the days of the week.
- **months:** An array of strings representing the months of the year.

## Class: RTC

### Member Functions

#### `init()`

- Initializes the RTC module. If initialization fails, an error message is printed.
- Sets the global variable `enabled` to true.
- Checks if an interrupt has been triggered and sets `interruptTriggered` accordingly.
- Checks if NTP synchronization is required based on the current year.

#### `set_time_from_NTP(int16_t utc_offset)`

- Sets the time of the RTC module based on NTP synchronization with an optional UTC offset.
- If synchronization fails, it sets a default date and time.

#### `set_hourly_alarm(uint minutes)`

- Sets up an hourly alarm using the RTC module.

#### `get_hours_string(bool padded, bool is24hour)`

- Gets the current hour as a string, optionally padded or in 24-hour format.

#### `get_mins_string(bool padded)`

- Gets the current minute as a string, optionally padded.

#### `get_secs_string(bool padded)`

- Gets the current second as a string, optionally padded.

#### `get_time_string(bool padded, bool is24hour)`

- Gets the current time as a formatted string, optionally padded or in 24-hour format.

#### `get_hours()`

- Gets the current hour.

#### `get_mins()`

- Gets the current minute.

#### `get_seconds()`

- Gets the current second.

#### `get_day()`

- Gets the current day of the month.

#### `get_month()`

- Gets the current month.

#### `get_year()`

- Gets the current year.

#### `get_step_date(uint16_t &day, uint16_t &month, uint16_t &year)`

- Gets the current date, updating cached values every 5 seconds.

#### `setup_interrupt()`

- Sets up the interrupt for the RTC module.

#### `get_day_date()`

- Gets the current date in the format "Day Month DayOfMonth Year."

#### `get_day_of_week()`

- Gets the current day of the week.

#### `get_month_date()`

- Gets the current month and day.

#### `check_interrupt()`

- Checks if an interrupt has been triggered, particularly an alarm interrupt. If true, it clears the interrupt.

### External Functions

#### `rtcInterrupt()`

- An external interrupt service routine (ISR) that sets `interruptTriggered` to true when called.

#### `RTC rtc;`

- An instance of the RTC class, for accessing RTC functions throughout the program.

> [Back to menu](tw_docs.md)
