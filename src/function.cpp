#include "framework.h"
#include "HP11C.h"

void SetX(long double val)
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

void ExitEditMode()
{
    shiftStackOnEditMode = true;
    if (inEditMode)
    {
        inEditMode = false;
    }
}

// Numerical Keys
void Key0Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '0';
    UpdateDisplay();
}

void Key1Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '1';
    UpdateDisplay();
}

void Key2Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '2';
    UpdateDisplay();
}

void Key3Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '3';
    UpdateDisplay();
}

void Key4Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '4';
    UpdateDisplay();
}

void Key5Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '5';
    UpdateDisplay();
}

void Key6Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '6';
    UpdateDisplay();
}

void Key7Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '7';
    UpdateDisplay();
}

void Key8Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '8';
    UpdateDisplay();
}

void Key9Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();
    
    stackString += '9';
    UpdateDisplay();
}

void KeyDecimalPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    EnterEditMode();

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
        SetX(f_pi);
    }
    else if (GkeyActive) // Blue
    {
        GkeyActive = false;
        return;
    }
    else
    {
        if (stackString[0] != L'-')
        {
            wstring tempString = L"-";
            tempString += stackString;
            stackString = tempString;
        }
        else
        {
            stackString = stackString.substr(1, stackString.size());
        }
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
