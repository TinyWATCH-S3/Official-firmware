
#include "tw_faces/face_Compass.h"
#include "peripherals/imu.h"
#include "fonts/Clock_Digits.h"
#include "fonts/RobotoMono_Regular_All.h"


#define RAD2DEG 0.0174532925
#define BACKGROUND TFT_BLACK


void FaceCompass::setup()
{
	if (!is_setup)
	{
		is_setup = true;
		// Add any one time setup code here
	}
}

void FaceCompass::draw(bool force)
{
	if (force || millis() - next_update > update_period)
	{
		setup();
		
		next_update = millis();

		if (!is_dragging || !is_cached)
		{
			if (is_dragging)
				is_cached = true;

			angle = imu.get_yaw();

			drawCompass(120,140,angle); // Draw centre of compass at 120,140

		}
		
		canvas[canvasid].pushSprite(_x, _y);
	}
}

bool FaceCompass::click(uint pos_x, uint pos_y)
{
	showingGyro = !showingGyro;
	return true;
}

bool FaceCompass::click_double(uint pos_x, uint pos_y)
{
	return false;
}

bool FaceCompass::click_long(uint pos_x, uint pos_y)
{
	return false;
}

void FaceCompass::drawCompass(int x, int y, int angle)
{
 canvas[canvasid].setFreeFont(RobotoMono_Regular[12]);

  // TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
  // this allows it to be used in both 8 and 16 bit colour sprites.
  canvas[canvasid].fillSprite(TFT_TRANSPARENT);

  #define NEEDLE_L 84/2  // Needle length is 84, we want radius which is 42
  #define NEEDLE_W 12/2  // Needle width is 12, radius is then 6

  // Draw the old needle position in the screen background colour so
  // it gets erased on the TFT when the sprite is drawn
  canvas[canvasid].fillTriangle(lx1,ly1,lx3,ly3,lx4,ly4,BACKGROUND);
  canvas[canvasid].fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,BACKGROUND);

  // Set text coordinate datum to middle centre
  canvas[canvasid].setTextDatum(MC_DATUM);
  canvas[canvasid].setTextColor(TFT_WHITE);
  
  canvas[canvasid].drawString("N",120,140-42,2);
  canvas[canvasid].drawString("E",120+42,140,2);
  canvas[canvasid].drawString("S",120,140+42,2);
  canvas[canvasid].drawString("W",120-42,140,2);

  canvas[canvasid].drawCircle(120,140,30,TFT_DARKGREY);

  getCoord(x, y, &lx1, &ly1, NEEDLE_L, angle);
  getCoord(x, y, &lx2, &ly2, NEEDLE_L, angle+180);
  getCoord(x, y, &lx3, &ly3, NEEDLE_W, angle+90);
  getCoord(x, y, &lx4, &ly4, NEEDLE_W, angle-90);
  
  canvas[canvasid].fillTriangle(lx1,ly1,lx3,ly3,lx4,ly4,TFT_RED);
  canvas[canvasid].fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,TFT_LIGHTGREY);

  canvas[canvasid].fillCircle(120, 140 ,3,TFT_DARKGREY);
  canvas[canvasid].fillCircle(120,140,2,TFT_LIGHTGREY);
  
}

// Get coordinates of end of a vector, centre at x,y, length r, angle a
// Coordinates are returned to caller via the xp and yp pointers
void FaceCompass::getCoord(int x, int y, int *xp, int *yp, int r, int a)
{
  float sx1 = cos( (a-90) * RAD2DEG );    
  float sy1 = sin( (a-90) * RAD2DEG );
  *xp =  sx1 * r + x;
  *yp =  sy1 * r + y;
}

FaceCompass face_compass;

