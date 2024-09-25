#pragma once
#include <Windows.h>
#include <string>
#include <wingdi.h>

using namespace std;

class Utils
{
public:
	static void WriteText(HDC hdc, Pos vector, const wstring& str);
	static void DrawRect(HDC hdc, Pos vector, int32 w, int32 h);
	static void DrawCircle(HDC hdc, Pos vector, int32 radius);
	static void DrawLine(HDC hdc, Pos from, Pos to);
	static void DrawColoredLine(HDC hdc, Pos from, Pos to, COLORREF color);
	static void DrawTransparentRect(HDC hdc, Pos vector, int32 w, int32 h, COLORREF color, int32 alpha);
};

