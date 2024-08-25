/* ImageData.h
*/

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <cstdint>
#include <cstdlib> // For malloc and free

template<typename PixelColor>
class ImageData {
public:
	ImageData(int width, int height)
		: width(width), height(height) {
		// Dynamically allocate memory in the heap (which should be in SDRAM)
		data = static_cast<PixelColor*>(malloc(width * height * sizeof(PixelColor)));
	}

	~ImageData() {
		// Free the dynamically allocated memory
		free(data);
	}

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	void SetPixel(int x, int y, const PixelColor& pixel) {
		data[y * width + x] = pixel;
	}

	void GetPixel(int x, int y, PixelColor& pixel) const {
		pixel = data[y * width + x];
	}

	const PixelColor* GetBuffer() const {
		return data;
	}

private:
	int width, height;
	PixelColor* data; // Pointer to the pixel data stored in SDRAM
};

#endif // IMAGE_DATA_H
