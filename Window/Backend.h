#pragma once
#include <string>
#include <memory>
#include <Windows.h>
#include <include/core/SkSurface.h>
#include <include/core/SkColorSpace.h>

class WindowBase;
class Backend
{
public:
	~Backend();
	static std::unique_ptr<Backend> create(WindowBase* win);
	virtual void resize() = 0;
	virtual sk_sp<SkSurface> getSurface() = 0;
	SkCanvas* getCanvas();
	virtual void textureFromImage(sk_sp<SkImage>& image) {};
	virtual void paint(HDC dc) = 0;
public:
	std::string backendType;
protected:
	Backend(WindowBase* win);
protected:
	WindowBase* win;
	sk_sp<SkSurface> surface;
private:
	static bool isGPUAvailable();
};

