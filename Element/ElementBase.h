#pragma once
#include <vector>
#include <include/core/SkCanvas.h>
#include "Eventer.h"

class ElementBase:public Eventer
{
public:
	ElementBase();
	~ElementBase();
	void paint(SkCanvas* canvas);
public:
	std::vector<ElementBase> children;
private:

};

