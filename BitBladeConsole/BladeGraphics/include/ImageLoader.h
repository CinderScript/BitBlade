/* ImageLoader.h
*/

#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "ImageData.h"
#include <string>

class ImageLoader {
public:
	// Static method to load an image from the /images directory and return an ImageData object
	static ImageData* LoadImage(const std::string& imageName);
};

#endif // IMAGE_LOADER_H
