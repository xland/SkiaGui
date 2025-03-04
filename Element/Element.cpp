#include "Element.h"

Element::Element(){

}

Element::~Element()
{
}

void Element::paint(SkCanvas* canvas)
{
    SkPaint paint;
    paint.setColor(SkColorSetARGB(bgColor.a, bgColor.r, bgColor.g, bgColor.b));
    auto rect = SkRect::MakeXYWH(x, y, w, h);
    if (radius.topLeft > 0 || radius.topRight > 0 || radius.bottomLeft > 0 || radius.bottomRight > 0) {
        SkVector radii[4]{ {radius.topLeft, radius.topLeft}, {radius.topRight, radius.topRight},
            {radius.bottomRight, radius.bottomRight}, {radius.bottomLeft, radius.bottomLeft}
        };
        SkRRect rr;
        rr.setRectRadii(rect, radii);
        canvas->drawRRect(rr, paint);
    }
    else {
        canvas->drawRect(rect, paint);
    }
    
}

void Element::layout()
{
    Clay_LayoutConfig layout{ 
        .sizing{size},
        .padding{padding},
        .childGap{span},
        .childAlignment{align},
        .layoutDirection{direction},
    };
    Clay_ElementDeclaration config{
        .id{id},
        .layout{layout},
        .backgroundColor{bgColor},
        .cornerRadius{radius},  
        .userData{this}
    };
    CLAY(config) {
        for (auto& ele : children) {
            ele->layout();
        }
    };
}
