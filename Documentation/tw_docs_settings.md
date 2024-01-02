# Settings Class

The `Settings` class is responsible for managing and persisting user settings. It provides functionality for loading, saving, and updating various configuration parameters.

## Adding new settings
Settings are added to the `Config struct` for runtime use, and if you want them to also be serialied and deserialised to Flash, you need to add them to the `JSON DEFAULT` list of items in `settings.cpp`, otherwise they will be ignored.

```cpp
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config,
wifi_start, wifi_ssid, wifi_pass, mdns_name, mqtt_enabled,
mqtt_broker_ip, mqtt_broker_port, mqtt_username, mqtt_password,
mqtt_device_name, mqtt_topic, time_24hour, show_digital,
left_handed, flipped, audio_ui, audio_alarm, imu_process_steps,
imu_process_wrist, city, country, ow_api_key, utc_offset);
```

## Class Members 

### Methods

#### `bool has_wifi_creds(void)`

- **Returns**
  - Type: `bool`. Indicates whether the Wi-Fi credentials are available.

#### `bool has_country_set(void)`

- **Returns**
  - Type: `bool`. Indicates whether the country is set in the configuration.

#### `void update_wifi_credentials(String ssid, String pass)`

- **Parameters**
  - `ssid`: Type: `String`. The new Wi-Fi SSID.
  - `pass`: Type: `String`. The new Wi-Fi password.
- **Description**: Updates the Wi-Fi credentials in the configuration and saves the changes.

#### `bool load()`

- **Returns**
  - Type: `bool`. Indicates the success of loading settings.
- **Description**: Loads settings from the flash memory. If no data is available or an error occurs, it creates new data.

#### `long backupNumber(const String filename)`

- **Parameters**
  - `filename`: Type: `String`. The name of the file.
- **Returns**
  - Type: `long`. The backup number parsed from the filename, or 0 if not in the correct format.
- **Description**: Parses the backup number from the given filename.

#### `bool backup()`

- **Returns**
  - Type: `bool`. Indicates the success of the backup operation.
- **Description**: Creates a backup of the settings file. Currently commented out and needs reimplementation.

#### `bool save(bool force)`

- **Parameters**
  - `force`: Type: `bool`. Indicates whether to force the save even if not enough time has passed.
- **Returns**
  - Type: `bool`. Indicates the success of the save operation.
- **Description**: Saves the configuration to the flash memory. It respects the maximum time between saves to avoid frequent writes.

#### `bool create()`

- **Returns**
  - Type: `bool`. Indicates the success of creating new data.
- **Description**: Creates new default data for settings and saves it.

#### `void print_file(void)`

- **Description**: Prints the content of the settings file.

### Variables

#### `settings`

- Type: `Settings`. An instance of the `Settings` class used for managing user settings.

> [Back to menu](tw_docs.md)
