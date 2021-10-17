#include "framework.h"

PCWSTR LoadStringFromRegister(PCWSTR registerName, PCWSTR defaultValue)
{
    HKEY hkey = NULL;
    bool loaded = false;

    WCHAR* output[400] = {};

    auto result = RegOpenKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        KEY_QUERY_VALUE,
        &hkey);
    if (result == ERROR_SUCCESS)
    {
        DWORD dwType;
        DWORD cbData = sizeof(output);

        cbData = sizeof(output);
        result = RegQueryValueEx(
            hkey,
            registerName,
            NULL,
            &dwType,
            reinterpret_cast<LPBYTE>(output),
            &cbData);

        if ((result == ERROR_SUCCESS) and
            (dwType == REG_SZ))
        {
            loaded = true;
        }
    }
    
    if (!loaded)
    {
        return defaultValue;
    }
    
    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    
    return reinterpret_cast<PCWSTR>(&output[0]);
}

HRESULT SaveStringToRegister(PCWSTR registerName, PCWSTR dataToSave)
{
    HKEY hkey = NULL;
    auto result = RegCreateKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,
        &hkey,
        NULL);
    if (result == ERROR_SUCCESS)
    {
        size_t cbSize;

        StringCbLengthW(
            dataToSave,
            (MAX_PATH * sizeof(WCHAR)),
            &cbSize);

        RegSetValueEx(
            hkey,
            registerName,
            0,
            REG_SZ,
            reinterpret_cast<LPBYTE>(const_cast<WCHAR*>(dataToSave)),
            (cbSize + sizeof(WCHAR)));
    }

    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    else
    {
        return E_FAIL;
    }
    
    return S_OK;
}


long LoadlongFromRegister(PCWSTR registerName, long defaultValue)
{
    HKEY hkey = NULL;
    bool loaded = false;

    long dwOutput = 0;

    auto result = RegOpenKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        KEY_QUERY_VALUE,
        &hkey);
    if (result == ERROR_SUCCESS)
    {
        DWORD dwType;
        DWORD cbData = sizeof(DWORD);

        result = RegQueryValueEx(
            hkey,
            registerName,
            NULL,
            &dwType,
            reinterpret_cast<LPBYTE>(&dwOutput),
            &cbData);

        if ((result == ERROR_SUCCESS) and
            (dwType == REG_DWORD))
        {
            loaded = true;
        }
    }
    
    if (!loaded)
    {
        return defaultValue;
    }
    
    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    
    return dwOutput;
}

HRESULT SaveLongToRegister(PCWSTR registerName, long dataToSave)
{
    HKEY hkey = NULL;
    auto result = RegCreateKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,
        &hkey,
        NULL);
    if (result == ERROR_SUCCESS)
    {
        RegSetValueEx(
            hkey,
            registerName,
            0,
            REG_DWORD,
            reinterpret_cast<LPBYTE>(&dataToSave),
            sizeof(DWORD));
    }

    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    else
    {
        return E_FAIL;
    }
    
    return S_OK;
}

HRESULT SavelongdoubleToRegister(PCWSTR registerName, long double dataToSave)
{
    HKEY hkey = NULL;
    auto result = RegCreateKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,
        &hkey,
        NULL);
    if (result == ERROR_SUCCESS)
    {
        RegSetValueEx(
            hkey,
            registerName,
            0,
            REG_BINARY,
            reinterpret_cast<LPBYTE>(&dataToSave),
            sizeof(long double));
    }

    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    else
    {
        return E_FAIL;
    }
    
    return S_OK;
}

long double LoadlongdoubleFromRegister(PCWSTR registerName, long double defaultValue)
{
    HKEY hkey = NULL;
    bool loaded = false;

    long double output = 0;

    auto result = RegOpenKeyEx(
        HKEY_CURRENT_USER,
        HKEY_PROJECT_TARGET,
        0,
        KEY_QUERY_VALUE,
        &hkey);
    if (result == ERROR_SUCCESS)
    {
        DWORD dwType;
        DWORD cbData = sizeof(long double);

        result = RegQueryValueEx(
            hkey,
            registerName,
            NULL,
            &dwType,
            reinterpret_cast<LPBYTE>(&output),
            &cbData);

        if ((result == ERROR_SUCCESS) and
            (dwType == REG_BINARY))
        {
            loaded = true;
        }
    }
    
    if (!loaded)
    {
        return defaultValue;
    }
    
    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    
    return output;
}
