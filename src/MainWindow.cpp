#include "framework.h"
#include "HP11C.h"

void MainWindowCreated(Window* pThis, WPARAM wParam, LPARAM lParam);
void CalculateDPI(Window* pThis, WPARAM wParam, LPARAM lParam);
void GetMonitorDPI(Window* pThis, WPARAM wParam, LPARAM lParam);

EditBox* pDisplayBox;
EditBox* pHeightInches;
EditBox* pWidthPixels;
EditBox* pHeightPixels;

Window* pButtonBackground;
EditBox* pDPIBox;

void CreateMainWindow()
{
    Rect rc; rc.Point({350, 250}); rc.Width(500); rc.Height(300);

    Window* pMainWindow = Window::Create(rc, 0x001c1713);
    pMainWindow->SetTitle(L"HP 11C Calculator");

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
            pButton->OnClick(Key1Press);
        }
    }

    buttons[0]->OnClick(SqrtKeyPress);

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
    pDisplayBox = new EditBox(rc, L"0.00", pTopBar->GetHWND(), ES_READONLY);
    pDisplayBox->SetWindowPos(HWND_TOP, rc, 0);

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
}

void UpdateDisplay(void)
{
    WCHAR text[50] = {};
    StringCchPrintf(
        text, 
        _countof(text), 
        L"%.2f",
        stack[0]);
    pDisplayBox->SetText(text);
}
