#include "framework.h"
#include "HP11C.h"

void SetX(double val)
{
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
    stackString = text;
}

// Numerical Keys
void Key0Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '0';
    UpdateDisplay();
}

void Key1Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '1';
    UpdateDisplay();
}

void Key2Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '2';
    UpdateDisplay();
}

void Key3Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '3';
    UpdateDisplay();
}

void Key4Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '4';
    UpdateDisplay();
}

void Key5Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '5';
    UpdateDisplay();
}

void Key6Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '6';
    UpdateDisplay();
}

void Key7Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '7';
    UpdateDisplay();
}

void Key8Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '8';
    UpdateDisplay();
}

void Key9Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }
    
    stackString += '9';
    UpdateDisplay();
}

void KeyDecimalPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (!InEditMode)
    {
        InEditMode = true;
        stackString = L"";
    }

    bool alreadyUsed = (stackString.find('.') != wstring::npos);
    if (!alreadyUsed)
    {
        stackString += '.';
        UpdateDisplay();
    }
}

// Operation Keys
void PlusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    InEditMode = false;
    
    double x = stack[0];
    double y = stack[1];

    SetX(y + x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void MinusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    InEditMode = false;
    
    double x = stack[0];
    double y = stack[1];

    SetX(y - x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void TimesKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    InEditMode = false;
    
    double x = stack[0];
    double y = stack[1];

    SetX(y * x);

    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[4] = 0;

    UpdateDisplay();
}

void DevideKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    InEditMode = false;
    
    double x = stack[0];
    double y = stack[1];

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
        InEditMode = false;

        double square = stack[0] * stack[0];
        SetX(square);

        UpdateDisplay();
    }
    else
    {
        InEditMode = false;

        double root = sqrt(stack[0]);
        SetX(root);

        UpdateDisplay();
    }
}

// Other Keys
void EnterKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    InEditMode = false;

    stack[3] = stack[2];
    stack[2] = stack[1];
    stack[1] = stack[0];

    if (stackString.find('.') == wstring::npos)
    {
        stackString += L".00";
    }

    UpdateDisplay();
}

void BackspaceKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    if (stackString.size() > 1)
    {
        stackString = stackString.substr(0, stackString.size() - 1);
    }
    else
    {
        SetX(stack[1]);
    }
    UpdateDisplay();
}

void FKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    FkeyActive = true;
    GkeyActive = false;
}

void GKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    GkeyActive = true;
    FkeyActive = false;
}
