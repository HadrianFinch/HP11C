#include "framework.h"
#include "HP11C.h"

unsigned long g_windowCount = 0;  // set to 1 to keep on system tray

HINSTANCE g_hInstance = NULL;
IWICImagingFactory* g_pWICFactory = nullptr;
PCWSTR HKEY_PROJECT_TARGET = L"SOFTWARE\\BlueDragon\\HP11C";

double stack[4] = {};
wstring stackString;
bool InEditMode = false;

bool FkeyActive = false;
bool GkeyActive = false;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    DWORD dwCoInit = COINIT_APARTMENTTHREADED;
    HRESULT hr = CoInitializeEx(nullptr, dwCoInit);
    if (FAILED(hr))
    {
        return HRESULT_CODE(hr);
    }
    
    hr = CreateWICFactory(&g_pWICFactory);
    if (FAILED(hr))
    {
        return HRESULT_CODE(hr);
    }

    CreateMainWindow();
    
    // spam that message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);        
        DispatchMessage(&msg);
    }
}