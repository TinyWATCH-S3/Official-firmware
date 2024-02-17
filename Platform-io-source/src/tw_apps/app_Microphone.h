#pragma once

#include "tw_apps/tw_app.h"
#include <arduinoFFT.h>
#include <driver/i2s.h>
#include <soc/i2s_reg.h>

#define SAMPLE_BUFFER_SIZE 128
#define SAMPLE_RATE 16000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_2		// blc
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_3 // ws
#define I2S_MIC_SERIAL_DATA GPIO_NUM_1
#define BLOCK_SIZE 512
#define DEG2RAD 0.0174532925

class AppMicrophone : public tw_app
{
	public:
		// virtual methods
		void setup(void);
		void pre_start(void);
		void draw(bool force);
		void draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y, uint8_t style_hint);
		bool click(uint16_t touch_pos_x, uint16_t touch_pos_y);
		bool click_double(uint16_t touch_pos_x, uint16_t touch_pos_y);

		// local methods
		void do_fft_calcs();
		void do_waveform_calcs();
		void fill_arc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);

	private:
		int bands[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		uint8_t waveform_size = 120;
		int16_t waveform_data[200] = {0};
		int16_t waveform_last = 0;
		// const int BLOCK_SIZE = 512;
		double vReal[BLOCK_SIZE];
		double vImag[BLOCK_SIZE];
		int32_t raw_samples[BLOCK_SIZE];

		const double signalFrequency = 1000;
		const double samplingFrequency = 10000;
		const uint8_t amplitude = 150;

		const i2s_port_t I2S_PORT = I2S_NUM_0;

		uint8_t visual_state = 0;

		bool should_calc = true;

		unsigned long shutdown_timer = 0;

		const uint16_t colors[9] = {
			RGB(255, 0, 0),
			RGB(255, 128, 0),
			RGB(255, 255, 0),
			RGB(128, 255, 0),
			RGB(0, 255, 0),
			RGB(0, 255, 128),
			RGB(0, 255, 255),
			RGB(0, 128, 255),
			RGB(0, 0, 255),
		};

		const uint16_t colors_dark[9] = {
			RGB(128, 0, 0),
			RGB(128, 64, 0),
			RGB(128, 128, 0),
			RGB(64, 128, 0),
			RGB(0, 128, 0),
			RGB(0, 128, 64),
			RGB(0, 128, 128),
			RGB(0, 64, 128),
			RGB(0, 0, 128),
		};
		arduinoFFT FFT = arduinoFFT();
};

extern AppMicrophone app_microphone;
