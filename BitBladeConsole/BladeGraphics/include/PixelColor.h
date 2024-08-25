/* ColorFormat.h
*/

#ifndef PIXEL_COLOR_H
#define PIXEL_COLOR_H

#include <cstdint>

// Pixel format structure definitions
struct RGB888 {
	uint8_t r, g, b;
};

struct RGB565 {
	uint16_t color;
};

// for windows
struct BGRA {
	uint8_t b, g, r, a;
};

#endif // PIXEL_COLOR_H
