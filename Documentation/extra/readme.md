# Welcome   {#mainpage}
Welcome to the documentation for the source code of the TinyWATCH S3 project. This guide provides insights into the intricacies of the firmware that powers the TinyWATCH S3, offering a deeper understanding of the functionalities and inner workings of the smartwatch.

This documentation serves as a resource for developers, contributors, and enthusiasts who aim to explore, understand, and potentially contribute to the firmware development of the TinyWATCH S3 project. 

This documentation is a work in progress, and the firmware is in a constant state of change as existing features are refined or re-written, new features are added and well, some features get removed.   

## Project Overview
TinyWATCH S3 is an ESP32-S3 based open source watch/wearable project designed by Unexpected Maker that delivers a hardware and software platform to either build upon, or to alter to suit a myriad of cases.

The hardware packs in a lot of features, including a vibrant capacitive touchscreen display, an I2S MEMS Microphone, 6 Axis IMU, 3 Axis magnetometer, on-board buzzer, battery charging, I2C fuel gauge, secondary SPI Flash for persistent user data plus WiFi and BLE connectivity.

## State Of The Project
I was hoping to have this firmware further along than what it is now before I opened it up to the community, but it's where it's at and it's time to let it out... I'm not nervous at all, really. maybe.

There are some obvious missing large ticket items, like WiFi web server-based user settings, and/or BLE connectivity, and some of the peripherals are not fully fleshed out or are not working properly - looking at you Mr Magnetometer!

Some of the code is more complex than I like and naughtily shares state/functionality across classes, like the face swiping and navigation system, but it does work well.

### Documentation
Not all of the code is documented right now, but it has started, and this Doxygen system will be the base for all future docs.

## Contributing To The Project
If you would like to contribute to the firmware (I hope many of you will), please fork the project and have at it.

I have created a dedicated area on my [Discord Server](https://discord.gg/ZUfpFUTbda) for firmware-related discussions, for those contributing. Please let me know what your Discord username is once you've joined my server, and I'll add you to the channel.
