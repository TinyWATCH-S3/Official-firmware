//
//  Copyright 2020 M Hotchin.
//  Released under the MIT license.
//  Code repository:
//
//  Run Length Encoded (RLE) bitmaps.  Each run is encoded as either one or two bytes,
//  with NO PADDING.  Thus, the data for each line of the bitmap is VARIABLE LENGTH, and
//  there is no way of determining where any line other than the first starts without
//  walking though the data.  
//
//  Note that one byte encoding ONLY occurs if the total number of colors is 16 or less,
//  and in that case the 'flags' member of the 'RLEBitmapInfo' will have the first bit
//  set.
//
//  In that case, if the high 4 bits of the first byte are ZERO, then this is a 2 byte
//  run.  The first byte is the index of the color in the color palette, and the second
//  byte is the length.
//
//  Else, the lower 4 bits are the color index, and the upper 4 bits are the run length.
//
//  If the 'flags' member first bit is zero, then ALL runs are 2 byte runs.  The first
//  byte is the palette index, and the second is the run length.
//
//  In order to save PROGMEM for other uses, the bitmap data is placed in a section that
//  occurs near the END of the used FLASH.  So, this data should only be accessed using
//  the 'far' versions of the progmem functions - the usual versions are limited to the
//  first 64K of FLASH.
//

#include <TFT_eSPI.h> // Include the TFT_eSPI library


#ifndef _RLEBITMAP_h
#define _RLEBITMAP_h

#if not defined PROGMEM_LATE
#if defined ARDUINO_ARCH_AVR
#define PROGMEM_LATE __attribute__ (( __section__(".fini1") ))
#else
#define PROGMEM_LATE PROGMEM
#endif
#endif

//
//  Since the bitmaps live in PROGMEM_LATE, the far addresses of the bitmap data and of
//  the palette can not be generated at link time.  Each bitmap will have a corresponding
//  function to be called that returns the information required to render it, as contained
//  in this structure.
//
struct RLEBitmapInfo
{
	uint32_t pRLEBM_data_far;    //  Far addresses are untyped 32 bit integers.
	uint32_t pRLEBM_palette_far; //
	unsigned int width;          //  Width and height of the bitmap
	unsigned int height;
	byte flags;
};


//  Adaptor class for different kinds of graphics contexts.  
class RLEGraphicsContext
{
public:
	//  These have the same semantics as the same functions in the Adafruit GFX class.  If
	//  your context does not support a transaction style API, the first two can be
	//  defined to do nothing and do all the work in writeFastHLine().
	virtual void startWrite() = 0;
	virtual void endWrite() = 0;
	virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) = 0;


protected:
	~RLEGraphicsContext() {};

};

void renderRLEBitmap(
	const RLEBitmapInfo &bitmapInfo, // Bitmap information retreived previously.
	int16_t x, int16_t y,            // Location to render bitmap (top-left corner)
	RLEGraphicsContext *pGFX,        // Graphics context for rendering.
	bool blackIsTransparent = false, // if 'true', black pixels in the bitmap are skipped.
	uint8_t reduction = 1);          // Factor to reduce the size of the bitmap


bool                                  // false if the bitmap and mask are different sizes.
renderRLEBitmapWithRLEMask(           //
	const RLEBitmapInfo &bitmapInfo,  // Bitmap information retreived previously.
	const RLEBitmapInfo &maskInfo,    // Bitmap that acts as a mask.
	int16_t x, int16_t y,             // Location to render bitmap (top-left corner)
	RLEGraphicsContext *pGFX,         // Graphics context for rendering.
	bool blackIsTransparent = false); // if 'true', black pixels in the bitmap are skipped.

	

//  Support for the 'TFT_eSPI' library.
#ifdef _TFT_eSPIH_

class TFTeSPIGraphicsContext : public RLEGraphicsContext
{
private:
	TFT_eSPI *m_pGFX;

public:
	TFTeSPIGraphicsContext(TFT_eSPI *pGFX) : m_pGFX(pGFX) {};

	virtual void startWrite() { m_pGFX->startWrite(); };
	virtual void endWrite() { m_pGFX->endWrite(); };
	virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
	{ m_pGFX->drawFastHLine(x, y, w, color); };
};

//  These are provided for backwards compatibility with the first release.
inline void
renderRLEBitmap(
	const RLEBitmapInfo &bitmapInfo, // Bitmap information retreived previously.
	int16_t x, int16_t y,            // Location to render bitmap (top-left corner)
	TFT_eSPI *pGFX,                  // Graphics context for rendering.
	bool blackIsTransparent = false, // if 'true', black pixels in the bitmap are skipped.
	uint8_t reduction = 1)           // Factor to reduce the size of the bitmap
{
	TFTeSPIGraphicsContext context(pGFX);
	renderRLEBitmap(bitmapInfo, x, y, &context, blackIsTransparent, reduction);
}

inline bool                          // false if the bitmap and mask are different sizes.
renderRLEBitmapWithRLEMask(          //
	const RLEBitmapInfo &bitmapInfo, // Bitmap information retreived previously.
	const RLEBitmapInfo &maskInfo,   // Bitmap that acts as a mask.
	int16_t x, int16_t y,            // Location to render bitmap (top-left corner)
	TFT_eSPI *pGFX,                  // Graphics context for rendering.
	bool blackIsTransparent = false) // if 'true', black pixels in the bitmap are skipped.
{
	TFTeSPIGraphicsContext context(pGFX);
	return renderRLEBitmapWithRLEMask(bitmapInfo, maskInfo, x, y, &context, blackIsTransparent);
}
#endif


#endif

