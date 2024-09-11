/* ImageLoader.cpp */

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ImageLoader.h"
#include <stdexcept>

ImageData* ImageLoader::LoadImageData(const std::string& imageName) {

	std::string filePath = "C:/Users/Cinde/source/repos/BitBlade/images/" + imageName;

	int width, height, channels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb);

	if (!data) {
		throw std::runtime_error("Failed to load image: " + filePath);
	}

	// Dynamically allocate an ImageData object (assuming RGB565 format for now, convert the data)
	ImageData* imageData = new ImageData(width, height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * channels;
			uint8_t r = data[index];
			uint8_t g = data[index + 1];
			uint8_t b = data[index + 2];

			// Convert RGB to RGB565
			uint16_t pixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
			imageData->SetPixel(x, y, pixel);
		}
	}

	stbi_image_free(data);
	return imageData;
}

