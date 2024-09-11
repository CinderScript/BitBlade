/* MasterSpriteData.cpp
*/

#include "MasterSpriteData.h"

MasterSpriteData::MasterSpriteData(ImageData* imageData)
	: scaleX(1.0f), scaleY(1.0f), pivotX(0), pivotY(0), imageData(imageData) {
	// Initialize sprite with the provided image data
}

MasterSpriteData::~MasterSpriteData() {
	delete imageData;  // Clean up the owned ImageData
}

void MasterSpriteData::SetScale(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void MasterSpriteData::SetPivot(int x, int y) {
	this->pivotX = x;
	this->pivotY = y;
}

int MasterSpriteData::GetPivotX() const { return pivotX; }
int MasterSpriteData::GetPivotY() const { return pivotY; }
const ImageData* MasterSpriteData::GetImageData() const { return imageData; }
