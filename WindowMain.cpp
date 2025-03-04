#include "Element/Element.h"
#include "WindowMain.h"


WindowMain::WindowMain()
{
    initWinPosSize();
    initWindow();
    auto ele = std::make_shared<Element>();
    ele->id = CLAY_ID("banner");
    ele->bgColor = { .r{255}, .g{55}, .b{155}, .a{55} };
    ele->size.width = CLAY_SIZING_FIXED(50);
    ele->size.height = CLAY_SIZING_FIXED(250);
    elements.push_back(ele);
    layout();
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
