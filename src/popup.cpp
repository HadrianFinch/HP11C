#include "framework.h"
#include "popup.h"
#include "decode.h"
#include "registers.h"
#include "layerwindow.h"
#include "resources.h"

bool CPopup::s_classRegistered = false;

/*static*/
HRESULT CPopup::Create(
    _In_ PCWSTR pWindowName,
    _In_ int pImageFile,
    _In_ POINT pt,
    _Outptr_ CPopup** ppPopup)
{
    HRESULT hr = S_OK;

    if (EnsureWndClass())
    {
        CPopup* pPopup = new CPopup(pImageFile, pt);

        hr = pPopup->Initialize(pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pPopup->m_hwnd, SW_SHOW);

            *ppPopup = pPopup;
            pPopup = nullptr;
        }

        delete pPopup;
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}

/*static*/
bool CPopup::EnsureWndClass()
{
    if (!s_classRegistered)
    {
        WNDCLASSW wndClass = {};

        wndClass.lpfnWndProc = CPopup::s_WindowProc;
        wndClass.hInstance = g_hInstance;
        wndClass.lpszClassName = c_szLayeredPopupClassName;
        wndClass.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IMG_ICON));
        wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);

        if (RegisterClass(&wndClass))
        {
            s_classRegistered = true;
        }
    }

    return s_classRegistered;
}

/*static*/
LRESULT CALLBACK CPopup::s_WindowProc(
    _In_ HWND hwnd,
    _In_ UINT uMsg, 
    _In_ WPARAM wParam, 
    _In_ LPARAM lParam)
{
    CPopup* pThis = nullptr;

    if (uMsg == WM_CREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<CPopup*>(pCS->lpCreateParams);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<CPopup*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis != nullptr)
    {
        // Assert(pThis->m_hwnd == hwnd);

        return pThis->WindowProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HRESULT CPopup::Initialize(
    _In_ PCWSTR pWindowName,
    _In_ int pImageFile)
{
    HRESULT hr = CreateBitmapFromResource(g_pWICFactory, pImageFile, &m_hBitmap);

    {
        BITMAP bm;
        GetObject(m_hBitmap, sizeof(bm), &bm);
        m_size = {bm.bmWidth, bm.bmHeight};
    }

    if (SUCCEEDED(hr))
    {
        m_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,              // Optional window styles.
                                c_szLayeredPopupClassName,
                                pWindowName,
                                WS_POPUPWINDOW | WS_CLIPCHILDREN, // Window style
                                m_pt.x, 
                                m_pt.y, 
                                m_size.cx, 
                                m_size.cy,
                                NULL,     // Parent window    
                                NULL,           // Menu
                                g_hInstance,    // Instance handle
                                this);
        if (m_hwnd == NULL)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }   

    return hr;
}

LRESULT CPopup::WindowProc(
    _In_ HWND hwnd,
    _In_ UINT uMsg, 
    _In_ WPARAM wParam, 
    _In_ LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            LayerWindow(hwnd, m_hBitmap, m_size, m_pt);
        }
        return 0;

        case WM_DESTROY:
        {
            if (m_masterPopup)
            {
                RECT rect = {};
                GetWindowRect(hwnd, &rect);
                SaveLongToRegister(REG_MAIN_LAST_X, rect.left + 1);
                SaveLongToRegister(REG_MAIN_LAST_Y, rect.top + 1);
            }
            
            hwnd = NULL;
            g_windowCount--;
            
            // if (g_windowCount == 0)
            // {
            //     PostQuitMessage(0);
            // } 
        }
        return 0;

        case WM_NCHITTEST:
        {
            if (m_dragRect.bottom != -1 and
                m_dragRect.top != -1 and
                m_dragRect.right != -1 and
                m_dragRect.left != -1)
            {
                DWORD coordintes = static_cast<DWORD>(lParam);
                POINT hitPos;
                hitPos.x = GET_X_LPARAM(lParam); 
                hitPos.y = GET_Y_LPARAM(lParam); 

                // RECT wndRect;
                POINT tl = {m_dragRect.left, m_dragRect.top};
                POINT br = {m_dragRect.right, m_dragRect.bottom};

                ClientToScreen(hwnd, &tl);
                ClientToScreen(hwnd, &br);

                if (hitPos.y >= tl.y and
                    hitPos.y <= br.y and
                    hitPos.x >= tl.x and
                    hitPos.x <= br.x)
                {
                    return HTCAPTION;
                }
            }
        }
        return HTCLIENT;

        case WM_HOTKEY:
        {
            if (wParam == 1)
            {
                if (m_masterPopup)
                {
                    
                }
            }
            else if (wParam == 2)
            {
                
            }
            else if (wParam == 3)
            {
                
            }
         
        }
        return 0;

        case WM_LBUTTONDOWN:
        {
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



HRESULT CWindowedPopup::Initialize(
    _In_ PCWSTR pWindowName,
    _In_ int pImageFile)
{
    EnsureWndClass();
    HRESULT hr = CreateBitmapFromResource(g_pWICFactory, pImageFile, &m_hBitmap);

    {
        BITMAP bm;
        GetObject(m_hBitmap, sizeof(bm), &bm);
        m_size = {bm.bmWidth, bm.bmHeight};
    }

    if (SUCCEEDED(hr))
    {
        m_hwnd = CreateWindowEx(0,              // Optional window styles.
                                c_szLayeredPopupClassName,
                                pWindowName,
                                WS_POPUPWINDOW | WS_CLIPCHILDREN, // Window style
                                m_pt.x, 
                                m_pt.y, 
                                m_size.cx, 
                                m_size.cy,
                                NULL,     // Parent window    
                                NULL,           // Menu
                                g_hInstance,    // Instance handle
                                this);
        if (m_hwnd == NULL)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }   

    return hr;
}

HRESULT CWindowedTopmostPopup::Initialize(
    _In_ PCWSTR pWindowName,
    _In_ int pImageFile)
{
    EnsureWndClass();
    HRESULT hr = CreateBitmapFromResource(g_pWICFactory, pImageFile, &m_hBitmap);

    {
        BITMAP bm;
        GetObject(m_hBitmap, sizeof(bm), &bm);
        m_size = {bm.bmWidth, bm.bmHeight};
    }
    
    if (SUCCEEDED(hr))
    {
        m_hwnd = CreateWindowEx(WS_EX_TOPMOST,              // Optional window styles.
                                c_szLayeredPopupClassName,
                                pWindowName,
                                WS_POPUPWINDOW | WS_CLIPCHILDREN, // Window style
                                m_pt.x, 
                                m_pt.y, 
                                m_size.cx, 
                                m_size.cy,
                                NULL,     // Parent window    
                                NULL,           // Menu
                                g_hInstance,    // Instance handle
                                this);
        if (m_hwnd == NULL)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }   

    return hr;
}
