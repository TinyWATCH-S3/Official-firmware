//
//  Copyright 2020 M Hotchin.
//  Released under the MIT license.
//  Code repository:
//

#include <Arduino.h>

#include "RLEBitmap.h"

//void Dump(uint32_t address)
//{
//	Serial.printf("%08x ", address);
//	for (int i = 0; i < 16; i++)
//	{
//		byte b = pgm_read_byte_far(address);
//		address++;
//		Serial.printf(" %02x", b);
//
//	}
//	Serial.println();
//}


//
//  By creating two different functions to handle reading RLE data, we don't have to keep
//  checking in the inner loop if the RLE data needs to be read one way or the other - the
//  decision is the same for any given RLE bitmap.
//
void GetRLEShortPalette(
	uint32_t &pData,      //  In, Out - location to read RLE data from, updated
	uint16_t &runLength,  //  Out - the length of the next run.
	uint16_t &colorIndex) //  Out - the *index* (into the color palette) of the next color.
{
	colorIndex = pgm_read_byte_far(pData);
	pData++;

	runLength = colorIndex & 0xF0;

	if (runLength != 0)
	{
		runLength = runLength >> 4;
		colorIndex &= 0x0f;
	}
	else
	{
		runLength = pgm_read_byte_far(pData);
		pData++;
	}
}

void GetRLELongPalette(
	uint32_t &pData,
	uint16_t &runLength,
	uint16_t &colorIndex)
{
	colorIndex = pgm_read_byte_far(pData);
	pData++;
	runLength = pgm_read_byte_far(pData);
	pData++;
}


void
renderRLEBitmap(
	const RLEBitmapInfo &bitmapInfo,
	int16_t x, int16_t y,
	RLEGraphicsContext *pGFX,
	bool blackIsTransparent,
	uint8_t reduction)
{
	uint32_t pRLEData = bitmapInfo.pRLEBM_data_far;

	uint16_t xPos = x;
	uint16_t yPos = y;

	uint16_t xOffset = 0;
	uint16_t yOffset = 0;

	//  Function pointer.
	void (*pRleReader)(uint32_t & pData,
		uint16_t & runLength,
		uint16_t & colorIndex) = nullptr;

	//  Decide which RLE reader to use.
	if (bitmapInfo.flags & 0x01)
		pRleReader = GetRLEShortPalette;
	else
		pRleReader = GetRLELongPalette;

	pGFX->startWrite();
	{
		while (yOffset < bitmapInfo.height)
		{
			uint16_t remainder = 0;

			while (xOffset < bitmapInfo.width)
			{
				uint16_t runLength = 0;
				uint16_t colorIndex = 0;
				(*pRleReader)(pRLEData, runLength, colorIndex);

				//  Write out every 'n'th line to the screen.
				if (yOffset % reduction == 0)
				{
					uint32_t address = bitmapInfo.pRLEBM_palette_far + sizeof(uint16_t) * colorIndex;
					uint16_t color = pgm_read_word_far(address);

					uint16_t newLength = runLength + remainder;

					if (!blackIsTransparent || color != 0)
					{
						pGFX->writeFastHLine(xPos + (xOffset / reduction), yPos + (yOffset / reduction),
							newLength / reduction, color);
					}

					remainder = newLength % reduction;
				}

				xOffset += runLength;
			}

			yOffset++;
			xOffset = 0;
		}
	}
	pGFX->endWrite();
}




bool
renderRLEBitmapWithRLEMask(
	const RLEBitmapInfo &bitmapInfo,
	const RLEBitmapInfo &maskInfo,
	int16_t x, int16_t y,
	RLEGraphicsContext *pGFX,
	bool blackIsTransparent)
{
	if ((bitmapInfo.width != maskInfo.width) || (bitmapInfo.height != maskInfo.height))
	{
		return false;
	}

	//  Arduino far addresses are just untyped 32-bit integers.
	uint32_t pBitmapRLE = bitmapInfo.pRLEBM_data_far;
	uint32_t pMaskRLE = maskInfo.pRLEBM_data_far ;

	const uint16_t xPosEnd = x + bitmapInfo.width;
	const uint16_t yPosEnd = y + bitmapInfo.height;

	uint16_t xPos = x;
	uint16_t yPos = y;

	void (*pBitmapRleReader)(uint32_t & pData,
		uint16_t & runLength,
		uint16_t & colorIndex) = nullptr;

	void (*pMaskRleReader)(uint32_t & pData,
		uint16_t & runLength,
		uint16_t & colorIndex) = nullptr;

	if (bitmapInfo.flags & 0x01)
		pBitmapRleReader = GetRLEShortPalette;
	else
		pBitmapRleReader = GetRLELongPalette;

	if (maskInfo.flags & 0x01)
		pMaskRleReader = GetRLEShortPalette;
	else
		pMaskRleReader = GetRLELongPalette;


	uint16_t bitmapRunLength, bitmapColorIndex;
	uint16_t maskRunLength, maskColorIndex;

	(*pBitmapRleReader)(pBitmapRLE, bitmapRunLength, bitmapColorIndex);
	(*pMaskRleReader)(pMaskRLE, maskRunLength, maskColorIndex);

	uint16_t colorBitmap = pgm_read_word_far(bitmapInfo.pRLEBM_palette_far + sizeof(uint16_t) * bitmapColorIndex);
	uint16_t colorMask = pgm_read_word_far(maskInfo.pRLEBM_palette_far + sizeof(uint16_t) * maskColorIndex);
	
	pGFX->startWrite();
	{
		while (yPos < yPosEnd)
		{
			while (xPos < xPosEnd)
			{
				uint16_t length = min(bitmapRunLength, maskRunLength);

				if (colorMask != 0)
				{
					if (colorBitmap != 0 || !blackIsTransparent)
					{
						pGFX->writeFastHLine(xPos, yPos, length, colorBitmap);
					}
				}
				else // Use non-masking color to avoid block erase flicker
				{
					if (!blackIsTransparent)
					{
						pGFX->writeFastHLine(xPos, yPos, length, colorMask);
					}
				}

				xPos += length;

				bitmapRunLength -= length;
				maskRunLength -= length;

				if (bitmapRunLength == 0)
				{
					(*pBitmapRleReader)(pBitmapRLE, bitmapRunLength, bitmapColorIndex);
					colorBitmap = pgm_read_word_far(bitmapInfo.pRLEBM_palette_far + sizeof(uint16_t) * bitmapColorIndex);
				}

				if (maskRunLength == 0)
				{
					(*pMaskRleReader)(pMaskRLE, maskRunLength, maskColorIndex);
					colorMask = pgm_read_word_far(maskInfo.pRLEBM_palette_far + sizeof(uint16_t) * maskColorIndex);
				}

			}

			yPos++;
			xPos = x;
		}
	}
	pGFX->endWrite();


	return true;
}



