#pragma once
#include <vector>
#include <include/core/SkCanvas.h>
#include <include/core/SkRRect.h>
#include <yoga/Yoga.h>
#include "Eventer.h"

class Element:public Eventer
{
public:
	Element();
	~Element();
	void paint(SkCanvas* canvas);
	void addChild(std::shared_ptr<Element> ele);

	void setLayoutPadding(float padding);
	void setLayoutPadding(float left, float top, float right, float bottom);
	void setLayoutMargin(float margin);
	void setLayoutMargin(float left, float top, float right, float bottom);
	void setSize(float w, float h);
	void setWidth(float w);
	void setHeight(float h);
	void setAlignSelf(YGAlign align);
	void setFlexDirection(YGFlexDirection direction);
	void setFlex(float flex);
	void setJustifyContent(YGJustify justifyContent);
	void setRadius(float r);
	void setRadisu(float lt, float rt, float rb, float lb);
public:
	bool isDirty;
	SkColor bgColor{ 0X00000000 };
	float radiusLT, radiusRT, radiusRB, radiusLB;
	YGNodeRef node;
	std::vector<std::shared_ptr<Element>> children;
private:
};

