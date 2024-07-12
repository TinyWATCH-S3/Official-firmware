#include "buzzer.h"
#include "settings/settings.h"
#include <Arduino.h>

static int pwmChannel;
static QueueHandle_t noteQueue;
static TaskHandle_t buzzerTaskHandle;

static void buzzerTask(void *param);

void init_buzzer(int pin, int pwmChannel)
{
	::pwmChannel = pwmChannel;
	ledcAttachPin(pin, pwmChannel);
	noteQueue = xQueueCreate(10, sizeof(BuzzerNote));
	xTaskCreate(buzzerTask, "buzzer", configMINIMAL_STACK_SIZE * 3, nullptr, 5, &buzzerTaskHandle);
}

void deinit_buzzer(int pin)
{
	vTaskDelete(buzzerTaskHandle);
	ledcDetachPin(pin);
}

static void buzzerTask(void *param)
{
	BuzzerNote note;
	TickType_t noteEndTime = 0;
	while (true)
	{
		// End time reached?
		if (xTaskGetTickCount() >= noteEndTime)
		{
			// Wait indefinitely for the next note, then play it
			if (xQueueReceive(noteQueue, &note, portMAX_DELAY) && note.freq != -1)
			{
				ledcWriteTone(pwmChannel, note.freq);
				noteEndTime = xTaskGetTickCount() + note.time;
			}
		}
		else
		{
			// Peek the front of the queue to check for a cancellation note (-1, -1)
			if (xQueuePeek(noteQueue, &note, 0) && note.freq == -1)
			{
				noteEndTime = 0;
			}
			else
			{
				vTaskDelay(5);
			}
		}
	}
}

static void queueNotes(std::initializer_list<BuzzerNote> notes)
{
	xQueueReset(noteQueue);
	BuzzerNote cancel = {-1, -1};
	xQueueSend(noteQueue, &cancel, 0);
	for (auto note : notes)
	{
		xQueueSend(noteQueue, &note, 0);
	}
	BuzzerNote off = {0, 0};
	xQueueSend(noteQueue, &off, 0);
}

void Buzzer(std::initializer_list<BuzzerNote> notes)
{
	if (settings.config.audio.alarm)
		queueNotes(notes);
}

void BuzzerUI(std::initializer_list<BuzzerNote> notes)
{
	if (settings.config.audio.ui)
		queueNotes(notes);
}
