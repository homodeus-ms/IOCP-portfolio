#pragma once


class UI
{
public:
	UI();
	virtual ~UI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	Pos GetPos() const { return _pos; }
	void SetPos(Pos pos) { _pos = pos; }
	VectorInt GetSize() const { return _size; }
	void SetSize(VectorInt size) { _size = size; }

	RECT GetRect();
	bool IsMouseInRect();

protected:
	Pos _pos = { 300, 400 };
	VectorInt _size = { 150, 150 };
};

