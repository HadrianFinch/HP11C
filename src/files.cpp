#include "framework.h"

void ReadAndPrintFile(PCWSTR filepath)
{
    HANDLE file = NULL;
    file = CreateFile(
        filepath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (file == NULL)
    {
        return;
    }

    char contents[1000] = {};
    DWORD cbRead = 0;

    bool bRet = !!ReadFile(
        file,
        contents,
        sizeof(contents),
        &cbRead,
        NULL);

    if (bRet)
    {
        MessageBoxA(NULL, contents, "File Contents", MB_OK);
    }
    else
    {
        MessageBox(NULL, L"Read File FAILED", L"ERROR", MB_OK | MB_ICONERROR);
    }

    CloseHandle(file);
}


#ifdef PHOTOEDITOR

ColorPalette ReadColorPaletteFile(PCWSTR filepath)
{
    HANDLE hFile = NULL;
    hFile = CreateFile(
        filepath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hFile == NULL)
    {
        return {};
    }

    char contents[1000] = {};
    DWORD cbRead = 0;

    bool bRet = !!ReadFile(
        hFile,
        contents,
        sizeof(contents),
        &cbRead,
        NULL);

    ColorPalette palette = {};

    if (bRet)
    {
        char* c = contents;
        int lineNum = 0;
        char* start = contents;
        bool firstSpace = false;
        while (*c != '\0')
        {
            if ((*c == ' ') and
                (!firstSpace))
            {
                *c = '\0';
                palette.colors[lineNum].colorCode = (int)strtol(start, NULL, 16);
                c++; c++;

                start = c;

                firstSpace = true;
            }
            else if (*c == '\n')
            {
                // A new line has been reached
                *c = '\0';

                StringCchCopyA(
                    palette.colors[lineNum].colorName,
                    _countof(palette.colors[lineNum].colorName),
                    start);

                firstSpace = false;
                start = c + 1;
                lineNum++;
            }
            else if (*c == '\r')
            {
                *c = '\0';
            }
            
            c++;
        }
        palette.paletteName = PathFindFileName(filepath);
    }

    CloseHandle(hFile);

    return palette;
}

void ImportColorPalette()
{
    PCWSTR filePath = OpenPLTfile();

    if (filePath == NULL)
    {
        return;
    }

    ColorPalette cp = ReadColorPaletteFile(filePath);

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
            L"Imported Color Palette",
            0,
            REG_BINARY,
            reinterpret_cast<LPBYTE>(&cp),
            sizeof(ColorPalette));

        WCHAR text[100] = {};
        StringCchPrintfW(
            text,
            _countof(text),
            L"The installation of the color palette \"%s\" is complete",
            cp.paletteName);
        ShowBaloon(
            L"Color Palette Installed", 
            text, 
            IMG_PALETTE);
    }

    if (hkey != NULL)
    {
        RegCloseKey(hkey);
    }
    else
    {
        return;
    }
}

#endif
