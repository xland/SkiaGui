#include "Raster.h"
#include "../WindowBase.h"

Raster::Raster(WindowBase* win):Backend(win)
{
    backendType = "Raster";
}

Raster::~Raster()
{
}

Raster* Raster::create(WindowBase* win)
{
    return nullptr;
}

void Raster::resize()
{
    surface.reset(nullptr);
    SkImageInfo info = SkImageInfo::MakeN32Premul(win->w, win->h);
    surface = SkSurfaces::Raster(info);
}

sk_sp<SkSurface> Raster::getSurface()
{
    if (nullptr == surface) {
        SkImageInfo info = SkImageInfo::MakeN32Premul(win->w, win->h);
        surface = SkSurfaces::Raster(info);
    }
    return surface;
}

void Raster::paint(HDC dc)
{
    SkPixmap pix;
    surface->peekPixels(&pix);
    BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), win->w, 0 - win->h, 1, 32, BI_RGB, win->h * 4 * win->w, 0, 0, 0, 0 };
    SetDIBitsToDevice(dc, 0, 0, win->w, win->h, 0, 0, 0, win->h, pix.addr(), &bmi, DIB_RGB_COLORS);
}
