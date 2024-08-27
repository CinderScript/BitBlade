/* MasterSpriteData.h
*/

#ifndef MASTER_SPRITE_DATA_H
#define MASTER_SPRITE_DATA_H

#include "ImageData.h"

class MasterSpriteData {
public:
	MasterSpriteData(ImageData* imageData);
	~MasterSpriteData();

	// Setters and Getters
	void SetScale(float scaleX, float scaleY);
	void SetPivot(int x, int y);

	int GetPivotX() const;
	int GetPivotY() const;
	const ImageData* GetImageData() const;

private:
	float scaleX, scaleY;
	int pivotX, pivotY;   // Pivot point for rotation/position
	const ImageData* imageData;
};

#endif // MASTER_SPRITE_DATA_H
