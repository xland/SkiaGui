#include "Element.h"

Element::Element() : node{ YGNodeNew() }
{
}

Element::~Element()
{
    YGNodeFreeRecursive(node);
}

void Element::paint(SkCanvas* canvas)
{
    SkPaint paint;
    paint.setColor(bgColor);
    auto x = YGNodeLayoutGetLeft(node);
    auto y = YGNodeLayoutGetTop(node);
    auto w = YGNodeLayoutGetWidth(node);
    auto h = YGNodeLayoutGetHeight(node);
    if (bgColor != 0X00000000) {
        auto rect = SkRect::MakeXYWH(x, y, w, h);
        if (radiusLT > 0 || radiusRT > 0 || radiusRB > 0 || radiusLB > 0) {
            SkVector radii[4]{ {radiusLT, radiusLT}, {radiusRT, radiusRT}, {radiusRB, radiusRB}, {radiusLB, radiusLB} };
            SkRRect rr;
            rr.setRectRadii(rect, radii);
            canvas->drawRRect(rr, paint);
        }
        else {
            canvas->drawRect(rect, paint);
        }
    }
    for (auto& ele : children) {
        canvas->save();
        canvas->translate(x, y);
        ele->paint(canvas);
        canvas->restore();
    }
}

void Element::addChild(std::shared_ptr<Element> ele)
{
    auto index = YGNodeGetChildCount(node);
    YGNodeInsertChild(node, ele->node, index);
    children.push_back(ele);
}

void Element::setLayoutPadding(float padding)
{
	YGNodeStyleSetPadding(node, YGEdgeAll, padding);
}
void Element::setLayoutPadding(float left, float top, float right, float bottom)
{
	YGNodeStyleSetPadding(node, YGEdgeLeft, left);
	YGNodeStyleSetPadding(node, YGEdgeTop, top);
	YGNodeStyleSetPadding(node, YGEdgeRight, right);
	YGNodeStyleSetPadding(node, YGEdgeBottom, bottom);
}
void Element::setLayoutMargin(float margin)
{
	YGNodeStyleSetMargin(node, YGEdgeAll, margin);
}
void Element::setLayoutMargin(float left, float top, float right, float bottom)
{
	YGNodeStyleSetMargin(node, YGEdgeLeft, left);
	YGNodeStyleSetMargin(node, YGEdgeTop, top);
	YGNodeStyleSetMargin(node, YGEdgeRight, right);
	YGNodeStyleSetMargin(node, YGEdgeBottom, bottom);
}

void Element::setSize(float w, float h)
{
	YGNodeStyleSetWidth(node, w);
	YGNodeStyleSetHeight(node, h);
}
void Element::setWidth(float w)
{
	YGNodeStyleSetWidth(node, w);
}
void Element::setHeight(float h)
{
	YGNodeStyleSetHeight(node, h);
}
void Element::setAlignSelf(YGAlign align)
{
	YGNodeStyleSetAlignSelf(node,align);
}
void Element::setFlexDirection(YGFlexDirection direction)
{
	YGNodeStyleSetFlexDirection(node, direction);
}
void Element::setFlex(float flex)
{
    YGNodeStyleSetFlex(node, flex);
}
void Element::setJustifyContent(YGJustify justifyContent)
{
	YGNodeStyleSetJustifyContent(node, justifyContent);
}

void Element::setRadius(float r)
{
    radiusLT = r;
    radiusRT = r;
    radiusRB = r;
    radiusLB = r;
}

void Element::setRadisu(float lt, float rt, float rb, float lb)
{
    radiusLT = lt;
    radiusRT = rt;
    radiusRB = rb;
    radiusLB = lb;
}
