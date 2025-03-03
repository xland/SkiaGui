#include <include/gpu/ganesh/gl/GrGLInterface.h>
#include <include/gpu/ganesh/gl/GrGLDirectContext.h>
#include <include/gpu/ganesh/gl/GrGLAssembleHelpers.h>
#include <include/gpu/ganesh/gl/GrGLAssembleInterface.h>

#include <include/gpu/ganesh/SkImageGanesh.h>
#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/gpu/ganesh/gl/GrGLBackendSurface.h>

#include <src/base/SkMathPriv.h>
#include <src/gpu/ganesh/GrCaps.h>
#include <src/gpu/ganesh/GrDirectContextPriv.h>
#include <src/gpu/ganesh/gl/GrGLDefines.h>
#include <src/gpu/ganesh/gl/GrGLUtil.h>

#include <Windows.h>
#include <GL/gl.h>
#include "../WindowBase.h"
#include "OpenGL.h"


namespace {
    #define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
    #define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126
    #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001 
    typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    extern "C" {
        __declspec(dllexport) int NvOptimusEnablement = 1;
        __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    }
}

OpenGL::OpenGL(WindowBase* win) : Backend(win), surfaceProps(SkSurfaceProps::kUseDeviceIndependentFonts_Flag, kRGB_H_SkPixelGeometry)
{
    backendType = "OpenGL";
    init();
}

OpenGL::~OpenGL()
{
    surface.reset(nullptr);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hglrc);
    grContext.reset(nullptr);
    backendContext.reset(nullptr);
    hglrc = NULL;
}

void OpenGL::resize()
{
    surface.reset(nullptr);
    if (grContext) {
        grContext->flushAndSubmit();
    }
}
sk_sp<SkSurface> OpenGL::getSurface()
{
    if (nullptr == surface) {
        auto backendRT = GrBackendRenderTargets::MakeGL(win->w, win->h,
            16, //sampleCount
            8, //stencilBits
            fbInfo);
        surface = SkSurfaces::WrapBackendRenderTarget(grContext.get(),
            backendRT,
            kBottomLeft_GrSurfaceOrigin,
            kRGBA_8888_SkColorType,
            nullptr,
            &surfaceProps);
    }
    return surface;
}
void OpenGL::init()
{
    HDC hdc = GetDC(win->hwnd);
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    auto flag = SetPixelFormat(hdc, pixelFormat, &pfd);
    HGLRC oldContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, oldContext);
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,0
    };
    HGLRC hglrc;
    HGLRC newContext = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (newContext) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(oldContext);
        hglrc = newContext;
    }
    else {
        hglrc = oldContext;
    }
    wglMakeCurrent(hdc, hglrc);
    ReleaseDC(win->hwnd, hdc);
    backendContext = GrGLMakeNativeInterface();
    GrGLint buffer;
    backendContext.get()->fFunctions.fGetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer);
    fbInfo.fFBOID = buffer;
    fbInfo.fFormat = GR_GL_RGBA8;
    fbInfo.fProtected = skgpu::Protected(false);
    grContext = GrDirectContexts::MakeGL(backendContext);
}
void OpenGL::paint(HDC dc) {
    grContext->flushAndSubmit(surface.get());
    SwapBuffers(dc);
}

void OpenGL::textureFromImage(sk_sp<SkImage>& image)
{
    image = SkImages::TextureFromImage(grContext.get(), image.get());
}