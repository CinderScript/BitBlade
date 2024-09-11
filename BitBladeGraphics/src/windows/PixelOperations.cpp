/* PixelOperations.cpp
* windows implementation
*/

#include "PixelOperations.h"

void PixelOperations::DrawRectangle(
	ImageData& imageBuffer, int x, int y, int width, int height, uint16_t color) {

	int bufferWidth = imageBuffer.GetWidth();
	int bufferHeight = imageBuffer.GetHeight();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int pixelX = x + j;
			int pixelY = y + i;

			if (pixelX >= 0 && pixelX < bufferWidth && pixelY >= 0 && pixelY < bufferHeight) {
				imageBuffer.SetPixel(pixelX, pixelY, color);
			}
		}
	}
}

void PixelOperations::DrawImage(ImageData& frameBuffer, int x, int y, const ImageData& image) {
	int imageWidth = image.GetWidth();
	int imageHeight = image.GetHeight();

	for (int i = 0; i < imageHeight; ++i) {
		for (int j = 0; j < imageWidth; ++j) {
			int pixelX = x + j;
			int pixelY = y + i;
			if (pixelX >= 0 && pixelX < frameBuffer.GetWidth() && pixelY >= 0 && pixelY < frameBuffer.GetHeight()) {
				uint16_t pixel;
				image.GetPixel(j, i, pixel);
				frameBuffer.SetPixel(pixelX, pixelY, pixel);
			}
		}
	}
}