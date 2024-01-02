# WifiController Class Documentation

## Overview
`WifiController` is a class designed for managing non-blocking WiFi connectivity and HTTP request handling in a separate thead using and incoming task queue and outgoing callback queue.

The idea behind this class is to implement a way to fire off a HTTP or HTTPS request along with a callback, and have the WiFi connection, request and disconnection happen without blocking the main thread. 

## Constructor

### `WifiController()`
Initializes the `WifiController`, creates task and callback queues, and starts the WiFi task.

## Methods

### `void kill_controller_task()`
Terminates the WiFi task, useful for stopping the WiFi functionality when no longer needed, like when the watch is about to do into deep sleep.

### `bool is_busy()`
Returns the busy state of the WiFi controller, indicating if a task is currently being processed, or if the watch is currently connecting to an AP.

### `bool connect()`
Attempts to connect to the WiFi network. Disconnects any existing connection, checks WiFi credentials, and tries to establish a new connection with a 5-second timeout.

If the watch is already connected to a WiFi router, it just exits early.

### `void disconnect()`
Disconnects from the WiFi network and turns off the WiFi module. Disconnect will not be called if `wifi_blocks_display=true` or if `settings.config.wifi_start=true`

### `void loop()`
Processes the callback queue and should be called in the main loop. It checks and executes callback items every 2 seconds.

### `String http_request(const String &url)`
Performs an HTTP request to the given URL and returns the response as a `String`. Supports both HTTP and HTTPS, with a 5-second timeout.

### `void perform_wifi_request(const String &url, _CALLBACK callback)`
Processes a WiFi request using the provided URL and callback, enqueues the result in the callback queue.

### `void add_to_queue(const String &url, _CALLBACK callback)`
Adds a task to the WiFi task queue with the specified URL and callback.

## Private Methods

### `static void wifi_task(void *pvParameters)`
Task function for FreeRTOS, processing items in the WiFi task queue.

## Member Variables

### `QueueHandle_t wifi_task_queue`
Queue for managing WiFi task items.

### `QueueHandle_t wifi_callback_queue`
Queue for managing callback items.

### `TaskHandle_t wifi_task_handler`
Handle for the WiFi task.

