/* PixelOperations.h
*/

#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include "ImageData.h"

class PixelOperations {
public:
	void DrawRectangle(ImageData& imageBuffer, int x, int y, int width, int height, uint16_t color);
	void DrawImage(ImageData& frameBuffer, int x, int y, const ImageData& image);
};

#endif // PIXEL_OPERATIONS_H