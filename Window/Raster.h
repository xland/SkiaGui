#pragma once

#include "Backend.h"

class WindowBase;
class Raster:public Backend
{
public:
	Raster(WindowBase* win);
	~Raster();
	static Raster* create(WindowBase* win);
	void resize() override;
	sk_sp<SkSurface> getSurface() override;
	void paint(HDC dc) override;
private:

};

