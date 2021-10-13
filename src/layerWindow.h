
BOOL LayerWindow(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE windowSize,
    _In_ POINT ptWinPos);
    

BOOL LayerWindowDrawText(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE windowSize,
    _In_ PCWSTR pszText,
    _In_ POINT ptTextPos);
    
BOOL LayerWindowStretch(
    _In_ HWND hwnd,
    _In_ HBITMAP hBitmap,
    _In_ SIZE bitmapSize,
    _In_ SIZE windowSize,
    _In_ POINT ptWinPos);
    