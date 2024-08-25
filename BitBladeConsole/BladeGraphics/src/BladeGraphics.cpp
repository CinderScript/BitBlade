/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "DisplayDriver.h"
#include "PixelOperations.h"
#include "FrameBuffer.h"
#include "PixelColor.h"



int BladeGraphics::TestFrame() {
	const int screenWidth = 1024;
	const int screenHeight = 600;

	// Create a frame buffer with BGRA format
	FrameBuffer<BGRA> frameBuffer(screenWidth, screenHeight);

	// Define the color for the rectangle using the BGRA format
	BGRA rectangleColor = { 0, 0, 255, 255 }; // Red color in BGRA format (B=0, G=0, R=255, A=255)

	// Calculate the position and size of the rectangle to center it
	int rectWidth = 200;
	int rectHeight = 100;
	int rectX = (screenWidth - rectWidth) / 2;
	int rectY = (screenHeight - rectHeight) / 2;

	// Create a PixelOperations instance for BGRA format and draw the rectangle
	PixelOperations<BGRA> pixelOps;
	pixelOps.DrawRectangle(frameBuffer, rectX, rectY, rectWidth, rectHeight, rectangleColor);

	// Render the buffer on the screen using DisplayDriver
	DisplayDriver displayDriver;
	displayDriver.Render(frameBuffer); // Render the buffer on the screen

	return 123; // Return a test value
}

