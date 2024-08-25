/* FrameBuffer.h
*/

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <cstdint>
#include <cstdlib> // For malloc and free

template<typename PixelColor>
class FrameBuffer {
public:
	FrameBuffer(int width, int height)
		: width(width), height(height) {
		// Dynamically allocate memory in the heap (which should be in SDRAM)
		buffer = static_cast<PixelColor*>(malloc(width * height * sizeof(PixelColor)));
	}

	~FrameBuffer() {
		// Free the dynamically allocated memory
		free(buffer);
	}

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	void SetPixel(int x, int y, const PixelColor& pixel) {
		buffer[y * width + x] = pixel;
	}

	void GetPixel(int x, int y, PixelColor& pixel) const {
		pixel = buffer[y * width + x];
	}

	const PixelColor* GetBuffer() const {
		return buffer;
	}

private:
	int width, height;
	PixelColor* buffer; // Pointer to the pixel data stored in SDRAM
};

#endif // FRAME_BUFFER_H
