#pragma once
#include "Window/WindowBase.h"

class WindowMain:public WindowBase
{
public:
	WindowMain();
	~WindowMain();
private:
	void initWinPosSize();
};

