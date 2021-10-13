
#include <wincodec.h>   // for IWICImagingFactory

HRESULT CreateWICFactory(
    _Outptr_ IWICImagingFactory** ppFactory);

HRESULT CreateBitmapFromFile(
    _In_ IWICImagingFactory* pIWICFactory,
    _In_ PCWSTR uri,
    _Out_ HBITMAP* phBitmap);

HRESULT CreateBitmapFromResource(
    _In_ IWICImagingFactory* pIWICFactory,
    _In_ int resourcesId,
    _Out_ HBITMAP* phBitmap);
