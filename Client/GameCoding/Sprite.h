#pragma once
#include "ResourceBase.h"
#include "Texture.h"

class Texture;

class Sprite : public ResourceBase
{
	using Super = ResourceBase;

public:
	Sprite(Texture* texture, uint32 x, uint32 y, uint32 endX, uint32 endY);
	virtual ~Sprite();
	HDC GetDc() { return _texture->GetDc(); }
	VectorInt GetPos() const { return VectorInt(_x, _y); }
	VectorInt GetSize() const { return VectorInt(_endX, _endY); }
	uint32 GetTransparent() const { return _texture->GetTransparent(); }

private:
	Texture* _texture = nullptr;
	uint32 _x;
	uint32 _y;
	uint32 _endX;
	uint32 _endY;
};

