/* 
    Uncomment librarys as needed to compile 
*/
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib") 
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Dwmapi.lib")
#pragma comment(lib,"Comdlg32.lib")
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"Advapi32.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"d2d1.lib")

#pragma warning(disable:4100)

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

/* 
    Uncomment Headders as needed to compile 
*/
#include <exception>
#include <wincodec.h>
#include <windowsx.h>
#include <strsafe.h>
#include <pathcch.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <dwmapi.h>
#include <shobjidl.h>
#include <commdlg.h>
#include <tchar.h>
#include <math.h>

#include "rect.h"

// #include <termicolor.h>
#include <string>
#include <vector>

#define RETURN_IF_FAILED(hr) { if(FAILED(hr)) return hr; }

#define ALIGN_SIZE( sizeToAlign, PowerOfTwo )\
(((sizeToAlign) + (PowerOfTwo) - 1) & ~((PowerOfTwo) - 1))

#ifdef DEBUG_BUILD
#define DBG                     2
#endif
#ifndef DEBUG_BUILD
#define DBG                     0
#endif

#ifndef ASSERT_DEFINED
#define ASSERT_DEFINED
__inline void Assert(_In_ bool value)
{
    if (DBG > 0)
    {
        if (value != false)
        {
            return;
        }
        else
        {
            __debugbreak();
        }
    }    
}
#endif

#define or    ||
#define and   &&
#define null  nullptr

extern HINSTANCE g_hInstance;
extern IWICImagingFactory* g_pWICFactory;
extern unsigned long g_windowCount;
extern PCWSTR HKEY_PROJECT_TARGET;

using namespace std;
