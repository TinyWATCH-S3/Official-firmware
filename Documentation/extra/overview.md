# Code Structure & Concepts
The main areas of the code project are separated into folders to keep things grouped in a logical way.

Everything you see displayed on the screen of the TinyWATCH happens inside a `FACE`. Faces inherit from the `tw_face` class, which manages the ability for faces to be swiped and navigated to. It also handles drawing of any controls or widgets assigned to a face.

Class Documentation: Detailed breakdowns of the key classes within the firmware, including their members, methods, and purposes.

The main entry point and runtime loop of the firmware lives in [tinywatch.cpp](@ref tinywatch.cpp). This is where the `Arduino setup() and loop()` functions live. 

Main display and touch interaction code lives in [display.cpp](@ref display.cpp)

### Faces
[TW Faces](@ref tw_face.cpp) are where you build your content for TinyWATCH. You can display anything you like using the `draw()` method, as well as assign Widgets and Controls to any face you create.

Faces can be setup to br navigated to each other, via swiping up, down, left and right.

### Widgets
[TW Widgets](@ref tw_widget.cpp) are self contained little "interactive apps" can run logic, or connectivity tasks and show specific info.

They are designed to be reusable, so can be placed on one or more faces.  

### Controls
[TW Controls](@ref tw_control.cpp) are UI elements like buttons, toggles, value sliders etc.
They can be placed on FACES and can also be bound to user settings using the [SettingsOption](@ref settingsOption.cpp) classes.

### Peripherals
Classes for the onboard peripherals are provided as convenient ways to talk to each peripheral library/driver.

[RTC](@ref rtc.cpp)

[IMU + MAG](@ref imu.cpp)

[Battery](@ref battery.cpp)

[Buzzer](@ref buzzer.cpp)


### WiFi
WiFi access setup lives in the [wifi_setup](@ref wifi_setup.cpp) class and general communication lives in the [wifi_controller](@ref wifi_controller.cpp) class.

### User Settings
User Settings are managed in the [settings](@ref settings.cpp) class. They are loaded and saved in the secondary user Flash IC. 

## Coding Style
The source code uses `TABS` and all method and variable names use `snake_case` - Please respect that for any contributions you may make for the project.

## Open Source License
This source code is released under the GPL 3.0 license. Please read the license to understand your obligations if you wish to use this code ibn you own projects, or contribute back to the project.


## Getting Started
Whether you are a developer looking to extend the functionality of the smartwatch or a user curious about the technology behind the device, this documentation is your gateway to unlocking the full potential of the firmware.

Let's embark on this journey of exploration and innovation as we delve into the heart of the TinyWATCH S3 firmware.