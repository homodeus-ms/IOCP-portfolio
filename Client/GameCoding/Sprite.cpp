#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(Texture* texture, uint32 x, uint32 y, uint32 endX, uint32 endY)
	: Super()
	, _texture(texture), _x(x), _y(y), _endX(endX), _endY(endY)
{
}

Sprite::~Sprite()
{
}
