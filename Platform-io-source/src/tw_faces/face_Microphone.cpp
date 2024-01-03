
#include "tw_faces/face_Microphone.h"
#include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

void FaceMicrophone::setup()
{
	if (!is_setup)
	{
		is_setup = true;

		// don't mess around with this
		i2s_config_t i2s_config = {
			.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
			.sample_rate = SAMPLE_RATE,
			.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
			.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
			.communication_format = I2S_COMM_FORMAT_STAND_I2S,
			.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
			.dma_buf_count = 4,
			.dma_buf_len = BLOCK_SIZE,
			.use_apll = false,
			.tx_desc_auto_clear = false,
			.fixed_mclk = 0,
		};

		// and don't mess around with this
		i2s_pin_config_t i2s_mic_pins = {
			.bck_io_num = I2S_MIC_SERIAL_CLOCK,
			.ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
			.data_out_num = I2S_PIN_NO_CHANGE,
			.data_in_num = I2S_MIC_SERIAL_DATA
		};
		
		// Start up the I2S peripheral
  		i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  		i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);

		info_println("setup complete");
	}
}

void FaceMicrophone::draw(bool force)
{
	setCpuFrequencyMhz(160);
	setup();

	if (force || millis() - next_update > update_period)
	{
		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;


			
			canvas[canvasid].fillSprite(TFT_BLACK);

			if (visual_state == 0)
			{
				do_fft_calcs();
				canvas[canvasid].drawLine(0, 140, 240, 140, TFT_WHITE);
				for (int b = 0; b < 8; b++)
				{
					int8_t bar = bands[b] / 50;
					bar = constrain(bar, 1, 100);
					canvas[canvasid].fillRect(b * 25+22, 40+(100-bar), 20, bar, colors[b]);
					canvas[canvasid].fillRect(b * 25+22, 144, 20, bar, colors_dark[b]);
				}
			}
			else if (visual_state == 1)
			{
				do_fft_calcs();
				for (int b = 0; b < 8; b++)
				{
					int8_t bar = bands[b] / 50;
					int radius = 12 * (b+1) + 4;
					display.fill_arc(canvasid, 120, 140, 0, constrain(bar, 1, 60), radius, radius, 8, colors[b]);
				}
			} 
			else if (visual_state == 2)
			{
				do_waveform_calcs();
				for (size_t i = 1; i < waveform_size; i++) 
				{
					uint8_t y1 = map(waveform_data[i-1], -32768, 32767, 10, display.height - 10);
					uint8_t y2 = map(waveform_data[i], -32768, 32767, 10, display.height - 10);

					// Option A
					// canvas[canvasid].drawLine(1, display.height/2 , display.width, display.height/2, RGB(0x77,0x00,0x00)); // Graticule (looks a bit silly)
					canvas[canvasid].drawLine((i*2), y1, (i*2), y2, colors[i / 15]);
    			}
			}
			else if (visual_state == 3)
			{
				do_waveform_calcs();
				for (size_t i = 1; i < waveform_size; i++) 
				{
					// Scale the Y position
					uint8_t y1 = map(waveform_data[i-1], -32768, 32767, 10, display.height - 10);
					uint8_t y2 = map(waveform_data[i], -32768, 32767, 10, display.height - 10);

					// Option B
					for (uint8_t y_offset = 0; y_offset < 8; y_offset++)
					{
						canvas[canvasid].drawLine((i*2), (y1 - 40) + (10 * y_offset) , (i*2), (y2 - 40) + (10 * y_offset), colors[y_offset]);
					}
    			}
			}
		}
		canvas[canvasid].pushSprite(_x,_y);
	}
}

bool FaceMicrophone::click(uint pos_x, uint pos_y)
{
	visual_state++;
	if (visual_state == 4)
		visual_state = 0;
	return true;
}

bool FaceMicrophone::click_double(uint pos_x, uint pos_y)
{
	return false;
}

bool FaceMicrophone::click_long(uint pos_x, uint pos_y)
{
	return false;
}

void FaceMicrophone::do_fft_calcs()
{
 size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * BLOCK_SIZE, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

  for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
	raw_samples[i] = raw_samples[i] / 10000;
    vReal[i] = raw_samples[i] << 2;
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }

  FFT.Windowing(vReal, BLOCK_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, BLOCK_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, BLOCK_SIZE);

  // Clear bands
  for (int i = 0; i < 8; i++)
    bands[i] = 0;

  // Calculate bands
  for (int i = 2; i < (BLOCK_SIZE / 2); i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
    if (vReal[i] > 2000) { // Add a crude noise filter, 10 x amplitude or more
      if (i <= 3 )             bands[0] = max(bands[0], (int)(vReal[i] / amplitude)); // 125Hz
      if (i > 4   && i <= 6 )   bands[1] = max(bands[1], (int)(vReal[i] / amplitude)); // 250Hz
      if (i > 6   && i <= 8 )   bands[2] = max(bands[2], (int)(vReal[i] / amplitude)); // 500Hz
      if (i > 8   && i <= 15 )  bands[3] = max(bands[3], (int)(vReal[i] / amplitude)); // 1000Hz
      if (i > 15  && i <= 30 )  bands[4] = max(bands[4], (int)(vReal[i] / amplitude)); // 2000Hz
      if (i > 30  && i <= 53 )  bands[5] = max(bands[5], (int)(vReal[i] / amplitude)); // 4000Hz
      if (i > 53  && i <= 140 ) bands[6] = max(bands[6], (int)(vReal[i] / amplitude)); // 8000Hz
      if (i > 140           ) bands[7] = max(bands[7], (int)(vReal[i] / amplitude)); // 16000Hz
    }
  }
}

void FaceMicrophone::do_waveform_calcs()
{
    size_t bytes_read = 0;
    i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * BLOCK_SIZE, &bytes_read, portMAX_DELAY);

	for (int i = 0; i < waveform_size; i++)
	{
		// We just want Amplitude, so this should be sufficient
		uint16_t sample_data = raw_samples[i+2] / 10000;
		waveform_data[i] = (sample_data) << 2;
	}

	// Capture the end waveform state so it can be repeated at the start
	// of the next refresh. (Looks neater)
	waveform_data[0] = waveform_last;
	waveform_last = waveform_data[waveform_size - 1];
}
FaceMicrophone face_microphone;
