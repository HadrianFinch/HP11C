#include "framework.h"
#include "HP11C.h"

void SetX(long double val)
{
    // Update the stack
    stack[0] = val;

    // Update the stackString
    WCHAR formatSeed[10] = L"%.";
    WCHAR formatString[10] = {};
    StringCchPrintf(
        formatString,
        _countof(formatString),
        L"%s%df",
        formatSeed,
        f_fix);

    WCHAR text[80] = {};
    StringCchPrintf(
        text,
        _countof(text),
        formatString,
        val);

    wstring str = text;

    if ((val != val) or (str == L"inf"))
    {
        str = L"error";
    }   
    
    stackString = str;
}

void SetNewX(long double val)
{
    // Update the stack
    stack[3] = stack[2];
    stack[2] = stack[1];
    stack[1] = stack[0];

    SetX(val);
}

void StackStringToStack0()
{
    stack[0] = _wtof(stackString.c_str());
}

void EnterEditMode()
{
    if (!inEditMode)
    {
        inEditMode = true;
        stackString = L"";
        if (shiftStackOnEditMode)
        {
            stack[3] = stack[2];
            stack[2] = stack[1];
            stack[1] = stack[0];
        }
        else
        {
            shiftStackOnEditMode = true;
        }
    }
}

bool CheckSTORCLFIX(int numValue)
{
    bool ret = STOmodeActive or RCLmodeActive or FIXmodeActive;

    if (STOmodeActive)
    {
        STOmodeActive = false;

        wstring regName = L"STOvalue";
        regName += to_wstring(numValue);

        SavelongdoubleToRegister(regName.c_str(), stack[0]);
        UpdateDisplay();
    }
    else if (RCLmodeActive)
    {
        RCLmodeActive = false;

        stack[3] = stack[2];
        stack[2] = stack[1];
        stack[1] = stack[0];

        wstring regName = L"STOvalue";
        regName += to_wstring(numValue);

        SetNewX(LoadlongdoubleFromRegister(regName.c_str(), 0));
        UpdateDisplay();
    }
    else if (FIXmodeActive)
    {
        FIXmodeActive = false;

        f_fix = numValue;
        SaveLongToRegister(L"FIXvalue", f_fix);

        SetX(stack[0]);
        UpdateDisplay();
    }

    return ret;
}

void ExitEditMode()
{
    SetSecondDisplayText(L"");
    RCLmodeActive = false;
    STOmodeActive = false;
    FIXmodeActive = false;

    shiftStackOnEditMode = true;
    if (inEditMode)
    {
        inEditMode = false;
        SetX(_wtof(stackString.c_str()));
    }
}

// Numerical Keys
void Key0Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(0))
    {
        EnterEditMode();

        stackString += '0';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key1Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(1))
    {
        EnterEditMode();

        stackString += '1';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key2Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(2))
    {
        EnterEditMode();

        stackString += '2';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key3Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(3))
    {
        EnterEditMode();

        stackString += '3';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key4Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(4))
    {
        EnterEditMode();

        stackString += '4';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key5Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(5))
    {
        EnterEditMode();

        stackString += '5';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key6Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(6))
    {
        EnterEditMode();

        stackString += '6';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key7Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
        ExitEditMode();
        FIXmodeActive = !FIXmodeActive;
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
    }
    else
    {
        if (!CheckSTORCLFIX(7))
        {
            EnterEditMode();

            stackString += '7';
            StackStringToStack0();
            UpdateDisplay();
        }
    }
}

void Key8Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(8))
    {
        EnterEditMode();

        stackString += '8';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void Key9Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!CheckSTORCLFIX(9))
    {
        EnterEditMode();

        stackString += '9';
        StackStringToStack0();
        UpdateDisplay();
    }
}

void KeyDecimalPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();

    RCLmodeActive = false;
    STOmodeActive = false;

    bool alreadyUsed = (stackString.find('.') != wstring::npos);
    if (!alreadyUsed)
    {
        stackString += '.';
        StackStringToStack0();
        UpdateDisplay();
    }
}

// Operation Keys
void PlusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    
    long double x = stack[0];
    long double y = stack[1];

    SetX(y + x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void MinusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    
    long double x = stack[0];
    long double y = stack[1];

    SetX(y - x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void TimesKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    
    long double x = stack[0];
    long double y = stack[1];

    SetX(y * x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void DevideKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    
    long double x = stack[0];
    long double y = stack[1];

    SetX(y / x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void SqrtKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
        ExitEditMode();

        long double square = stack[0] * stack[0];
        SetX(square);

        UpdateDisplay();
    }
    else
    {
        ExitEditMode();

        long double root = sqrt(stack[0]);
        SetX(root);

        UpdateDisplay();
    }
}

void YtotheX(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;

        long double precent = stack[0] / 100;
        SetX(precent);

        UpdateDisplay();
    }
    else
    {
        long double result = pow(stack[1], stack[0]);
        SetX(result);

        UpdateDisplay();
    }
}

void ChangeSignKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
        ExitEditMode();
        SetNewX(f_pi);
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
        return;
    }
    else
    {
        SetX(-1 * stack[0]);
    }
    UpdateDisplay();
}

void TenTotheX(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    SetX(pow(10.0, stack[0]));
    UpdateDisplay();
}

void OneOverX(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    SetX(1 / stack[0]);
    UpdateDisplay();
}


// Other Keys
void EnterKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
        SetX((long double)rand() / (long double)RAND_MAX);
        UpdateDisplay();
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
    }
    else
    {
        stack[3] = stack[2];
        stack[2] = stack[1];
        stack[1] = stack[0];

        if (stackString.find('.') == wstring::npos)
        {
            stackString += L".";
            for (int i = 0; i < f_fix; i++)
            {
                stackString += L"0";
            }        
        }

        shiftStackOnEditMode = false;

        UpdateDisplay();
    }
}

void BackspaceKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (stackString.size() > 1)
    {
        stackString = stackString.substr(0, stackString.size() - 1);
    }
    else
    {
        ExitEditMode();
        SetX(stack[1]);
    }
    UpdateDisplay();
}

void XYReverseKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    long double x = stack[0];
    SetX(stack[1]);
    stack[1] = x;

    UpdateDisplay();
}

void RollDown(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (FkeyActive) // Orange
    {
        FkeyActive = false;
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
        ExitEditMode();

        long double stored = stack[3];

        stack[3] = stack[2];
        stack[2] = stack[1];
        stack[1] = stack[0];
        SetX(stored);

        UpdateDisplay();
    }
    else
    {
        ExitEditMode();

        long double x = stack[0];

        SetX(stack[1]);

        stack[1] = stack[2];
        stack[2] = stack[3];
        stack[3] = x;

        UpdateDisplay();
    }
}

void STOkeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    STOmodeActive = !STOmodeActive;
    UpdateDisplay();
}

void RCLkeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    ExitEditMode();
    RCLmodeActive = !RCLmodeActive;
    UpdateDisplay();
}

void FKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    SetSecondDisplayText(L"f");
    FkeyActive = true;
    GkeyActive = false;
}

void GKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    SetSecondDisplayText(L"g");
    GkeyActive = true;
    FkeyActive = false;
}
