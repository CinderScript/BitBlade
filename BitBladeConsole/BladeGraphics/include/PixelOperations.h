/* PixelOperations.h
*/

#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include "ImageData.h"

class PixelOperations {
public:
	void DrawRectangle(ImageData& imageData, int x, int y, int width, int height, uint16_t color);

};

#endif // PIXEL_OPERATIONS_H

