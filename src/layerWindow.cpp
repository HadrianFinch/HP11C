#include "framework.h"

BOOL LayerWindow(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE windowSize,
    _In_ POINT ptWinPos)
{
    // Add the WS_EX_LAYERED bit to the window ex style
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    HDC hdcSrc = CreateCompatibleDC(NULL);
    HGDIOBJ hBmpSave = SelectObject(hdcSrc, hBitmap);

    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    POINT ptNull = {0,0};

    // fix window pos errors
    ptWinPos.x = ptWinPos.x - 1;
    ptWinPos.y = ptWinPos.y - 1;

    BOOL bRet = UpdateLayeredWindow(
        hwnd,
        NULL,
        &ptWinPos,
        &windowSize,
        hdcSrc,
        &ptNull,
        NULL,
        &bf,
        ULW_ALPHA);

    SelectObject(hdcSrc, hBmpSave);
    DeleteDC(hdcSrc);

    return bRet;
}

BOOL LayerWindowDrawText(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE windowSize,
    _In_ PCWSTR pszText,
    _In_ POINT ptTextPos)
{
    // Add the WS_EX_LAYERED bit to the window ex style
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    HDC hdcSrc = CreateCompatibleDC(NULL);
    HDC hdcDest = CreateCompatibleDC(NULL);

    BITMAPINFO bi = {};
	bi.bmiHeader.biSize = sizeof(bi);
	bi.bmiHeader.biWidth = windowSize.cx;
	bi.bmiHeader.biHeight = -windowSize.cy; // negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;

    unsigned char* pvBits;
    HBITMAP hBitmapSection = CreateDIBSection(
        hdcSrc,
        &bi,
        DIB_RGB_COLORS,
        (void**)&pvBits,
        NULL,
        0);

    HGDIOBJ hBmpSave1 = SelectObject(hdcSrc, hBitmap);
    HGDIOBJ hBmpSave2 = SelectObject(hdcDest, hBitmapSection);

    BitBlt(
        hdcDest,
        0,
        0,
        windowSize.cx,
        windowSize.cy,
        hdcSrc,
        0,
        0,
        SRCCOPY);

    size_t cchLength;
    StringCchLength(pszText, MAX_PATH, &cchLength);

    SIZE textSize;
    GetTextExtentPoint(
        hdcDest,
        pszText,
        static_cast<UINT>(cchLength),
        &textSize);

    SetBkColor(hdcDest, RGB(216, 216, 216));
    ExtTextOut(
        hdcDest,
        ptTextPos.x,
        ptTextPos.y,
        ETO_OPAQUE,
        NULL,
        pszText,
        static_cast<UINT>(cchLength),
        NULL);

    const UINT pitch = 4 * windowSize.cx;

    // // Fixup the alpha for the text to be opaque
    // for (int y = ptTextPos.y; y < ptTextPos.y + textSize.cy; y++)
    // {
    //     for (int x = ptTextPos.x; x < ptTextPos.x + textSize.cx; x++)
    //     {
    //         UINT index = (pitch * y) + (x * 4) + 3;

    //         pvBits[index] = 0xFF;
    //     }
    // }

    // Fixup the alpha for the text to be opaque v2
    for (int y = ptTextPos.y; y < ptTextPos.y + textSize.cy; y++)
    {
        for (int x = ptTextPos.x; x < ptTextPos.x + textSize.cx; x++)
        {
            UINT index = (pitch * y) + (x * 4) + 3;

            pvBits[index] = 0xFF;
        }
    }

    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    POINT ptNull = {0,0};

    BOOL bRet = UpdateLayeredWindow(
        hwnd,
        NULL,
        nullptr,
        &windowSize,
        hdcDest,
        &ptNull,
        NULL,
        &bf,
        ULW_ALPHA);

    SelectObject(hdcSrc, hBmpSave1);
    SelectObject(hdcDest, hBmpSave2);

    DeleteObject(hBitmapSection);

    DeleteDC(hdcSrc);
    DeleteDC(hdcDest);

    return bRet;
}


BOOL LayerWindowStretch(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE bitmapSize,
    _In_ SIZE windowSize,
    _In_ POINT ptWinPos)
{
    // Add the WS_EX_LAYERED bit to the window ex style
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    HDC hdcSrc = CreateCompatibleDC(NULL);
    HDC hdcDest = CreateCompatibleDC(NULL);

    BITMAPINFO bi = {};
	bi.bmiHeader.biSize = sizeof(bi);
	bi.bmiHeader.biWidth = windowSize.cx;
	bi.bmiHeader.biHeight = -windowSize.cy; // negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;

    unsigned char* pvBits;
    HBITMAP hBitmapSection = CreateDIBSection(
        hdcSrc,
        &bi,
        DIB_RGB_COLORS,
        (void**)&pvBits,
        NULL,
        0);

    HGDIOBJ hBmpSave1 = SelectObject(hdcSrc, hBitmap);
    HGDIOBJ hBmpSave2 = SelectObject(hdcDest, hBitmapSection);

    StretchBlt(
        hdcDest,
        0,
        0,
        windowSize.cx,
        windowSize.cy,
        hdcSrc,
        0,
        0,
        bitmapSize.cx,
        bitmapSize.cy,
        SRCCOPY);

    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    POINT ptNull = {0,0};

    BOOL bRet = UpdateLayeredWindow(
        hwnd,
        NULL,
        nullptr,
        &windowSize,
        hdcDest,
        &ptNull,
        NULL,
        &bf,
        ULW_ALPHA);

    SelectObject(hdcSrc, hBmpSave1);
    SelectObject(hdcDest, hBmpSave2);

    DeleteObject(hBitmapSection);

    DeleteDC(hdcSrc);
    DeleteDC(hdcDest);

    return bRet;    
}
