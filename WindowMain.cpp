#include "WindowMain.h"

WindowMain::WindowMain()
{
    initWinPosSize();
    initWindow();
}

WindowMain::~WindowMain()
{
}

void WindowMain::initWinPosSize()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    w = 1200;
    h = 800;
    x = (screenWidth - w) / 2;
    y = (screenHeight - h) / 2;
}
