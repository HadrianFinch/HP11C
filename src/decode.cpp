#include "framework.h"
#include <wincodec.h>
#include <windowsx.h>
#include <windows.h>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

HRESULT CreateWICFactory(
    _Outptr_ IWICImagingFactory** ppFactory)
{
    // Create WIC factory
    return CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(ppFactory)
        );
}

HRESULT CreateBitmapFromFile(
    _In_ IWICImagingFactory* pIWICFactory,
    _In_ PCWSTR uri,
    _Out_ HBITMAP* phBitmap)
{
    
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICFormatConverter *pConverter = NULL;
    IWICBitmapScaler *pScaler = NULL;

    IWICBitmapDecoder* pDecoder = nullptr;
    RETURN_IF_FAILED(pIWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder));


    IWICBitmapFrameDecode* pFirstFrame = nullptr;
    RETURN_IF_FAILED(pDecoder->GetFrame(0, &pFirstFrame));

    // Convert the image format to 32bppPBGRA
    RETURN_IF_FAILED(pIWICFactory->CreateFormatConverter(&pConverter));

    RETURN_IF_FAILED(
        pConverter->Initialize(
            pFirstFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut));

    UINT width = 0;
    UINT height = 0;
    RETURN_IF_FAILED(pConverter->GetSize(&width, &height));

    UINT stride = ALIGN_SIZE(width * 4, sizeof(DWORD));
    UINT cbSize = stride * height;
    BYTE* pMemory = new BYTE[cbSize];

    WICRect bitmapRect = {0, 0, static_cast<INT>(width), static_cast<INT>(height)};
    RETURN_IF_FAILED(pConverter->CopyPixels(
        &bitmapRect,
        stride,
        cbSize,
        pMemory));

    *phBitmap = CreateBitmap(
        width,
        height,
        1,
        32,
        pMemory);

    if (*phBitmap == NULL)
    {
        return E_OUTOFMEMORY;
    } 

    delete(pMemory);

    // release varibles from memory
    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);
    SafeRelease(&pFirstFrame);
    return S_OK;
}

HRESULT CreateBitmapFromResource(
    _In_ IWICImagingFactory* pIWICFactory,
    _In_ int resourceId,
    _Out_ HBITMAP* phBitmap)
{
    HRESULT hr = S_OK;

    // WIC interface pointers.
    IWICStream* pIWICStream = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFirstFrame = nullptr;

    // Resource management.
    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void *pImageFile = NULL;
    DWORD imageFileSize = 0;

    // Locate the resource in the application's executable.
    imageResHandle = FindResource(
        NULL,             
        MAKEINTRESOURCE(resourceId),
        L"Image");

    hr = (imageResHandle ? S_OK : E_FAIL);

    // Load the resource to the HGLOBAL.
    if (SUCCEEDED(hr))
    {
        imageResDataHandle = LoadResource(NULL, imageResHandle);
        hr = (imageResDataHandle ? S_OK : E_FAIL);
    }

    // Lock the resource to retrieve memory pointer.
    if (SUCCEEDED(hr))
    {
        pImageFile = LockResource(imageResDataHandle);
        hr = (pImageFile ? S_OK : E_FAIL);
    }

    // Calculate the size.
    if (SUCCEEDED(hr))
    {
        imageFileSize = SizeofResource(NULL, imageResHandle);
        hr = (imageFileSize ? S_OK : E_FAIL);
    }

    if (SUCCEEDED(hr))
    {
        hr = pIWICFactory->CreateStream(&pIWICStream);
    }

    // Initialize the stream with the memory pointer and size.
    if (SUCCEEDED(hr))
    {
        hr = pIWICStream->InitializeFromMemory(
            reinterpret_cast<BYTE*>(pImageFile),
            imageFileSize);
    }

    // Create a decoder for the stream.
    if (SUCCEEDED(hr))
    {
        hr = pIWICFactory->CreateDecoderFromStream(
            pIWICStream,
            NULL, 
            WICDecodeMetadataCacheOnLoad,
            &pDecoder);
    }

    if (pDecoder == nullptr)
    {
        if (DBG >= 2)
        {
            __debugbreak();
        }
        
        return hr;
    }

    // Retrieve the initial frame.
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFirstFrame);
    }

    IWICFormatConverter* pConverter = NULL;
    IWICBitmapScaler* pScaler = NULL;

    RETURN_IF_FAILED(pDecoder->GetFrame(0, &pFirstFrame));

    // Convert the image format to 32bppPBGRA
    RETURN_IF_FAILED(pIWICFactory->CreateFormatConverter(&pConverter));

    RETURN_IF_FAILED(pConverter->Initialize(
            pFirstFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut));

    UINT width = 0;
    UINT height = 0;
    RETURN_IF_FAILED(pConverter->GetSize(&width, &height));

    UINT stride = ALIGN_SIZE(width * 4, sizeof(DWORD));
    UINT cbSize = stride * height;
    BYTE* pMemory = new BYTE[cbSize];

    WICRect bitmapRect = {0, 0, static_cast<INT>(width), static_cast<INT>(height)};
    RETURN_IF_FAILED(pConverter->CopyPixels(
        &bitmapRect,
        stride,
        cbSize,
        pMemory));

    *phBitmap = CreateBitmap(
        width,
        height,
        1,
        32,
        pMemory);

    if (*phBitmap == NULL)
    {
        return E_OUTOFMEMORY;
    } 

    delete(pMemory);


    return hr;
}
