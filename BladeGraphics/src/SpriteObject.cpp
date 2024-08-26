/* SpriteObject.cpp
*/

#include "SpriteObject.h"

SpriteObject::SpriteObject(ImageData* imageData)
	: x(0), y(0), rotation(0), scaleX(1.0f), scaleY(1.0f), pivotX(0), pivotY(0), imageData(imageData) {
	// Initialize sprite with the provided image data
}

SpriteObject::~SpriteObject() {
	delete imageData;  // Clean up the owned ImageData
}

void SpriteObject::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

void SpriteObject::SetRotation(float rotation) {
	this->rotation = rotation;
}

void SpriteObject::SetScale(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void SpriteObject::SetPivot(int x, int y) {
	this->pivotX = x;
	this->pivotY = y;
}

int SpriteObject::GetX() const { return x; }
int SpriteObject::GetY() const { return y; }
float SpriteObject::GetRotation() const { return rotation; }
float SpriteObject::GetScaleX() const { return scaleX; }
float SpriteObject::GetScaleY() const { return scaleY; }
int SpriteObject::GetPivotX() const { return pivotX; }
int SpriteObject::GetPivotY() const { return pivotY; }
const ImageData* SpriteObject::GetImageData() const { return imageData; }
