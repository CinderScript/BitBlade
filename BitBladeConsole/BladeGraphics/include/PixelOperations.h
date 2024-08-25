/* PixelOperations.h
*/

#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include "ImageData.h"

template<typename PixelColor>
class PixelOperations {
public:
	void DrawRectangle(ImageData<PixelColor>& data, int x, int y, int width, int height, const PixelColor& color);
};

#endif // PIXEL_OPERATIONS_H

