#include "settings/settings.h"
#include "tw_apps/app_Microphone.h"
// #include "fonts/RobotoMono_Light_All.h"
#include "fonts/RobotoMono_Regular_All.h"

/**
 * @brief Called the first time an app is opened
 *
 * Put any one off setup code in here
 *
 */
void AppMicrophone::setup()
{
	if (!is_setup)
	{
		is_setup = true;

		// and don't mess around with this
		i2s_pin_config_t i2s_mic_pins = {
			.mck_io_num = -1,
			.bck_io_num = I2S_MIC_SERIAL_CLOCK,
			.ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
			.data_out_num = I2S_PIN_NO_CHANGE,
			.data_in_num = I2S_MIC_SERIAL_DATA
		};

		// Start up the I2S peripheral
		i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
		i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);

		info_println("setup complete");

		sweep_size = settings.config.app_microphone.sweep_size;
		gain_factor = settings.config.app_microphone.gain_factor;

		if (sweep_size == 0) // First config setting. Make sure we aren't set to zero
		{
			sweep_size = 1;
			settings.config.app_microphone.sweep_size = 1;
			settings.config.app_microphone.gain_factor = 0;
		}
	}
}

/**
 * @brief Put anything in here that you want to have run every time the app is opened
 *
 * This is not the same as setup() above that only ever gets called the first time the app opens
 *
 */
void AppMicrophone::pre_start() { shutdown_timer = millis(); }

/**
 * @brief Put anything in here that you want to have run when the app is opened
 *
 */
void AppMicrophone::pre_close()
{
	// i2s_stop(I2S_NUM_0);
	// i2s_driver_uninstall(I2S_NUM_0);
	is_setup = false;
}

/**
 * @brief Draw the icon that gets shown on the app menu face
 *
 * Icons are 64x64 with rounded corners as per the code below, but the inner content can be anything that represents the app well
 * @param canvasid
 * @param _pos_x
 * @param _pos_y

 */
void AppMicrophone::draw_icon(uint8_t canvasid, int16_t _pos_x, int16_t _pos_y)
{
	if (!is_icon_cached)
	{
		is_icon_cached = true;
		icon_sprite.fillSprite(themes.current().col_background_dull);
		icon_sprite.drawSmoothRoundRect(0, 0, 10, 6, icon_width, icon_width, RGB(0x00, 0x00, 0x99), 0);
		icon_sprite.setTextDatum(4); // Middle, Center
		icon_sprite.setFreeFont(RobotoMono_Regular[15]);
		icon_sprite.setTextColor(TFT_WHITE);
		icon_sprite.drawString("M", icon_width / 2, icon_width / 2);
	}

	icon_x = _pos_x;
	icon_y = _pos_y;
	icon_sprite.pushToSprite(&canvas[canvasid], _pos_x, _pos_y);
}

/**
 * @brief Usual draw code that draws the apps content, like a face draw it's content
 *
 * @param force
 */
void AppMicrophone::draw(bool force)
{
	// Override the CPU settings for this app
	setCpuFrequencyMhz(required_cpu_speed);

	setup();

	// Example of how to auto shut down the app after a period of time
	//
	// if (millis() - shutdown_timer > 5000)
	// {
	// 	// close without saving settings
	// 	close(false);
	// 	return;
	// }

	if (force || millis() - next_update > update_period)
	{
		next_update = millis();
		canvas[canvasid].fillSprite(TFT_BLACK);

		// Init text
		canvas[canvasid].setFreeFont(RobotoMono_Regular[16]);

		canvas[canvasid].setTextDatum(BC_DATUM);

		if (visual_state == 0) // Spectrum Bar
		{
			do_fft_calcs();
			canvas[canvasid].drawLine(0, 140, 240, 140, TFT_WHITE);
			for (int b = 0; b < 8; b++)
			{
				int8_t bar = bands[b] / 50;
				bar = constrain(bar, 1, 100);
				canvas[canvasid].fillRect(b * 25 + 22, 40 + (100 - bar), 20, bar, colors[b]);
				canvas[canvasid].fillRect(b * 25 + 22, 144, 20, bar, colors_dark[b]);
			}
		}
		else if (visual_state == 1) // Spectrum Arc
		{
			do_fft_calcs();
			for (int b = 0; b < 8; b++)
			{
				int8_t bar = bands[b] / 50;
				int radius = 12 * (b + 1) + 4;
				display.fill_arc(canvasid, 120, 140, 0, constrain(bar, 1, 60), radius, radius, 8, colors[b]);
			}
		}
		else if (visual_state == 2) // Waveform
		{
			do_waveform_calcs();
			for (size_t i = 0; i < waveform_size - 1; i++)
			{
				uint8_t y1 = waveform_data[i];
				uint8_t y2 = waveform_data[i + 1];
				for (int j = 0; j < sweep_size; j++)
				{
					canvas[canvasid].drawLine((i * 2), y1 + j, (i * 2), y2 + j, colors[i / 15]);
				}
			}
		}
		else if (visual_state == 3) // Multi-Waveform
		{
			do_waveform_calcs();
			for (size_t i = 0; i < waveform_size - 1; i++)
			{
				uint8_t y1 = waveform_data[i];
				uint8_t y2 = waveform_data[i + 1];
				;
				for (uint8_t y_offset = 0; y_offset < 8; y_offset++)
				{
					for (int j = 0; j < sweep_size; j++)
					{
						canvas[canvasid].drawLine((i * 2), (y1 - 40) + (10 * y_offset) + j, (i * 2), (y2 - 40) + (10 * y_offset) + j, colors[y_offset]);
					}
				}
			}
		}
	}

	// Do Text if needed
	if (is_texting)
	{
		text_fader--;
		canvas[canvasid].setTextColor(RGB(text_fader * 4, text_fader * 4, text_fader * 4), RGB(0x00, 0x00, 0x00));
		canvas[canvasid].drawString(text_indicator, display.center_x, display.height - (5 + text_fader));
		if (text_fader <= 0)
			is_texting = false;
	}

	canvas[canvasid].pushSprite(_x, _y);
}

bool AppMicrophone::process_touch(touch_event_t touch_event)
{
	if (touch_event.type == TOUCH_TAP)
	{
		visual_state++;
		if (visual_state == 4)
			visual_state = 0;

		return true;
	}
	else if (touch_event.type == TOUCH_SWIPE)
	{
		if (touch_event.dir == TOUCH_SWIPE_RIGHT)
		{
			if (sweep_size < SWEEP_MAX)
			{
				sweep_size++;
				text_indicator = "Size " + String(sweep_size);
				settings.config.app_microphone.sweep_size = sweep_size;
				is_texting = true;
				text_fader = 64;
			}
		}
		else if (touch_event.dir == TOUCH_SWIPE_LEFT)
		{
			if (sweep_size > 1)
			{
				sweep_size--;
				text_indicator = "Size " + String(sweep_size);
				settings.config.app_microphone.sweep_size = sweep_size;
				is_texting = true;
				text_fader = 64;
			}
		}
		else if (touch_event.dir == TOUCH_SWIPE_DOWN)
		{
			if (gain_factor > 0)
			{
				gain_factor--;
				text_indicator = "Gain " + String(gain_factor);
				settings.config.app_microphone.gain_factor = gain_factor;
				is_texting = true;
				text_fader = 64;
			}
		}
		else if (touch_event.dir == TOUCH_SWIPE_UP)
		{
			if (gain_factor < 19)
			{
				gain_factor++;
				text_indicator = "Gain " + String(gain_factor);
				settings.config.app_microphone.gain_factor = gain_factor;
				is_texting = true;
				text_fader = 64;
			}
		}
		settings.save(false);
		return true;
	}
	return false;
}

/**
 * @brief Calculate the FFT stuff to use to display the VU meter
 *
 */
void AppMicrophone::do_fft_calcs()
{
	size_t bytes_read = 0;
	i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * BLOCK_SIZE, &bytes_read, portMAX_DELAY);
	int samples_read = bytes_read / sizeof(int32_t);

	for (uint16_t i = 0; i < BLOCK_SIZE; i++)
	{
		raw_samples[i] = raw_samples[i] / 20000;
		vReal[i] = raw_samples[i] << 2;
		vImag[i] = 0.0; // Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
	}

	FFT.Windowing(vReal, BLOCK_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
	FFT.Compute(vReal, vImag, BLOCK_SIZE, FFT_FORWARD);
	FFT.ComplexToMagnitude(vReal, vImag, BLOCK_SIZE);

	// Clear bands
	for (int i = 0; i < 8; i++)
		bands[i] = 0;

	// Calculate bands
	for (int i = 2; i < (BLOCK_SIZE / 2); i++)
	{ // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
		if (vReal[i] > 2000)
		{ // Add a crude noise filter, 10 x amplitude or more
			if (i <= 3)
				bands[0] = max(bands[0], (int)(vReal[i] / amplitude)); // 125Hz
			if (i > 4 && i <= 6)
				bands[1] = max(bands[1], (int)(vReal[i] / amplitude)); // 250Hz
			if (i > 6 && i <= 8)
				bands[2] = max(bands[2], (int)(vReal[i] / amplitude)); // 500Hz
			if (i > 8 && i <= 15)
				bands[3] = max(bands[3], (int)(vReal[i] / amplitude)); // 1000Hz
			if (i > 15 && i <= 30)
				bands[4] = max(bands[4], (int)(vReal[i] / amplitude)); // 2000Hz
			if (i > 30 && i <= 53)
				bands[5] = max(bands[5], (int)(vReal[i] / amplitude)); // 4000Hz
			if (i > 53 && i <= 140)
				bands[6] = max(bands[6], (int)(vReal[i] / amplitude)); // 8000Hz
			if (i > 140)
				bands[7] = max(bands[7], (int)(vReal[i] / amplitude)); // 16000Hz
		}
	}
}

/**
 * @brief Calculate the waveform data to display various waveforms
 *
 */
void AppMicrophone::do_waveform_calcs()
{
	size_t bytes_read = 0;
	i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * BLOCK_SIZE, &bytes_read, portMAX_DELAY);
	int samples_read = bytes_read / sizeof(int32_t);

	// We just want Amplitude, so this should be sufficient

	uint16_t sample_data = 0;
	int i = 0;
	int x_offset = 10;

	for (int j = 0; j < samples_read; j++)
	{
		if (i > waveform_size)
			break;

		// Gain Adjustment
		uint16_t sample_data = raw_samples[j] / (20000 - (gain_factor * 1000));
		if (sample_data > 0)
		{
			if (x_offset == 0)
			{
				waveform_data[i] = map(sample_data << 2, -32768, 32767, 10, display.height - 10);
				i++;
			}
			else
			{
				// Skip the first few samples as they have visually unappealing artefacts
				x_offset--;
			}
		}
	}

	// Capture the end waveform state so it can be repeated at the start
	// of the next refresh. (Looks neater)
	waveform_data[0] = waveform_last;
	waveform_last = waveform_data[waveform_size];
}

AppMicrophone app_microphone;