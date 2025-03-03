#define CLAY_IMPLEMENTATION
#include "Lib/clay/clay.h"

#include <Windows.h>
#include "WindowMain.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{    
    WindowMain win;
    win.show();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}