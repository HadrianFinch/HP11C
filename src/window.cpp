#include "framework.h"
#include "HP11C.h"

bool Window::s_classRegistered = false;

Window* Window::Create(Rect point, WindowType type, Window* hwndParent)
{
    Window* pWindow = new Window;
    pWindow->initilize(
        hwndParent,
        type,
        point,
        L"Preset Type");
    return pWindow;
}   

Window* Window::Create(Rect point, WindowType type, int image, Window* hwndParent)
{
    Window* pWindow = new Window;
    HBITMAP hbmp;
    CreateBitmapFromResource(g_pWICFactory, image, &hbmp);
    
    pWindow->initilize(
        hwndParent,
        type,
        point,
        L"Layered Image");
    
    LayerWindow(pWindow->m_hwnd, hbmp, point.Size(), point.Point());

    return pWindow;
}   

Window* Window::Create(Rect rect, DWORD color, Window* hwndParent)
{
    Window* pWindow = new Window;
    pWindow->m_color = color;
    pWindow->initilize(
        hwndParent,
        WindowType_Color,
        rect,
        L"Color View");
    
    return pWindow;
}

Window* Window::Create(Rect rect, DWORD color, unsigned int opacity, Window* hwndParent)
{
    Window* pWindow = new Window;
    pWindow->m_color = color;
    pWindow->initilize(
        hwndParent,
        WindowType_Color,
        rect,
        L"Color View",
        0,
        WS_EX_LAYERED);
    SetLayeredWindowAttributes(pWindow->m_hwnd, NULL, opacity, LWA_ALPHA);
    return pWindow;
}

Window* Window::CreateSZ(Rect rect, DWORD color, Window* hwndParent)
{
    Window* pWindow = new Window;
    pWindow->m_color = color;
    pWindow->m_sizeable = true;
    pWindow->initilize(
        hwndParent,
        WindowType_Color,
        rect,
        L"Color View",
        WS_SIZEBOX);
    
    return pWindow;
}

void Window::initilize(
    Window* hwndParent,
    WindowType wt, 
    Rect rc,
    PCWSTR windowName, 
    DWORD dwStyle, 
    DWORD dwExStyle)
{
    m_type = wt;

    // Change styles
    if (hwndParent != nullptr)
    {
        dwStyle |= (WS_CHILD | WS_CLIPSIBLINGS);
        hwndParent->m_children.push_back(this);
    }
    else
    {
        dwStyle |= (WS_CLIPCHILDREN | WS_POPUPWINDOW | WS_SIZEBOX);
        
        rc.Width(rc.Width() + (s_ncborderSize * 2));
        rc.Height(rc.Height() + s_ncborderSize);
    }
    
    HWND hwndP = NULL;
    if (hwndParent != nullptr)
    {
        hwndP = hwndParent->m_hwnd;
    }

    if ((wt == WindowType_Close) or 
        (wt == WindowType_Minimize) or 
        (wt == WindowType_Maximize))
    {
        rc.Width(13);
        rc.Height(13);
    }
    
    // Make the window
    RegisterClass();
    m_hwnd = CreateWindowExW(
        dwExStyle,
        s_className,
        windowName,
        dwStyle,
        rc.Point().x,
        rc.Point().y,
        labs(rc.Width()),
        labs(rc.Height()),
        hwndP,
        NULL,
        g_hInstance,
        this);

    SetWindowPos(HWND_TOP, rc, 0);
    switch (wt)
    {
        case WindowType_Color:
        {
            
        }
        break;

        case WindowType_Close:
        {
            HBITMAP hbmp = NULL;
            CreateBitmapFromResource(g_pWICFactory, IMG_CLOSE, &hbmp);
            LayerWindow(m_hwnd, hbmp, rc.Size(), rc.Point());
        }
        break;

        case WindowType_Minimize:
        {
            HBITMAP hbmp = NULL;
            CreateBitmapFromResource(g_pWICFactory, IMG_MINIMIZE, &hbmp);
            LayerWindow(m_hwnd, hbmp, rc.Size(), rc.Point());
        }
        break;

        case WindowType_Maximize:
        {
            HBITMAP hbmp = NULL;
            CreateBitmapFromResource(g_pWICFactory, IMG_MAXIMIZE, &hbmp);
            LayerWindow(m_hwnd, hbmp, rc.Size(), rc.Point());
        }
        break;
    }
    ShowWindow(m_hwnd, SW_SHOW);
    m_windowRect = rc;
}

LRESULT CALLBACK Window::s_wndProc(
    HWND hwnd, 
    UINT msg, 
    WPARAM wParam, 
    LPARAM lParam)
{
    Window* pThis = nullptr;

    if (msg == WM_CREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCS->lpCreateParams);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        SetTimer(hwnd, 321, 5, NULL);
    }
    else
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis != nullptr)
    {
        return pThis->WndProc(msg, wParam, lParam);
    } 

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (m_pWndProc != nullptr)
    {
        return m_pWndProc(this, msg, wParam, lParam);
    }
    
    if ((msg == WM_TIMER) and (wParam == 321))
    {
        KillTimer(m_hwnd, wParam);
        Call(m_WMCREATE, wParam, lParam);
    }
    
    if ((GetParent(m_hwnd) == NULL))
    {
        if (msg == WM_NCCALCSIZE)
        {
            if (wParam)
            {
                NCCALCSIZE_PARAMS* pncc = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

                auto rcWindow = pncc->rgrc[0];

                DefWindowProc(m_hwnd, msg, wParam, lParam);
                
                pncc->rgrc[0] = {rcWindow.left + s_ncborderSize,
                                rcWindow.top,
                                rcWindow.right - s_ncborderSize,
                                rcWindow.bottom - s_ncborderSize};

                InvalidateRect(m_hwnd, NULL, true);

                return 0;
            }
            return DefWindowProc(m_hwnd, msg, wParam, lParam);
        }
        else if (msg == WM_NCACTIVATE)
        {
            return 0;
        }
        else if (msg == WM_DESTROY)
        {
            PostQuitMessage(0);
            return 0;
        }
    }
    
    switch (m_type)
    {
        case WindowType_Color:
        {
            if (msg == WM_PAINT)
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(m_hwnd, &ps);

                Rect rcPaint({});
                rcPaint.GetClientRect(m_hwnd);
                rcPaint.Point({0, 0});
                
                COLORREF colorref = m_color;
                HBRUSH brush = CreateSolidBrush(colorref);
                FillRect(hdc, rcPaint.RectPointer(), brush);

                EndPaint(m_hwnd, &ps);
                return 0;
            }

            if (msg == WM_NCPAINT)
            {
                HDC hdc = GetDCEx(m_hwnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);

                COLORREF colorref = m_color;
                HBRUSH brush = CreateSolidBrush(colorref);
                FillRect(hdc, m_windowRect.RectPointer(), brush);
                 
                ReleaseDC(m_hwnd, hdc);
                return 0;
            }
        }
    }

    if (msg == WM_SETCURSOR)
    {
        if (LOWORD(lParam) == HTCLIENT)
        {
            LPCWSTR cursorImage = IDC_ARROW;
            
            HCURSOR cursor = LoadCursorW(NULL, cursorImage);
            if (cursor != NULL)
            {
                SetCursor(cursor);
            }
            return FALSE;
        }
        return DefWindowProc(m_hwnd, msg, wParam, lParam);
    }
    else if (msg == WM_WINDOWPOSCHANGED)
    {
        WINDOWPOS* pos = reinterpret_cast<WINDOWPOS*>(lParam);
        if ((pos->cx != 0) and (pos->cy != 0))
        {   
            if ((m_windowRect.Width() != Rect::SIZE_FULL) and (m_windowRect.Height() != Rect::SIZE_FULL))
            {
                m_windowRect.Size({pos->cx, pos->cy});
            }
            if ((m_windowRect.GetVerticalAlignment() == align_default) and
                (m_windowRect.GetHorizontalAlignment() == align_default))
            {
                m_windowRect.Point({pos->x, pos->y}); 
            }

            for (Window* pChild : m_children)
            {
                Rect& rcChild = pChild->m_windowRect;

                if (rcChild.Width() == Rect::SIZE_FULL)
                {
                    Rect rc = rcChild;
                    rc.Width(m_windowRect.Width());
                    pChild->SetWindowPos(NULL, rc, SWP_NOMOVE);
                }
                
                if (rcChild.Height() == Rect::SIZE_FULL)
                {
                    Rect rc = rcChild; 
                    rc.Height(m_windowRect.Height());
                    pChild->SetWindowPos(NULL, rc, SWP_NOMOVE);
                }

                if (rcChild.GetVerticalAlignment() == align_bottom)
                {
                    Rect rc = rcChild;
                    long parentHeight = m_windowRect.Height();

                    // Account for Rect::SIZE_FULL
                    if (parentHeight == Rect::SIZE_FULL)
                    {
                        Rect rc2;
                        rc2.GetClientRect(GetParent(m_hwnd));
                        parentHeight = rc2.Height();
                    }
                    
                    rc.Point({rc.Point().x, parentHeight - (rc.Point().y + rc.Height())});
                    pChild->SetWindowPos(NULL, rc, SWP_NOSIZE);
                }

                if (rcChild.GetHorizontalAlignment() == align_right)
                {
                    Rect rc = rcChild;
                    long parentWidth = m_windowRect.Width();

                    // Account for Rect::SIZE_FULL
                    if (parentWidth == Rect::SIZE_FULL)
                    {
                        Rect rc2;
                        rc2.GetClientRect(GetParent(m_hwnd));
                        parentWidth = rc2.Width();
                    }
                    
                    rc.Point({parentWidth - (rc.Point().x + rc.Width()), rc.Point().y});
                    pChild->SetWindowPos(NULL, rc, SWP_NOSIZE);
                }
            }
        }
        return 0;
    }
    else if (msg == WM_LBUTTONDOWN)
    {
        Call(m_WMLBUTTONDOWN, wParam, lParam);
        m_mouseDown = true;
        if (m_dragWindow != nullptr)
        {
            m_dragPtStart.x = GET_X_LPARAM(lParam);
            m_dragPtStart.y = GET_Y_LPARAM(lParam);
            ClientToScreen(m_hwnd, &m_dragPtStart);
            
            SetCapture(m_hwnd);  
        }        
    }
    else if (msg == WM_LBUTTONUP)
    {
        Call(m_WMLBUTTONUP, wParam, lParam);
        ReleaseCapture();
        m_mouseDown = false;
        if (m_dragWindow != nullptr)
        {
            m_dragPtStart = {};
        }        

        switch (m_type)
        {
            case WindowType_Close:
            {
                DestroyWindow(GetParent(GetParent(m_hwnd)));
            }
            return 0;

            case WindowType_Minimize:
            {
                HWND hwnd = GetParent(GetParent(m_hwnd));
                ShowWindow(hwnd, SW_MINIMIZE);
            }
            return 0;
        }
        return 0;
    }
    else if (msg == WM_MOUSEMOVE)
    {
        Call(m_WMMOUSEMOVE, wParam, lParam);
        TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT)};
        tme.dwFlags = TME_LEAVE;
        TrackMouseEvent(&tme); 

        if ((m_dragWindow != nullptr) and (m_mouseDown))
        {
            POINT ptMouse = {};
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            ClientToScreen(m_hwnd, &ptMouse);
            if (WindowFromPoint(ptMouse) == m_hwnd)
            {
                SIZE dist = {};
                dist.cx = (ptMouse.x - m_dragPtStart.x);
                dist.cy = (ptMouse.y - m_dragPtStart.y);

                RECT rcParent = {};
                GetWindowRect(m_dragWindow->m_hwnd, &rcParent);
                ::SetWindowPos(
                    m_dragWindow->m_hwnd,
                    NULL,
                    (rcParent.left + dist.cx),
                    (rcParent.top + dist.cy),
                    (rcParent.right - rcParent.left),
                    (rcParent.bottom - rcParent.top),
                    0);

                m_dragPtStart = ptMouse;
            }
        }
        return 0;
    }
    else if (msg == WM_MOUSELEAVE)
    {
        m_mouseDown = false;
    }
    else if (msg == WM_NCHITTEST)
    {
        if (m_sizeable)
        {
            return DefWindowProc(m_hwnd, msg, wParam, lParam);
        }
        return HTCLIENT;
    }
    else if (msg == WM_KEYUP)
    {
        Call(m_WMKEYUP, wParam, lParam);
        return 0;
    }

    return DefWindowProc(m_hwnd, msg, wParam, lParam);
}
