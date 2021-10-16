PCWSTR LoadStringFromRegister(PCWSTR registerName, PCWSTR defaultValue);
HRESULT SaveStringToRegister(PCWSTR registerName, PCWSTR dataToSave);
long LoadlongFromRegister(PCWSTR registerName, long defaultValue);
HRESULT SaveLongToRegister(PCWSTR registerName, long dataToSave);
double LoadDoubleFromRegister(PCWSTR registerName, double defaultValue);
HRESULT SaveDoubleToRegister(PCWSTR registerName, double dataToSave);
