# WifiSetup Documentation

The `WifiSetup` class handles the setup of WiFi AP connections for the TinyWATCH S3 device. It provides an access point (AP) mode for connecting the device to a WiFi network and storing the AP credentials.

## Class Members

### Private Members

- **`bool done`**: Flag indicating if the WiFi setup process is completed.
- **`DNSServer dnsServer`**: DNS server for handling DNS requests during the setup process.
- **`AsyncWebServer webServer`**: Asynchronous web server for handling HTTP requests during the setup process.

### Static Functions

- **`static void htmlEscape(String &str)`**: Static function to escape HTML special characters.

### Structs

#### scan_responder

- **`WifiSetup *server`**: Pointer to the `WifiSetup` instance.
- **`bool waiting`**: Flag indicating if the scan process is waiting for results.
- **`std::string response`**: String containing the HTML response for network scanning.

#### connect_responder

- **`WifiSetup *server`**: Pointer to the `WifiSetup` instance.
- **`bool waiting`**: Flag indicating if the connection process is waiting for results.
- **`unsigned long start`**: Timestamp when the connection process started.
- **`static const int timeout`**: Timeout duration for connection attempts.
- **`std::string response`**: String containing the HTML response for connection attempts.

## Class Methods

### Public Methods

#### `void start()`

- *Description*: Starts the WiFi setup process by configuring the device as an access point and initializing the DNS and web servers.
- *Side Effects*: Updates the `done` flag, starts the AP mode, and initializes the DNS and web servers.
- *Usage*: Call this method to initiate the WiFi setup process.

#### `void stop(bool restart)`

- *Description*: Stops the WiFi setup process and optionally restarts the device.
- *Parameters*:
  - `bool restart`: If true, the device is restarted; otherwise, it returns to the main loop.
- *Side Effects*: Stops the AP mode, shuts down the DNS and web servers.
- *Usage*: Call this method to end the WiFi setup process.

#### `void process()`

- *Description*: Handles DNS requests. Should be called from the main loop to process DNS requests.
- *Side Effects*: Processes DNS requests.
- *Usage*: Include this method in the main loop to handle DNS requests.

## External Dependencies

- `peripherals/buzzer.h`: Buzzer module for providing audio feedback.
- `Arduino.h`: Arduino core library.
- `DNSServer.h`: DNS server library.
- `ESPAsyncWebServer.h`: Asynchronous web server library.
- `WiFi.h`: WiFi library.
- `wifi_setup.h`: Header file for WiFi setup.
- `wifi_setup_templates.h`: Header file containing HTML templates for the WiFi setup UI.
- `display.h`: Display module for handling UI updates.

> [Back to menu](tw_docs.md)
