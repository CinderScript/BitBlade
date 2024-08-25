/* PixelOperations.h
*/

#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include "FrameBuffer.h"

template<typename PixelColor>
class PixelOperations {
public:
	void DrawRectangle(FrameBuffer<PixelColor>& buffer, int x, int y, int width, int height, const PixelColor& color);
};

#endif // PIXEL_OPERATIONS_H

