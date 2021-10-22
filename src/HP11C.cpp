#include "framework.h"
#include "HP11C.h"

unsigned long g_windowCount = 0;  // set to 1 to keep on system tray

HINSTANCE g_hInstance = NULL;
IWICImagingFactory* g_pWICFactory = nullptr;
PCWSTR HKEY_PROJECT_TARGET = L"SOFTWARE\\BlueDragon\\HP11C";

long double stack[4] = {};
wstring stackString;
bool inEditMode = false;

bool FkeyActive = false;
bool GkeyActive = false;

bool RCLmodeActive = false;
bool STOmodeActive = false;
bool FIXmodeActive = false;

int f_fix = 2;

long double f_pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;

bool alawaysOnTop = false;
bool autoCopyToClipboard = false;

bool shiftStackOnEditMode = true;

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
    
    f_fix = LoadlongFromRegister(L"FIXvalue", 2);

    SetX(LoadlongdoubleFromRegister(L"Stack0", 0));
    stack[1] = LoadlongdoubleFromRegister(L"Stack1", 0);
    stack[2] = LoadlongdoubleFromRegister(L"Stack2", 0);
    stack[3] = LoadlongdoubleFromRegister(L"Stack3", 0);

    autoCopyToClipboard = LoadlongFromRegister(L"autoClipboard", false);
    alawaysOnTop = LoadlongFromRegister(L"keepOnTop", false);

    CreateMainWindow();
    
    // spam that message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);        
        DispatchMessage(&msg);
    }

    SaveLongToRegister(L"autoClipboard", autoCopyToClipboard);
    SaveLongToRegister(L"keepOnTop", alawaysOnTop);

    SavelongdoubleToRegister(L"Stack0", stack[0]);
    SavelongdoubleToRegister(L"Stack1", stack[1]);
    SavelongdoubleToRegister(L"Stack2", stack[2]);
    SavelongdoubleToRegister(L"Stack3", stack[3]);
}