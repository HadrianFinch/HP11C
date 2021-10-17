#include "button.h"
#include "buttontypes.h"
#include "decode.h"
#include "files.h"
#include "function.h"
#include "layerwindow.h"
#include "MainWindow.h"
#include "registers.h"
#include "resources.h"
#include "window.h"

extern long double stack[4];
extern std::wstring stackString;
extern bool inEditMode;

extern bool FkeyActive;
extern bool GkeyActive;

extern int f_fix;

extern bool shiftStackOnEditMode;
