﻿#pragma once
#include <include/core/SkData.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkSurface.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFontMetrics.h>
#include <dwmapi.h>
#include <versionhelpers.h>
#include <windowsx.h>
#include <yoga/Yoga.h>
#include "../Element/Element.h"
#include "Backend.h"


class WindowBase:public Element
{
public:
	WindowBase();
	~WindowBase();
	void show();
	void layout();
public:
	int x, y, w, h;
	HWND hwnd;
protected:
	void initWindow();
	bool alphaWindow();
private:
	static LRESULT CALLBACK routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK processWinMsg(UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT hitTest(const int& x, const int& y);
private:
	std::unique_ptr<Backend> backend;
	YGConfigRef config;
};

