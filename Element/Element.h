#pragma once
#include <vector>
#include <include/core/SkCanvas.h>
#include "Eventer.h"

class Element:public Eventer
{
public:
	Element();
	~Element();
	void paint(SkCanvas* canvas);
	void layout();
public:
	std::vector<Element> children;
	float x, y, w, h;

private:

};

