#pragma once

#include <initializer_list>
#include <stdint.h>

struct BuzzerNote
{
    int16_t freq;     // Hz
    int16_t time;     // ms
};

void init_buzzer(int pin, int pwmChannel);
void deinit_buzzer(int pin);
void Buzzer(std::initializer_list<BuzzerNote> notes);
void BuzzerUI(std::initializer_list<BuzzerNote> notes);
