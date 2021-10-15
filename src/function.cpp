#include "framework.h"
#include "HP11C.h"

void Key1Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    stack[0] = stack[0] * 10;
    stack[0] += 1;

    UpdateDisplay();
}

void Key2Press(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    stack[0] = stack[0] * 10;
    stack[0] += 2;

    UpdateDisplay();
}

void SqrtKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam)
{
    stack[0] = sqrt(stack[0]);

    UpdateDisplay();
}
