#include "framework.h"

class Window;
void SetX(long double val);

// Numerical Keys
void Key0Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key1Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key2Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key3Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key4Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key5Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key6Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key7Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key8Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void Key9Press(Window* pThis, WPARAM wParam, LPARAM lParam);
void KeyDecimalPress(Window* pThis, WPARAM wParam, LPARAM lParam);

// Operataion Keys
void PlusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void MinusKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void TimesKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void DevideKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void SqrtKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void ChangeSignKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void OneOverX(Window* pThis, WPARAM wParam, LPARAM lParam);
void TenTotheX(Window* pThis, WPARAM wParam, LPARAM lParam);
void YtotheX(Window* pThis, WPARAM wParam, LPARAM lParam);

// Other keys
void EnterKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void BackspaceKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void XYReverseKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void RollDown(Window* pThis, WPARAM wParam, LPARAM lParam);
void STOkeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void RCLkeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void FKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
void GKeyPress(Window* pThis, WPARAM wParam, LPARAM lParam);
