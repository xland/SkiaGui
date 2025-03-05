值得模仿的
https://www.cnblogs.com/sexintercourse/p/15850686.html

- 渲染图像
- 渲染文字
- 替换bgColor
- 渲染列表（重用样式？）




leftBar->size.width = 360;
    leftBar->size.height = CLAY_SIZING_GROW(0);
    leftBar->direction = CLAY_TOP_TO_BOTTOM;

    auto logoBox = std::make_shared<Element>();
    logoBox->id = CLAY_ID("logoBox");
    logoBox->size.width = CLAY_SIZING_GROW(0);
    logoBox->size.height = CLAY_SIZING_FIXED(80);
    logoBox->bgColor = { .r{118}, .g{98}, .b{68}, .a{127} };
    logoBox->direction = CLAY_LEFT_TO_RIGHT;
    logoBox->span = 12;
    logoBox->align = { .x{CLAY_ALIGN_X_CENTER },.y{CLAY_ALIGN_Y_CENTER } };

    auto logo = std::make_shared<Image>();
    logo->id = CLAY_ID("logo");
    logo->size.width = CLAY_SIZING_FIXED(60);
    logo->size.height = CLAY_SIZING_FIXED(60);
    logo->bgColor = { .r{118}, .g{98}, .b{168}, .a{127} };

    auto logo1 = std::make_shared<Image>();
    logo1->id = CLAY_ID("logo1");
    logo1->size.width = CLAY_SIZING_FIXED(60);
    logo1->size.height = CLAY_SIZING_FIXED(60);
    logo1->bgColor = { .r{118}, .g{98}, .b{168}, .a{127} };

    logoBox->children.push_back(logo); 
    logoBox->children.push_back(logo1);
    leftBar->children.push_back(logoBox);

    

    body.children.push_back(std::move(leftBar));


    auto rightBar = std::make_shared<Element>();
    rightBar->id = CLAY_ID("rightBar");
    rightBar->bgColor = { .r{255}, .g{255}, .b{255}, .a{255} };
    rightBar->size.width = CLAY_SIZING_GROW(0);
    rightBar->size.height = CLAY_SIZING_GROW(0);
    body.children.push_back(std::move(rightBar));

    layout();
    layout();