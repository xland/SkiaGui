#pragma once
#include <vector>
#include <include/core/SkCanvas.h>
#include <include/core/SkRRect.h>
#include "../Lib/clay/clay.h"
#include "Eventer.h"

class Element:public Eventer
{
public:
	Element();
	~Element();
	void paint(SkCanvas* canvas);
	void layout();
public:
	std::vector<std::shared_ptr<Element>> children;
	float x,y,w,h;
	bool isDirty;
	Clay_ElementId id;
	Clay_Padding padding{ CLAY_PADDING_ALL(2) };
	Clay_Sizing size;
	Clay_LayoutDirection direction{ CLAY_LEFT_TO_RIGHT };
	Clay_ChildAlignment align{ CLAY_ALIGN_X_LEFT };
	uint16_t span{6};
	Clay_Color bgColor{ .r{255}, .g{255}, .b{255}, .a{255} };
	Clay_CornerRadius radius{ .topLeft{0}, .topRight{0}, .bottomLeft{0}, .bottomRight{0} };
private:

};

