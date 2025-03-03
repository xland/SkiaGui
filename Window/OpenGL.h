#pragma once
#include <include/gpu/ganesh/gl/GrGLInterface.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include "Backend.h"

class WindowBase;
class OpenGL:public Backend
{
public:
	OpenGL(WindowBase* win);
	~OpenGL();
	void resize() override;
	sk_sp<SkSurface> getSurface() override;
	void paint(HDC dc) override;
	void textureFromImage(sk_sp<SkImage>& image) override;
private:
	void init();
private:
	HGLRC hglrc;
	sk_sp<GrDirectContext> grContext;
	sk_sp<const GrGLInterface> backendContext;
	SkSurfaceProps surfaceProps;
	GrGLFramebufferInfo fbInfo;
};

