#include "framework.h"
#include "HP11C.h"

void MainWindowCreated(Window* pThis, WPARAM wParam, LPARAM lParam);
void CalculateDPI(Window* pThis, WPARAM wParam, LPARAM lParam);
void GetMonitorDPI(Window* pThis, WPARAM wParam, LPARAM lParam);

EditBox* pWidthInches;
EditBox* pHeightInches;
EditBox* pWidthPixels;
EditBox* pHeightPixels;

Window* pButtonBackground;
EditBox* pDPIBox;

void CreateMainWindow()
{
    Rect rc; rc.Point({350, 250}); rc.Width(500); rc.Height(300);

    Window* pMainWindow = Window::Create(rc, 0x001c1713);
    pMainWindow->SetTitle(L"HP 11C");

    rc.Height(88);
    rc.Width(484);
    rc.Point({8, 0});
    Window* pTopBar = Window::Create(rc, 0x00d8d8d8, pMainWindow);
    pTopBar->SetDrag(pMainWindow);

    rc.Point({440, 10});
    Window* pMinimize = Window::Create(rc, WindowType_Minimize, pTopBar);


    rc.Point({461, 10});
    Window* pClose = Window::Create(rc, WindowType_Close, pTopBar);

    vector<Window*> buttons;

    rc.Size({28, 28});
    for (long x = 17; x < 450; x += 48)
    {
        for (long y = 109; y < 257; y += 49)
        {
            Rect buttonRect = rc;
            buttonRect.Point({x, y});

            Window* pButton = Window::Create(buttonRect, WindowType_Layered, IMG_BUTTON, pMainWindow);
            pButton->SetWindowPos(HWND_TOP, buttonRect, 0);

            buttons.push_back(pButton);
        }
    }

    DestroyWindow(buttons[22]->GetHWND());
    DestroyWindow(buttons[23]->GetHWND());

    rc.Point({257, 207});
    rc.Size({28, 77});
    buttons[22] = Window::Create(rc, WindowType_Layered, IMG_ENTER_BUTTON, pMainWindow);
    buttons[23] = buttons[22];

    rc.Size({488, 204});
    rc.Point({7, 94});
    pButtonBackground = Window::Create(rc, WindowType_Layered, IMG_BUTTON_BACKGROUND, pMainWindow);
    pButtonBackground->SetWindowPos(HWND_TOP, rc, 0);
    
    rc.Size({453, 185});
    rc.Point({12, 4});
    Window* pOverlay = Window::Create(rc, WindowType_Layered, IMG_BUTTON_OVERLAY, pButtonBackground);

    rc.Size({254, 45});
    rc.Point({82, 21});
    pWidthInches = new EditBox(rc, L"0.00", pTopBar->GetHWND(), ES_READONLY);
    pWidthInches->SetWindowPos(HWND_TOP, rc, 0);

    // rc.Size({73, 22});
    // rc.Point({139, 143});
    // Window* pCalculate = Window::Create(rc, WindowType_Layered, MW_CALCULATE, pMainWindow);
    // pCalculate->SetWindowPos(HWND_TOP, rc, 0);
    
    // pMainWindow->OnCreate(MainWindowCreated);
    // pCalculate->OnClick(CalculateDPI);
    // pCaptureDPI->OnClick(GetMonitorDPI);
}

void MainWindowCreated(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    pThis->SetWindowPos(NULL, pThis->GetRect(), 0);
    
    // Rect rc;

    // rc.Width(134);
    // rc.Height(21);

    // rc.Point({22, 66});
    // pWidthInches = new EditBox(rc, L"", pThis->GetHWND());
    // pWidthInches->SetWindowPos(HWND_TOP, rc, 0);

    // rc.Point({22, 110});
    // pHeightInches = new EditBox(rc, L"", pThis->GetHWND());
    // pHeightInches->SetWindowPos(HWND_TOP, rc, 0);


    // rc.Point({193, 66});
    // pWidthPixels = new EditBox(rc, L"", pThis->GetHWND());
    // pWidthPixels->SetWindowPos(HWND_TOP, rc, 0);

    // rc.Point({193, 110});
    // pHeightPixels = new EditBox(rc, L"", pThis->GetHWND());
    // pHeightPixels->SetWindowPos(HWND_TOP, rc, 0);

    // rc.Width(125); rc.Height(23);
    // rc.Point({113, 199});
    // pDPIBox = new EditBox(rc, L"", pThis->GetHWND(), ES_READONLY);
    // pDPIBox->SetWindowPos(HWND_TOP, rc, 0);
}

void GetMonitorDPI(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    DWORD widthCM = 0;
    DWORD heightCM = 0;

    HMONITOR hmon;
    hmon = MonitorFromWindow(GetParent(GetParent(pThis->GetHWND())), MONITOR_DEFAULTTONEAREST);
    MONITORINFOEX monInfo = {};
    monInfo.cbSize = sizeof(monInfo);
    GetMonitorInfoW(hmon, &monInfo);
    Rect rcPixels = monInfo.rcMonitor;
    
    GetMonitorSizeFromEDID(
        monInfo.szDevice,
        heightCM,
        widthCM);

    double widthInches = (double)widthCM / 2.5;
    double heightInches = (double)heightCM / 2.5;

    WCHAR strWidthInches[35] = {};
    WCHAR strHeightInches[35] = {};

    WCHAR strWidthPixels[35] = {};
    WCHAR strHeightPixels[35] = {};

    swprintf(strWidthInches, L"%5.2f", widthInches);
    swprintf(strHeightInches, L"%5.2f", heightInches);

    _itow(rcPixels.Width(), strWidthPixels, 10);
    _itow(rcPixels.Height(), strHeightPixels, 10);

    pWidthInches->SetText(strWidthInches);
    pHeightInches->SetText(strHeightInches);

    pWidthPixels->SetText(strWidthPixels);
    pHeightPixels->SetText(strHeightPixels);

    CalculateDPI(pThis, 0, 0);
}

void CalculateDPI(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    double inchesDiagonal = 0;
    {
        double x = _wtof(pWidthInches->GetText().c_str());
        double y = _wtof(pHeightInches->GetText().c_str());

        inchesDiagonal = sqrt(((x * x) + (y * y)));
    }

    double pixelsDiagonal = 0;
    {
        double x = _wtof(pWidthPixels->GetText().c_str());
        double y = _wtof(pHeightPixels->GetText().c_str());

        pixelsDiagonal = sqrt(((x * x) + (y * y)));
    }

    if ((inchesDiagonal != 0) and (pixelsDiagonal != 0))
    {
        double DPI = pixelsDiagonal / inchesDiagonal;
        
        WCHAR text[100] = {};
        StringCchPrintf(
            text, 
            _countof(text), 
            L"%5.2f",
            DPI);
        pDPIBox->SetText(text);
    }
    else
    {
        pDPIBox->SetText(L"err");
    }
}
