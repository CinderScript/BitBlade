/* ImageData.h
*/

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <cstdint>
#include <cstdlib>

class ImageData {
public:
	ImageData(int width, int height);
	~ImageData();

	int GetWidth() const;
	int GetHeight() const;

	void SetPixel(int x, int y, uint16_t pixel);  // Using uint16_t for RGB565
	void GetPixel(int x, int y, uint16_t& pixel) const;

	const uint16_t* GetBuffer() const;

private:
	int width, height;
	uint16_t* data; // Pointer to the pixel data stored in SDRAM
};

#endif // IMAGE_DATA_H