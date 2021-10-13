#include "framework.h"
#include "button.h"
#include "buttonTypes.h"

/* Override */
void CCloseButton::OnClicked() 
{
    HWND hwndParent = GetParent(m_hwnd);
    if (hwndParent != NULL)
    {
        DestroyWindow(hwndParent);
    }
}

/* Override */
void CQuitAppButton::OnClicked() 
{
    PostQuitMessage(0);
}

/* Override */
void CMinimizeButton::OnClicked() 
{
    HWND hwndParent = GetParent(m_hwnd);
    if (hwndParent != NULL)
    {
        ShowWindow(hwndParent, SW_MINIMIZE);
    }
}

/* Override */
void CMaximizeButton::OnClicked() 
{
    // HWND hwndParent = GetParent(m_hwnd);
    // if (hwndParent != NULL)
    // {
    //     if (g_maximized)
    //     {
    //         ShowWindow(hwndParent, SW_RESTORE);
    //         SetWindowPos(
    //             hwndParent,
    //             NULL,
    //             g_hrcRestore.point.x,
    //             g_hrcRestore.point.y,
    //             g_hrcRestore.size.cx,
    //             g_hrcRestore.size.cy,
    //             0);
    //         g_maximized = false;
    //     }
    //     else if (!g_maximized)
    //     {
    //         Rect rcParent({});
    //         rcParent.GetWindowRect(hwndParent);
    //         g_hrcRestore = rcParent.HRECT();

    //         ShowWindow(hwndParent, SW_SHOWMAXIMIZED);
    //         g_maximized = true;
    //     }
    // }
}

/* Override */
void CDragBox::LBUTTONDOWN(LPARAM lParam) 
{
    HWND hwndParent = GetParent(m_hwnd);
    if (hwndParent != NULL)
    {
        ptStart.x = GET_X_LPARAM(lParam);
        ptStart.y = GET_Y_LPARAM(lParam);
        ClientToScreen(m_hwnd, &ptStart);
        
        SetCapture(m_hwnd);
    }
}

/* Override */
void CDragBox::OnClicked() 
{
    ptStart = {};
    ReleaseCapture();
}

/* Override */
void CDragBox::MouseMove(HWND hwnd, LPARAM lParam) 
{
    HWND hwndParent = GetParent(m_hwnd);
    if (hwndParent != NULL)
    {
        if (m_mouseDown)
        {
            POINT ptMouse = {};
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            ClientToScreen(m_hwnd, &ptMouse);

            SIZE dist = {};
            dist.cx = (ptMouse.x - ptStart.x);
            dist.cy = (ptMouse.y - ptStart.y);

            RECT crParent = {};
            GetWindowRect(hwndParent, &crParent);

            // if (g_maximized)
            // {
            //     ShowWindow(hwndParent, SW_RESTORE);
            //     SetWindowPos(
            //         hwndParent,
            //         NULL,
            //         (crParent.left + dist.xDist),
            //         (crParent.top + dist.yDist),
            //         g_hrcRestore.size.cx,
            //         g_hrcRestore.size.cy,
            //         0);
            //     g_maximized = false;
            // }
            // else
            // {
            //     SetWindowPos(
            //         hwndParent,
            //         NULL,
            //         (crParent.left + dist.xDist),
            //         (crParent.top + dist.yDist),
            //         (crParent.right - crParent.left),
            //         (crParent.bottom - crParent.top),
            //         0);
            // }

            ptStart = ptMouse;
        }
    }
}
