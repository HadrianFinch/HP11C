#pragma once
#include <windows.h>

typedef struct tagHRECT
{
    POINT point;
    SIZE size;
} HRECT, *PHRECT;

enum Alignment
{
    align_default = 1,

    align_left = 1,
    align_right = 2,
    
    align_top = 1,
    align_bottom = 2,

    align_center = 3,
    align_middle = 3
};


class Rect
{
private:
    RECT m_rect;

    Alignment m_horizontalAlignment = align_default;
    Alignment m_verticalAlignment = align_default;
public:
    static const long SIZE_FULL = -21l;
    Rect(tagRECT rc)
    {
        m_rect = rc;
    }
    Rect(tagHRECT hrc, Alignment h, Alignment v)
    {
        m_rect = {hrc.point.x, hrc.point.y, (hrc.point.x + hrc.size.cx), (hrc.point.y + hrc.size.cy)};
        m_horizontalAlignment = h;
        m_verticalAlignment = v;
    }
    Rect()
    {
        m_rect = {};
    }
    
    ~Rect() {}

    HRECT HRECT() const
    {
        tagHRECT hrc = {};
        hrc.point = Point();
        hrc.size = Size();
        return hrc;
    }
    void HRECT(tagHRECT hrc)
    {
        Point(hrc.point);
        Size(hrc.size);
    }

    RECT RECT() const
    {
        return m_rect;
    }
    PRECT RectPointer()
    {
        return &m_rect;
    }
    void RECT(tagRECT rc)
    {
        m_rect = rc;
    }

    POINT Point() const
    {
        return {m_rect.left, m_rect.top};
    }
    void  Point(POINT pt)
    {
        long width = Width();
        long height = Height();
        m_rect.left = pt.x;
        m_rect.top = pt.y;
        Width(width);
        Height(height);
    }

    POINT BottomRight() const
    {
        return {m_rect.right, m_rect.bottom};
    }
    void  BottomRight(POINT pt)
    {
        m_rect.right = pt.x;
        m_rect.bottom = pt.y;
    }

    long Width() const
    {
        return (m_rect.right - m_rect.left);
    }
    void Width(long width)
    {
        m_rect.right = (m_rect.left + width);
    }
    
    long Height() const
    {
        return (m_rect.bottom - m_rect.top);
    }
    void Height(long height)
    {
        m_rect.bottom = (m_rect.top + height);
    }

    long CenterOf_Width() const
    {
        return m_rect.left + (Width() / 2);
    }
    long CenterOf_Height() const
    {
        return m_rect.top + (Height() / 2);
    }

    POINT Middle() const
    {
        return {CenterOf_Width(), CenterOf_Height()};
    }
    POINT MiddleCalabrate(SIZE size) const
    {
        return {CenterOf_Width() - (size.cx / 2), CenterOf_Height() - (size.cy / 2)};
    }
    
    SIZE Size() const
    {
        return {Width(), Height()};
    }
    void Size(SIZE size)
    {
        Width(size.cx);
        Height(size.cy);
    }

    void Normalize()
    {
        tagRECT outputRect;

        outputRect.left = min(m_rect.left, m_rect.right);
        outputRect.top = min(m_rect.top, m_rect.bottom);
        outputRect.right = max(m_rect.left, m_rect.right);
        outputRect.bottom = max(m_rect.top, m_rect.bottom);

        m_rect = outputRect;
    }

    void ClientToScreen(HWND hwnd)
    {
        POINT tmpPt = Point();
        ::ClientToScreen(hwnd, &tmpPt);
        Point(tmpPt);
    }

    bool IsPointInRect(tagPOINT pt)
    {
        bool is = true;
        if (!(pt.x > Point().x))
        {
            is = false;
        }
        if (!(pt.y > Point().y))
        {
            is = false;
        }
        if (!(pt.x < BottomRight().x))
        {
            is = false;
        }
        if (!(pt.y < BottomRight().y))
        {
            is = false;
        }
        return is;
    }
    
    void ScreenToClient(HWND hwnd)
    {
        POINT tmpPt = Point();
        ::ScreenToClient(hwnd, &tmpPt);
        Point(tmpPt);
    }

    // ⚠️This will override current values⚠️
    void GetWindowRect(HWND hwnd)
    {
        ::GetWindowRect(hwnd, &m_rect);
    }

    // ⚠️This will override current values⚠️
    void GetClientRect(HWND hwnd)
    {
        ::GetClientRect(hwnd, &m_rect);
    }

    void SetAlignment(Alignment h, Alignment v)
    {
        m_horizontalAlignment = h;
        m_verticalAlignment = v;
    }
    Alignment GetHorizontalAlignment()
    {
        return m_horizontalAlignment;
    }
    Alignment GetVerticalAlignment()
    {
        return m_verticalAlignment;
    }
    
};
