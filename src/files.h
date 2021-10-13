
struct ColorPaletteEntry
{
    int colorCode;
    char colorName[35];
};

struct ColorPalette
{
    ColorPaletteEntry colors[50];
    PWSTR paletteName = {};
};

void ReadAndPrintFile(PCWSTR filepath);
ColorPalette ReadColorPaletteFile(PCWSTR filepath);
void ImportColorPalette();
