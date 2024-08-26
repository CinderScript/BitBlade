/* SpriteObject.h
*/

#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include "ImageData.h"

class SpriteObject {
public:
	SpriteObject(ImageData* imageData);
	~SpriteObject();

	// Setters and Getters
	void SetPosition(int x, int y);
	void SetRotation(float rotation);
	void SetScale(float scaleX, float scaleY);
	void SetPivotPoint(int x, int y);

	int GetX() const;
	int GetY() const;
	float GetRotation() const;
	float GetScaleX() const;
	float GetScaleY() const;
	int GetPivotX() const;
	int GetPivotY() const;
	const ImageData* GetImageData() const;

private:
	int x, y;             // Position of the sprite
	float rotation;       // Rotation angle in degrees
	float scaleX, scaleY; // Scaling factors for width and height
	int pivotX, pivotY;   // Pivot point for rotation
	const ImageData* imageData; // Pointer to ImageData for the sprite's image
};

#endif // SPRITE_OBJECT_H
