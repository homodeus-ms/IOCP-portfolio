#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
public:
	Texture();
	virtual ~Texture();

	Texture* LoadBmp(HWND hwnd, const wstring& path);
	HDC GetDc() const { return _hdc; }
	VectorInt GetSize() const { return _size; }
	void SetSize(VectorInt size) { _size = size; }
	uint32 GetTransparent() const { return _transparent; }
	void SetTransparent(uint32 transparent) { _transparent = transparent; }

private:

	HDC _hdc = {};
	HBITMAP _bitMap = {};
	VectorInt _size = {};
	uint32 _transparent = (RGB(255, 0, 255));
};

