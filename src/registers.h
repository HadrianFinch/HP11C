PCWSTR LoadStringFromRegister(PCWSTR registerName, PCWSTR defaultValue);
HRESULT SaveStringToRegister(PCWSTR registerName, PCWSTR dataToSave);
long LoadlongFromRegister(PCWSTR registerName, long defaultValue);
HRESULT SaveLongToRegister(PCWSTR registerName, long dataToSave);

#define REG_MAIN_LAST_X             L"RegisterSaveMainWindowLastX"
#define REG_MAIN_LAST_Y             L"RegisterSaveMainWindowLastY"

BOOL GetMonitorDevice(wstring adapterName, DISPLAY_DEVICE &ddMon );
BOOL GetMonitorSizeFromEDID(TCHAR* adapterName, DWORD& Width, DWORD& Height);
