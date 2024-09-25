#pragma once

class Texture;

struct FlipbookInfo
{
	Texture* texture = nullptr;
	wstring name = L"";
	VectorInt size = {};
	uint32 start = 0;
	uint32 end = 0;
	uint32 line = 0;
	float duration = 1.f;
	bool loop = true;
};

class Flipbook
{
public:
	Flipbook() {}
	~Flipbook() {}

	void SetFlipbookInfo(const FlipbookInfo& info) { _info = info; }
	const FlipbookInfo& GetFlipbookInfo() const { return _info; }

private:
	FlipbookInfo _info = {};
};

