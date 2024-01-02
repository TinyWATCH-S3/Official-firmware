# Settings Class

## Overview

The `Settings` class is responsible for managing and persisting user settings. It uses JSON for serialization and deserialization and provides functionality for loading, saving, and updating various configuration parameters.

## Dependencies

- `settings/settings.h`
- `utilities/user_flash.h`
- `tinywatch.h`
- `Preferences.h`
- `nlohmann/json.hpp`

## Key Features

- Serialization and deserialization of settings using JSON.
- Non-volatile storage management.
- Handles WiFi credentials, MQTT configuration, battery settings, and more.

## Adding new settings
Basic settings are added to the `Config struct` for runtime use, and if you want them to also be serialied and deserialised to Flash, you need to add them to the `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT` MACRO in `settings.cpp`, otherwise they will be ignored.

Groups of settings for a specific topic, like the settings for MQTT should be added to their own `Config struct` (see `Config_mqtt` below) and should then referenced in the main `Config struct`. They also need to be process independently in a `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT` MACRO to ensure they are also serialied and deserialised.

## Data Structures

### Config_mqtt

Used for MQTT configuration:
- `enabled`
- `broker_ip`
- `broker_port`
- `username`
- `password`
- `device_name`
- `topic`

### Config_battery

Manages battery-related settings:
- `perc_offset`
- `low_perc`
- `low_volt_warn`
- `low_volt_cutoff`

### Config_open_weather

Used for Open Weather settings:
- `api_key`
- `poll_frequency`

### Config

Main configuration structure:
- WiFi settings: `wifi_start`, `wifi_ssid`, `wifi_pass`, `mdns_name`
- MQTT settings
- Battery settings
- Open Weather settings
- Additional settings like `city`, `country`, `utc_offset`, etc.

## Member Functions

### `bool has_wifi_creds()`

Checks if WiFi credentials are present.

### `bool has_country_set()`

Checks if the country setting is set.

### `void update_wifi_credentials(String ssid, String pass)`

Updates WiFi credentials.

### `String get_load_status()`

Retrieves the status of the last load operation.

### `bool load()`

Loads settings from non-volatile memory.

### `long backupNumber(const String filename)`

Returns the backup number from the filename.

### `bool backup()`

Handles the backup of settings.

### `bool save(bool force)`

Saves settings to non-volatile memory.

### `bool create()`

Creates new default settings.

### `void print_file()`

Prints the contents of the settings file.
