#pragma once
#include "Window/WindowBase.h"
#include <yoga/Yoga.h>

class WindowMain:public WindowBase
{
public:
	WindowMain();
	~WindowMain();
private:
	void initWinPosSize();
};

