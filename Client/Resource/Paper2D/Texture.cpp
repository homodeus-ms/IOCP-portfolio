#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

Texture* Texture::LoadBmp(HWND hwnd, const wstring& path)
{
    HDC hdc = ::GetDC(hwnd);
    _hdc = ::CreateCompatibleDC(hdc);

    _bitMap = (HBITMAP)::LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (_bitMap == 0)
    {
        ::MessageBox(hwnd, path.c_str(), L"Image Load Failed", NULL);
        return nullptr;
    }

    HBITMAP prev = (HBITMAP)::SelectObject(_hdc, (HGDIOBJ)_bitMap);
    ::DeleteObject(prev);

    BITMAP bitmap = {};
    ::GetObject(_bitMap, sizeof(bitmap), &bitmap);
    _size.x = bitmap.bmWidth;
    _size.y = bitmap.bmHeight;

    
    return this;
}
