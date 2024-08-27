/* MasterSpriteData.cpp
*/

#include "SpriteInstance.h"

SpriteInstance::SpriteInstance(MasterSpriteData* spriteData)
	: x(0), y(0), rotation(0), scaleX(1.0f), scaleY(1.0f),
	pivotX(spriteData->GetPivotX()), pivotY(spriteData->GetPivotY()),
	imageData(spriteData->GetImageData()) {
	// Initialize sprite with the provided image data
}

SpriteInstance::~SpriteInstance() {
	// image data is owned by SpriteInstance, which is owned by BladeGraphics
}

void SpriteInstance::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

void SpriteInstance::SetRotation(float rotation) {
	this->rotation = rotation;
}

void SpriteInstance::SetScale(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void SpriteInstance::SetPivot(int x, int y) {
	this->pivotX = x;
	this->pivotY = y;
}

int SpriteInstance::GetX() const { return x; }
int SpriteInstance::GetY() const { return y; }
float SpriteInstance::GetRotation() const { return rotation; }
float SpriteInstance::GetScaleX() const { return scaleX; }
float SpriteInstance::GetScaleY() const { return scaleY; }
int SpriteInstance::GetPivotX() const { return pivotX; }
int SpriteInstance::GetPivotY() const { return pivotY; }
const ImageData* SpriteInstance::GetImageData() const { return imageData; }
