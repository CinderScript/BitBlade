/* SpriteInstance.h
*/

#ifndef SPRITE_INSTANCE_H
#define SPRITE_INSTANCE_H

#include "ImageData.h"
#include "MasterSpriteData.h"

class SpriteInstance {
public:
	SpriteInstance(MasterSpriteData* imageData);
	~SpriteInstance();

	// Setters and Getters
	void SetPosition(int x, int y);
	void SetRotation(float rotation);
	void SetScale(float scaleX, float scaleY);
	void SetPivot(int x, int y);

	int GetX() const;
	int GetY() const;
	float GetRotation() const;
	float GetScaleX() const;
	float GetScaleY() const;
	int GetPivotX() const;
	int GetPivotY() const;
	const ImageData* GetImageData() const;

private:
	int x, y;
	float rotation;
	float scaleX, scaleY;
	int pivotX, pivotY;   // Pivot point for rotation/position
	const ImageData* imageData;
};

#endif // SPRITE_INSTANCE_H
