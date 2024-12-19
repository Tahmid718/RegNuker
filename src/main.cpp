#include "enumerator.h"

#include <windows.h>
#include <iostream>

#define pass (void)0

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lcmdLine, int ncmdShow)
{
    LPCWSTR message = L"Warning. This very destructive script can cause devastating damage to your PC. This program can make your PC unstable to use or even unable to boot up, also makes it hard to fix the issue. If you still want to run, click yes. Or press no and nothing ever happens.";

    try
    {
        Beep(528, 500);
    }
    catch(...)
    {
        pass;
    }
    
    int MGW = MessageBoxW(NULL, message, L" Warning", MB_YESNO|MB_ICONWARNING);

    if (MGW == IDYES)
    {
        Registry_Enum RE_Obj;
        RE_Obj.Panel_();
    }

    return 0;
}
