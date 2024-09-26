#include "Utils.h"
#include "pch.h"

void Utils::WriteText(HDC hdc, Pos vector, const wstring& str)
{
    
	::TextOut(hdc, static_cast<int32>(vector.x), static_cast<int32>(vector.y), str.c_str(), static_cast<int32>(str.size()));
}

void Utils::DrawRect(HDC hdc, Pos vector, int32 w, int32 h)
{
	::Rectangle(hdc, 
		static_cast<int32>(vector.x - w / 2),
		static_cast<int32>(vector.y - h / 2),
		static_cast<int32>(vector.x + w / 2),
		static_cast<int32>(vector.y + h / 2));
}

void Utils::DrawCircle(HDC hdc, Pos vector, int32 radius)
{
	::Ellipse(hdc,
		static_cast<int32>(vector.x - radius),
		static_cast<int32>(vector.y - radius),
		static_cast<int32>(vector.x + radius),
		static_cast<int32>(vector.y + radius));
}

void Utils::DrawLine(HDC hdc, Pos from, Pos to)
{
	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));
}

void Utils::DrawColoredLine(HDC hdc, Pos from, Pos to, COLORREF color)
{
	HPEN pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN oldPen = (HPEN)::SelectObject(hdc, (HGDIOBJ)pen);

	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));

	::SelectObject(hdc, (HGDIOBJ)oldPen);
	::DeleteObject(pen);
}

void Utils::DrawTransparentRect(HDC hdc, Pos vector, int32 w, int32 h, COLORREF color, int32 alpha)
{
    int32 left = static_cast<int32>(vector.x - w / 2);
    int32 top = static_cast<int32>(vector.y - h / 2);
    int32 right = static_cast<int32>(vector.x + w / 2);
    int32 bottom = static_cast<int32>(vector.y + h / 2);
    int32 width = right - left;
    int32 height = bottom - top;

    // �޸� DC ����
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // ��Ʈ���� �����ϰ� �ʱ�ȭ
    BLENDFUNCTION blendFunc = { 0 };
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.BlendFlags = 0;
    blendFunc.SourceConstantAlpha = alpha;
    blendFunc.AlphaFormat = 0;

    // ���� ���� �� ������ �簢�� �׸���
    HBRUSH hBrush = CreateSolidBrush(color);
    RECT rect = { 0, 0, width, height };
    FillRect(memDC, &rect, hBrush);
    DeleteObject(hBrush);

    // AlphaBlend �Լ��� ����Ͽ� ������ �簢���� ��� DC�� ����
    AlphaBlend(hdc, left, top, width, height, memDC, 0, 0, width, height, blendFunc);

    // �޸� DC�� ��Ʈ�� ����
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}
