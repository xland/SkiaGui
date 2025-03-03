#include "../Lib/clay/clay.h"
#include "Element.h"

Element::Element()
{
}

Element::~Element()
{
}

void Element::paint(SkCanvas* canvas)
{

}

void Element::layout()
{
    auto padding = CLAY_PADDING_ALL(8);
    Clay_LayoutConfig layout{ .padding{padding} };
    Clay_ElementDeclaration config{ .layout{layout} };
    CLAY(config) {
        for (auto& ele : children) {
            ele.layout();
        }
    };
}
