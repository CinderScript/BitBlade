/* ImageData.cpp
*/

#include "ImageData.h"

ImageData::ImageData(int width, int height)
	: width(width), height(height), data(nullptr) {

	data = static_cast<uint16_t*>(malloc(width * height * sizeof(uint16_t)));
}

ImageData::~ImageData() {
	free(data);
}

int ImageData::GetWidth() const {
	return width;
}

int ImageData::GetHeight() const {
	return height;
}

void ImageData::SetPixel(int x, int y, uint16_t pixel) {
	data[y * width + x] = pixel;
}

void ImageData::GetPixel(int x, int y, uint16_t& pixel) const {
	pixel = data[y * width + x];
}

const uint16_t* ImageData::GetBuffer() const {
	return data;
}
