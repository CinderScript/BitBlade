/* PixelOperations.cpp
* windows implementation
*/

#include "PixelOperations.h"
#include "PixelColor.h"

template<typename PixelColor>
void PixelOperations<PixelColor>::DrawRectangle(
	FrameBuffer<PixelColor>& buffer, int x, int y, int width, int height, const PixelColor& color) {

	int bufferWidth = buffer.GetWidth();
	int bufferHeight = buffer.GetHeight();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int pixelX = x + j;
			int pixelY = y + i;

			if (pixelX >= 0 && pixelX < bufferWidth && pixelY >= 0 && pixelY < bufferHeight) {
				buffer.SetPixel(pixelX, pixelY, color);
			}
		}
	}
}

template class PixelOperations<BGRA>;