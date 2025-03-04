#include "Element/Element.h"
#include "WindowMain.h"


WindowMain::WindowMain()
{
    initWinPosSize();
    initWindow();
    alphaWindow();

    auto leftBar = std::make_shared<Element>();
    leftBar->id = CLAY_ID("leftBar");
    leftBar->bgColor = { .r{28}, .g{98}, .b{158}, .a{127} };
    leftBar->size.width = CLAY_SIZING_FIXED(360);
    leftBar->size.height = CLAY_SIZING_GROW(0);
    leftBar->direction = CLAY_TOP_TO_BOTTOM;
    body.children.push_back(std::move(leftBar));

    auto logo = std::make_shared<Element>();
    logo->id = CLAY_ID("logo");


    auto rightBar = std::make_shared<Element>();
    rightBar->id = CLAY_ID("rightBar");
    rightBar->bgColor = { .r{255}, .g{255}, .b{255}, .a{255} };
    rightBar->size.width = CLAY_SIZING_GROW(0);
    rightBar->size.height = CLAY_SIZING_GROW(0);
    body.children.push_back(std::move(rightBar));

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
