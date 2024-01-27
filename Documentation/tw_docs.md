# Introduction to TinyWATCH S3 Firmware Docs
Welcome to the documentation for the source code of the TinyWATCH S3 project. This guide provides insights into the intricacies of the firmware that powers the TinyWATCH S3, offering a deeper understanding of the functionalities and inner workings of the smartwatch.

## Project Overview
TinyWATCH S3 is an ESP32-S3 based open source watch/wearable project designed by Unexpected Maker that delivers a hardware and software platform to either build upon, or to alter to suit a myriad of cases.

This device packs in a lot of features, including a vibrant capacitive touch screen display, an I2S MEMS Microphone, 6 Axis IMU, 3 Axis magentometer, onboard buzzer, battery charging, I2C fuel gauge, plus WiFi and BLE connectivity. 

## Purpose of the Documentation
This documentation serves as a resource for developers, contributors, and enthusiasts who aim to explore, understand, and potentially contribute to the firmware development of the TinyWATCH S3 project. 

This documentation is a work in progress, and the firmware is in a constant state of change as existing features are refined or re-written, new features are added and well, some features get removed.   

## Contents
Class Documentation: Detailed breakdowns of the key classes within the firmware, including their members, methods, and purposes.

The main entry point and runtime loop of the firmware lives in [tinywatch.cpp](tw_docs_tinywatch.md). This is where the `Arduino setup() and loop()` functions live. 

Main display and touch interaction code ives in [display.cpp](tw_docs_display.md)

#### Faces
[TW Faces](tw_docs_faces.md) are where you build your content for TinyWATCH. You can display anything you like using th `draw()` method, as well as assign Widgets and Controls to any face you create.

Faces can be setup to allow navigation between them, via swiping up, down, left and right.

#### Widgets
[TW Widgets](tw_docs_widgets.md) are self contained little "interactive apps" can run logic, or connectivity tasks and show specific info.

They are designed to be reusable, so can be placed on one or more faces.  

#### Controls
[TW Controls](tw_docs_controls.md) are UI elements like buttons, toggles, value sliders etc.
They can be placed on FACES and can also be bound to user settings using the [SettingsOption](tw_docs_settingsoptions.md) classes.

#### Peripherals
Classes for the onboard peripherals are provided as convenient ways to talk to each peripheral library/driver.
[RTC](tw_docs_peripherals_rtc.md)
[ IMU + MAG](tw_docs_peripherals_imu.md)
[Battery](tw_docs_peripherals_battery.md)
[Buzzer](tw_docs_peripherals_buzzer.md)

#### WiFi
WiFi access setup lives in the [wifi_setup](tw_docs_wifi_setup.md) class and general communication lives in the [wifi_controller](tw_docs_wifi_controller.md) class.

#### User Settings
User Settings are managed in the [settings](tw_docs-settings.md) class. They are loaded and saved in the secondary user Flash IC. 

## Coding Style
The source code uses `TABS` and all method and variable names use `snake_case` - Please respect that for any contributions you may make for the project.

## Open Source License
This source code is released under the GPL 3.0 license. Please read the license to understand your obligations if you wish to use this code ibn you own projects, or contribute back to the project.


## Getting Started
Whether you are a developer looking to extend the functionality of the smartwatch or a user curious about the technology behind the device, this documentation is your gateway to unlocking the full potential of the TinyWATCH S3 firmware.

Let's embark on this journey of exploration and innovation as we delve into the heart of the TinyWATCH S3 firmware.
