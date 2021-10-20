#include "framework.h"
#include "HP11C.h"

void MainWindowCreated(Window* pThis, WPARAM wParam, LPARAM lParam);
void CalculateDPI(Window* pThis, WPARAM wParam, LPARAM lParam);
void GetMonitorDPI(Window* pThis, WPARAM wParam, LPARAM lParam);
void KeyboardInputController(Window* pThis, WPARAM wParam, LPARAM lParam);

EditBox* pDisplayBox;
EditBox* pSecondDisplayBox;
Window* pButtonBackground;

void CreateMainWindow()
{
    Rect rc; rc.Point({350, 250}); rc.Width(500); rc.Height(300);

    Window* pMainWindow = Window::Create(rc, 0x001c1713);
    pMainWindow->SetTitle(L"HP 11C Calculator");
    pMainWindow->OnKeyPress(KeyboardInputController);

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
    Rect buttonRect = rc;
    for (long x = 17; x < 450; x += 48)
    {
        for (long y = 109; y < 257; y += 49)
        {
            buttonRect.Point({x, y});

            Window* pButton = Window::Create(buttonRect, WindowType_Layered, IMG_BUTTON, pMainWindow);
            pButton->SetWindowPos(HWND_TOP, buttonRect, 0);

            buttons.push_back(pButton);
        }
    }


    // Numerical Keys
    buttons[27]->OnClick(Key0Press);
    buttons[26]->OnClick(Key1Press);
    buttons[30]->OnClick(Key2Press);
    buttons[34]->OnClick(Key3Press);
    buttons[25]->OnClick(Key4Press);
    buttons[29]->OnClick(Key5Press);
    buttons[33]->OnClick(Key6Press);
    buttons[24]->OnClick(Key7Press);
    buttons[28]->OnClick(Key8Press);
    buttons[32]->OnClick(Key9Press);
    buttons[31]->OnClick(KeyDecimalPress);

    // Operation Keys
    buttons[39]->OnClick(PlusKeyPress);
    buttons[38]->OnClick(MinusKeyPress);
    buttons[37]->OnClick(TimesKeyPress);
    buttons[36]->OnClick(DevideKeyPress);
    buttons[0]->OnClick(SqrtKeyPress);
    buttons[20]->OnClick(ChangeSignKeyPress);
    buttons[16]->OnClick(OneOverX);
    buttons[12]->OnClick(YtotheX);
    buttons[8]->OnClick(TenTotheX);

    // Other Keys
    buttons[18]->OnClick(BackspaceKeyPress);
    buttons[14]->OnClick(XYReverseKeyPress);
    buttons[10]->OnClick(RollDown);
    buttons[15]->OnClick(STOkeyPress);
    buttons[19]->OnClick(RCLkeyPress);

    // fix the F button
    buttonRect = buttons[7]->GetRect();
    DestroyWindow(buttons[7]->GetHWND());
    buttons[7] = Window::Create(buttonRect, WindowType_Layered, IMG_FBUTTON, pMainWindow);
    buttons[7]->OnClick(FKeyPress);

    // fix the G button
    buttonRect = buttons[11]->GetRect();
    DestroyWindow(buttons[11]->GetHWND());
    buttons[11] = Window::Create(buttonRect, WindowType_Layered, IMG_GBUTTON, pMainWindow);
    buttons[11]->OnClick(GKeyPress);

    // Fix the enter key
    DestroyWindow(buttons[22]->GetHWND());
    DestroyWindow(buttons[23]->GetHWND());
    rc.Point({257, 207});
    rc.Size({28, 77});
    buttons[22] = Window::Create(rc, WindowType_Layered, IMG_ENTER_BUTTON, pMainWindow);
    buttons[22]->OnClick(EnterKeyPress);
    buttons[23] = buttons[22];

    // add the button background
    rc.Size({488, 204});
    rc.Point({7, 94});
    Window* pButtonBackground = Window::Create(rc, WindowType_Layered, IMG_BUTTON_BACKGROUND, pMainWindow);
    pButtonBackground->SetWindowPos(HWND_TOP, rc, 0);
    
    // add the button overlay
    rc.Size({453, 185});
    rc.Point({12, 4});
    Window* pOverlay = Window::Create(rc, WindowType_Layered, IMG_BUTTON_OVERLAY, pButtonBackground);

    // Add the display Container
    rc.Size({260, 45});
    rc.Point({82, 21});
    Window* pDisplayBackground = Window::Create(rc, 0x005d553f, pTopBar);

    // add the display
    rc.Size({235, 25});
    rc.Point({12, 12});
    pDisplayBox = new EditBox(rc, stackString, pDisplayBackground->GetHWND(), ES_READONLY, 0);
    pDisplayBox->SetWindowPos(HWND_TOP, rc, 0);

    rc.Size({100, 10});
    rc.Point({12, 35});
    pSecondDisplayBox = new EditBox(rc, L"", pDisplayBackground->GetHWND(), ES_READONLY, 0);
    pSecondDisplayBox->SetWindowPos(HWND_TOP, rc, 0);

    HRSRC hResource = FindResource(
        g_hInstance,
        MAKEINTRESOURCE(FONT_7SEGMENT),
        L"BINARY");

    if (hResource)
    {
        HGLOBAL hGlobal = LoadResource(g_hInstance, hResource);
        if (hGlobal != nullptr)
        {
            void* fontData = LockResource(hGlobal);
            DWORD numFonts = 0;
            DWORD size = SizeofResource(g_hInstance, hResource);
            HANDLE hFakeFont = AddFontMemResourceEx(
                fontData, 
                size, 
                nullptr, 
                &size);
            
            HFONT hFont = CreateFontW(
                22, // font size
                0, // Auto width
                0,
                0,
                FW_BOLD,
                true, // italic
                false, // underline
                false, // strikethrough
                DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                VARIABLE_PITCH,
                L"DSEG7 Classic");
            
            SendMessageW(
                pDisplayBox->GetHWND(),
                WM_SETFONT,
                (WPARAM)hFont,
                TRUE);

            hFont = CreateFontW(
                9, // font size
                0, // Auto width
                0,
                0,
                FW_NORMAL,
                false, // italic
                false, // underline
                false, // strikethrough
                DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                VARIABLE_PITCH,
                L"Consolas");

            SendMessageW(
                pSecondDisplayBox->GetHWND(),
                WM_SETFONT,
                (WPARAM)hFont,
                TRUE);
        }   
    }
}

void MainWindowCreated(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    pThis->SetWindowPos(NULL, pThis->GetRect(), 0);
}

void KeyboardInputController(Window* pThis, WPARAM keycode, LPARAM lParam)
{
    switch (keycode)
    {
        // Numerical Keys
        case 0x30: // 0 Key
            Key0Press(nullptr, 0, 0);
        break;
        
        case 0x31: // 1 Key
            Key1Press(nullptr, 0, 0);
        break;
        
        case 0x32: // 2 Key
            Key2Press(nullptr, 0, 0);
        break;
        
        case 0x33: // 3 Key
            Key3Press(nullptr, 0, 0);
        break;
        
        case 0x34: // 4 Key
            Key4Press(nullptr, 0, 0);
        break;
        
        case 0x35: // 5 Key
            Key5Press(nullptr, 0, 0);
        break;
        
        case 0x36: // 6 Key
            Key6Press(nullptr, 0, 0);
        break;
        
        case 0x37: // 7 Key
            Key7Press(nullptr, 0, 0);
        break;
        
        case 0x38: // 8 Key or * key with shift
        {
            short keyState = GetKeyState(VK_SHIFT);
            if (keyState != 0)
            {
                TimesKeyPress(nullptr, 0, 0);
            }
            else
            {
                Key8Press(nullptr, 0, 0);
            }            
        }
        break;
        
        case 0x39: // 9 Key
            Key9Press(nullptr, 0, 0);
        break;
        
        case VK_OEM_PERIOD: // . Key
            KeyDecimalPress(nullptr, 0, 0);
        break;

        // Operation Keys
        case VK_OEM_PLUS: // + Key
            PlusKeyPress(nullptr, 0, 0);
        break;

        case VK_OEM_MINUS: // - Key
            MinusKeyPress(nullptr, 0, 0);
        break;

        case VK_OEM_2: // / Key
            DevideKeyPress(nullptr, 0, 0);
        break;

        // Other Keys
        case VK_RETURN: // enter Key
            EnterKeyPress(nullptr, 0, 0);
        break;

        case VK_BACK: // enter Key
            BackspaceKeyPress(nullptr, 0, 0);
        break;
    }    
}

void UpdateDisplay(void)
{
    // stack[0] = _wtof(stackString.c_str()); // Moved to SetX()
    pDisplayBox->SetText(stackString);
}

void SetSecondDisplayText(std::wstring text)
{
    pSecondDisplayBox->SetText(text);
}
