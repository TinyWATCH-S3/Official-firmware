#include <LittleFS.h>
#include <Arduino.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

class ScreenCapture
{
	public:
		void capture(const char *filename);

	private:
};

void ScreenCapture::capture(const char *filename)
{
}

ScreenCapture scapture;
