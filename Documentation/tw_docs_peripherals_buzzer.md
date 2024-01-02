# Buzzer Module Documentation

The Buzzer module is responsible for handling sound functionality, allowing the system to play different notes and melodies.

## External Dependencies

- **"buzzer.h":** Header file for the buzzer module.
- **Arduino.h:** Arduino Core library for general Arduino functionalities.
- **"settings/settings.h":** Header file for system settings.
- **TaskScheduler:** Arduino TaskScheduler library for task scheduling.
- **QueueHandle_t:** FreeRTOS library for task communication.

## Global Variables

- **pwmChannel:** Global variable representing the PWM channel for the buzzer.
- **noteQueue:** Global variable representing the FreeRTOS queue for storing buzzer notes.
- **buzzerTaskHandle:** Global variable representing the handle for the buzzer task.

## Functions

### `init_buzzer(pin: int, pwmChannel: int) -> void`

- Initializes the buzzer with the specified pin and PWM channel.
- Attaches the buzzer to the specified pin.
- Creates a FreeRTOS queue for storing buzzer notes.
- Creates a buzzer task using `buzzerTask` function.

### `deinit_buzzer(pin: int) -> void`

- Deinitializes the buzzer.
- Deletes the buzzer task.
- Detaches the buzzer from the specified pin.

### `buzzerTask(param: void) -> void`

- Buzzer task function that continuously checks for notes to play.
- Plays notes from the queue based on their frequency and duration.
- Cancels the current note if a cancellation note is received.

### `queueNotes(notes: std::initializer_list<BuzzerNote>) -> void`

- Resets the note queue and sends a cancellation note to stop the current note.
- Enqueues a list of buzzer notes to the note queue.
- Sends an off note to stop the buzzer after playing the specified notes.

### `Buzzer(notes: std::initializer_list<BuzzerNote>) -> void`

- Plays a sequence of buzzer notes if audio alarm is enabled in system settings.

### `BuzzerUI(notes: std::initializer_list<BuzzerNote>) -> void`

- Plays a sequence of buzzer notes if audio UI is enabled in system settings.

## Struct: BuzzerNote

Represents a single note for the buzzer.

- **`freq: int`**: Frequency of the note in Hertz. Set to -1 for a cancellation note.
- **`time: TickType_t`**: Duration of the note in milliseconds.

## Global Object: settings

An instance of the `Settings` class for accessing system settings.

## Usage

```cpp
// Example usage of Buzzer
Buzzer({{440, 500}, {523, 500}, {587, 500}});  // Plays three notes with frequencies and durations.
```
> [Back to menu](tw_docs.md)
