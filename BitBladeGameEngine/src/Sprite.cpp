#include "Sprite.h"
/* Sprite.cpp
*/



Sprite::Sprite( const ImageSource* imageSource )
	: image( imageSource ),
	pivot( imageSource->Pivot() ),
	scale( imageSource->Scale() ) {}

