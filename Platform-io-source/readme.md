# TinyWATCH S3 Open Source Firmware
Welcome to the EARLY PRE ALPHA source code of the TinyWATCH S3 project. This guide provides insights into the intricacies of the firmware that powers the TinyWATCH S3, offering a deeper understanding of the functionalities and inner workings of the smartwatch.

## Project Overview
TinyWATCH S3 is an ESP32-S3 based open source watch/wearable project designed by Unexpected Maker that delivers a hardware and software platform to either build upon, or to alter to suit a myriad of cases.

This device packs in a lot of features, including a vibrant capacitive touch screen display, an I2S MEMS Microphone, 6 Axis IMU, 3 Axis magnetometer, onboard buzzer, battery charging, I2C fuel gauge, secondary SPI Flash for persistent user data plus WiFi and BLE connectivity.

# State Of The Project
I was hoping to have this firmware further along that what it is now before I opened it up to the community, but it's at where's it's at and it's time to let it out... I'm not nervous at all, really. maybe.

There are some obvious missing large ticket items, like WiFi web server based user settings, and/or BLE connectivity, and some of the peripherals are not fully fleshed out or are not working properly - looking at you Mr Magnetometer!

Some of the code is more complex than id like and naughtily shares state/functionality across classes, like the face swiping and navigation system, but it does work well.

# Documentation
There is some early [markdown based docs](tw_docs.md) in the `/docs` folder, though not all of it has been kept up to date with the constantly changing in code, but it's a staring point.

Not all of the code is documented right now. There is a lot of work still to be done and I'd like to eventually move to Sphinx for documentation for the project.

# Contributing To The Project
If you would like to contribute to the firmware (I hope many of you will), please fork the project and have at it.

I have created dedicated area on my [Discord Server](https://discord.gg/ZUfpFUTbda) for firmware related discussions for those contributing. Please let me know what your Discord username is once you've joined my server, and I'll add you to the channel.
