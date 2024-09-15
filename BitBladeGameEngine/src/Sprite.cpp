#include "Sprite.h"
/* Sprite.cpp
*/



namespace game {
	Sprite::Sprite( const ImageSource* imageSource )
		: image( imageSource ),
		pivot( imageSource->Pivot() ),
		scale( imageSource->Scale() ) {}

}