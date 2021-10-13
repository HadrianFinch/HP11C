#include "framework.h"

BOOL GetMonitorDevice(wstring adapterName, DISPLAY_DEVICE &ddMon )
{
    DWORD devMon = 0;

    while (EnumDisplayDevices(adapterName.c_str(), devMon, &ddMon, 0))
    {
        if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE &&
            ddMon.StateFlags & DISPLAY_DEVICE_ATTACHED) // for ATI, Windows XP
            break;

        devMon++;
    }

    if (ddMon.DeviceString[0] == '\0')
    {
        EnumDisplayDevices(adapterName.c_str(), 0, &ddMon, 0);
        if (ddMon.DeviceString[0] == '\0')
            wcscpy_s(ddMon.DeviceString, L"Default Monitor");
    }
    return ddMon.DeviceID[0] != '\0';
}

BOOL GetMonitorSizeFromEDID(WCHAR* adapterName, DWORD& Width, DWORD& Height)
{
    DISPLAY_DEVICE ddMon;
    ZeroMemory(&ddMon, sizeof(ddMon));
    ddMon.cb = sizeof(ddMon);

    //read edid
    bool result = false;
    Width = 0;
    Height = 0;
    if (GetMonitorDevice(adapterName, ddMon))
    {
        WCHAR model[8];
        WCHAR* s = wcschr(ddMon.DeviceID, '\\') + 1;
        size_t len = wcschr(s, '\\') - s;
        if (len >= _countof(model))
            len = _countof(model) - 1;
        wcsncpy_s(model, s, len);

        WCHAR *path = wcschr(ddMon.DeviceID, '\\') + 1;
        WCHAR str[MAX_PATH] = L"SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\";
        wcsncat_s(str, path, wcschr(path, '\\')-path);
        path = wcschr(path, '\\') + 1;
        HKEY hKey;
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, str, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            DWORD i = 0;
            DWORD size = MAX_PATH;
            FILETIME ft;
            while(RegEnumKeyEx(hKey, i, str, &size, NULL, NULL, NULL, &ft) == ERROR_SUCCESS)
            {
                HKEY hKey2;
                if(RegOpenKeyEx(hKey, str, 0, KEY_READ, &hKey2) == ERROR_SUCCESS)
                {
                    size = MAX_PATH;
                    if(RegQueryValueEx(hKey2, L"Driver", NULL, NULL, (LPBYTE)&str, &size) == ERROR_SUCCESS)
                    {
                        if (wcscmp(str, path) == 0)
                        {
                            HKEY hKey3;
                            if(RegOpenKeyEx(hKey2, L"Device Parameters", 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
                            {
                                BYTE EDID[256];
                                size = 256;
                                if(RegQueryValueEx(hKey3, L"EDID", NULL, NULL, (LPBYTE)&EDID, &size) == ERROR_SUCCESS)
                                {
                                    DWORD p = 8;
                                    WCHAR model2[9];

                                    char byte1 = EDID[p];
                                    char byte2 = EDID[p+1];
                                    model2[0]=((byte1 & 0x7C) >> 2) + 64;
                                    model2[1]=((byte1 & 3) << 3) + ((byte2 & 0xE0) >> 5) + 64;
                                    model2[2]=(byte2 & 0x1F) + 64;
                                    swprintf(model2 + 3, L"%X%X%X%X", (EDID[p+3] & 0xf0) >> 4, EDID[p+3] & 0xf, (EDID[p+2] & 0xf0) >> 4, EDID[p+2] & 0x0f);
                                    if (wcscmp(model, model2) == 0)
                                    {
                                        Width = EDID[22];
                                        Height = EDID[21];
                                        result = true;
                                    }
                                    else
                                    {
                                        // EDID incorrect
                                    }
                                }
                                RegCloseKey(hKey3);
                            }
                        }
                    }
                    RegCloseKey(hKey2);
                }
                i++;
            }
            RegCloseKey(hKey);
        }
    }

    return result;
}

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
