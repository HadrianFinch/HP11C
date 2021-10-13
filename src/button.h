#pragma once
#include "framework.h"

class CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton);
protected:
    static bool EnsureWndClass();

    static LRESULT CALLBACK s_WindowProc(
        _In_ HWND hwnd,
        _In_ UINT uMsg, 
        _In_ WPARAM wParam, 
        _In_ LPARAM lParam);

    static constexpr PCWSTR c_szLayeredButtonClassName = L"LayeredButton";
    static bool s_classRegistered; 

    HRESULT Initialize(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile);

public:
    CButton(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        m_size(size),
        m_pt(pt)
    {
    }

    virtual ~CButton()
    {
        if (m_hwnd != NULL)
        {
            DestroyWindow(m_hwnd);
        }
    }

protected:
    virtual void OnClicked()
    {
    }
    virtual void TimerTrigger(WPARAM timerId)
    {
        KillTimer(m_hwnd, timerId);
    }
    virtual void LBUTTONDOWN(LPARAM lParam)
    {
    }
    virtual void MouseMove(HWND hwnd, LPARAM lParam)
    {
    }
    virtual void WINDOWPOSCHANGING(HWND hwnd, LPARAM lParam)
    {
    }
    virtual LRESULT NCHITTEST(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
    }
    virtual void WMCREATE(HWND hwnd);
    virtual void WMDESTROY(HWND hwnd)
    {
    }
    
    LRESULT WindowProc(
        _In_ HWND HWND,
        _In_ UINT uMsg, 
        _In_ WPARAM wParam, 
        _In_ LPARAM lParam);
protected:
    bool m_mouseDown = false;

public:
    int m_pHoverImageFileName = 0;
    SIZE m_HoverSize = {-1, -1};
    POINT m_HoverPt = {-1, -1};

    int m_pMouseDownImageFileName = 0;
    SIZE m_MouseDownSize;
    POINT m_MouseDownPt;

    HWND m_hwnd = NULL;
    SIZE m_size;
    POINT m_pt;
    HBITMAP m_hBitmap = NULL;
};
