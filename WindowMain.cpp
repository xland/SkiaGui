#include "Element/Element.h"
#include "Element/Image.h"
#include "Element/Text.h"
#include "WindowMain.h"


WindowMain::WindowMain()
{
    initWinPosSize();
    initWindow();
    alphaWindow();
    setFlexDirection(YGFlexDirectionRow);

    auto leftBar = std::make_shared<Element>();
    leftBar->bgColor = 0X66FF0033;
    leftBar->setLayoutPadding(20.f);
    leftBar->setWidth(360.f);

    auto logo = std::make_shared<Element>();
    logo->bgColor = 0X66338833;
    logo->setWidth(160.f);
    logo->setHeight(160.f);
    leftBar->addChild(logo);

    auto contentBox = std::make_shared<Element>();
    contentBox->bgColor = 0XFFFFFFFF;
    contentBox->setFlex(1.f);
    addChild(leftBar);
    addChild(contentBox);
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
