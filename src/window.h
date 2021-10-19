#include "framework.h"

enum WindowType
{
    WindowType_Close = 1,
    WindowType_Minimize = 2,
    WindowType_Maximize = 3,
    WindowType_TopmostPopup = 4,
    WindowType_WindowedPopup = 5,

    WindowType_WndProc = 7,
    WindowType_Color = 8,
    WindowType_ColorMoveBox = 9,
    WindowType_ColorTransparent = 10,
    WindowType_Layered = 11
};

class Window;

typedef LRESULT (*PFN_wndProc)(Window* pThis, UINT, WPARAM, LPARAM);
typedef void (*PFN_wndFunc)(Window* pThis, WPARAM, LPARAM);

class Window
{
protected:
    Rect m_windowRect;
    HWND m_hwnd = NULL;

    Window* m_dragWindow = nullptr;
    POINT m_dragPtStart = {};
    bool m_mouseDown = false;

    bool m_sizeable = false;

    vector<Window*> m_children;

    LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
    PFN_wndProc m_pWndProc = nullptr;
    WindowType m_type = WindowType_WndProc;

    DWORD m_color = 0x00000000;

    PFN_wndFunc m_WMCREATE = nullptr;
    PFN_wndFunc m_WMLBUTTONDOWN = nullptr;
    PFN_wndFunc m_WMLBUTTONUP = nullptr;
    PFN_wndFunc m_WMMOUSEMOVE = nullptr;
    PFN_wndFunc m_WMNCHITTEST = nullptr;
    PFN_wndFunc m_WMKEYUP = nullptr;

protected:
    bool Call(PFN_wndFunc pfnFunc, WPARAM wParam, LPARAM lParam)
    {
        if (pfnFunc != nullptr)
        {
            pfnFunc(this, wParam, lParam);
            return true;
        }
        return false;
    }
    static LRESULT CALLBACK s_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static constexpr PCWSTR s_className = L"DynamicWindow_v1";
    static bool s_classRegistered;

    static bool RegisterClass()
    {
        if (!s_classRegistered)
        {
            WNDCLASSW wc = {};

            wc.lpfnWndProc = Window::s_wndProc;
            wc.hInstance = g_hInstance;
            wc.lpszClassName = s_className;
            wc.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
            wc.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IMG_ICON));

            if (::RegisterClassW(&wc))
            {
                s_classRegistered = true;
            }
        }
        
        return s_classRegistered; 
    }
public:
    static const int s_ncborderSize = 4;
    Window()
    {
    }
    ~Window()
    {
    }

    static Window* Create(
        Rect point,
        WindowType type,
        Window* hwndParent = nullptr);

    static Window* Create(
        Rect point,
        WindowType type, 
        int image,
        Window* hwndParent = nullptr);

    static Window* Create(
        Rect rect,
        DWORD ExStyles, 
        DWORD styles, 
        PFN_wndProc pWndProc,
        Window* hwndParent = nullptr);

    static Window* Create(
        Rect rect,
        DWORD color,
        Window* hwndParent = nullptr);

    static Window* CreateSZ(
        Rect rect,
        DWORD color,
        Window* hwndParent = nullptr);

    static Window* Create(
        Rect rect,
        DWORD color,
        unsigned int transparency,
        Window* hwndParent = nullptr);

protected:
    void initilize(
        Window* hwndParent,
        WindowType wt,
        Rect rc, 
        PCWSTR windowName, 
        DWORD dwStyle = NULL, 
        DWORD dwExStyle = NULL);

public:
    HWND GetHWND() const
    {
        return m_hwnd;
    }

    void SetTitle(wstring title)
    {
        SetWindowText(m_hwnd, title.c_str());
    }

    void SetWindowPos(HWND hwndInsertAfter, Rect rc, UINT flags)
    {
        ::SetWindowPos(
            m_hwnd, 
            hwndInsertAfter, 
            rc.Point().x, 
            rc.Point().y,
            rc.Width(),
            rc.Height(),
            flags);
    }

    void SetDrag(Window* parent)
    {
        m_dragWindow = parent;
    }


    void OnClick(PFN_wndFunc func)
    {
        m_WMLBUTTONUP = func;
    }
    void OnMouseDown(PFN_wndFunc func)
    {
        m_WMLBUTTONDOWN = func;
    }
    void OnMouseMove(PFN_wndFunc func)
    {
        m_WMMOUSEMOVE = func;
    }
    void OnCreate(PFN_wndFunc func)
    {
        m_WMCREATE = func;
    }
    void OnKeyPress(PFN_wndFunc func)
    {
        m_WMKEYUP = func;
    }

    void Disable()
    {
        EnableWindow(m_hwnd, FALSE);
    }

    void Enable()
    {
        EnableWindow(m_hwnd, TRUE);
    }

    const Rect& GetRect()
    {
        return m_windowRect;
    }
};

class EditBox
{
private:
    HWND m_hwnd;
    Rect m_rc;
public:
    EditBox(Rect rc, wstring text, HWND hwndParent, DWORD style = 0, DWORD exStyle = WS_EX_CLIENTEDGE)
    {
        DWORD dwStyle = WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | WS_CLIPSIBLINGS | style;
        DWORD dwExStyle = WS_EX_NOPARENTNOTIFY | exStyle;

        m_hwnd = CreateWindowExW(
            dwExStyle,
            L"EDIT",
            text.c_str(),
            dwStyle,
            rc.Point().x,
            rc.Point().y,
            labs(rc.Width()),
            labs(rc.Height()),
            hwndParent,
            NULL,
            g_hInstance,
            null);
        ShowWindow(m_hwnd, SW_SHOW);
    }
    ~EditBox();
public:

    wstring GetText() const
    {
        wstring str;
        WCHAR arr[150] = {};
        GetWindowTextW(m_hwnd, arr, 150);
        str = arr;
        return str;
    }

    wstring SetText(wstring text)
    {
        wstring oldText = GetText();
        SetWindowTextW(m_hwnd, text.c_str());
        return oldText;
    }

    HWND GetHWND() const
    {
        return m_hwnd;
    }

    void SetWindowPos(HWND hwndInsertAfter, Rect rc, UINT flags)
    {
        ::SetWindowPos(
            m_hwnd, 
            hwndInsertAfter, 
            rc.Point().x, 
            rc.Point().y,
            rc.Width(),
            rc.Height(),
            flags);

        m_rc = rc;
    }

};
