#include "WindowMain.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <stdint.h>
typedef uint32_t Color;
static constexpr inline Color GetColor(unsigned r, unsigned g, unsigned b, unsigned a = 255) {
    return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

LRESULT CALLBACK WindowMain::RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        LPVOID pThis = pCS->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    auto obj = reinterpret_cast<WindowMain*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (obj) {
        if (msg == WM_CLOSE) {
            delete (WindowMain*)obj;
        }
        else
        {
            return obj->WindowProc(hWnd, msg, wParam, lParam);
        }

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


WindowMain::WindowMain() {
    static int num = 0;
    std::wstring className = std::format(L"SkiaGui{}", num++);
    auto hinstance = GetModuleHandle(NULL);
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = &WindowMain::RouteWindowMessage;
    wcx.cbWndExtra = sizeof(WindowMain*);
    wcx.hInstance = hinstance;
    wcx.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(hinstance, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcx.lpszClassName = className.c_str();
    if (!RegisterClassEx(&wcx))
    {
        MessageBox(NULL, L"ע�ᴰ����ʧ��", L"ϵͳ��ʾ", NULL);
        return;
    }
    auto exStyle = WS_EX_LAYERED;   
    this->hwnd = CreateWindowEx(exStyle, wcx.lpszClassName, wcx.lpszClassName, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, 
        x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
    BOOL attrib = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &attrib, sizeof(attrib));//�Ƴ����ڴ���ر�ʱ�Ķ���Ч��
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    PostMessage(hwnd, WM_REFRESH, NULL, NULL);

}

WindowMain::~WindowMain()
{
    delete[] pixelData;
    DeleteDC(hCompatibleDC);
    DeleteObject(bottomHbitmap);
}

LRESULT CALLBACK WindowMain::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_NCCALCSIZE:
    {
        if (wParam == TRUE)
        {
            return false;
        }
        break;
    }
    case WM_NCHITTEST:
    {
        POINT p;
        ::GetCursorPos(&p);
        ::ScreenToClient(hWnd, &p);
        if (p.x > 0 && p.x<200 && p.y>0 && p.y < 200) {
            return HTCAPTION;
        }
        else {
            return HTCLIENT;
        }
    }
    case WM_SETCURSOR: {
        return true;
    }
    case WM_REFRESH: {
        HDC hdc;
        hdc = GetDC(hWnd);
        if (hCompatibleDC == NULL)
        {
            hCompatibleDC = CreateCompatibleDC(NULL);
            bottomHbitmap = CreateCompatibleBitmap(hdc, w, h); //����һ���뵱ǰDC���ݵ�λͼ
            pixelData = new unsigned char[w*h*4];
            DeleteObject(SelectObject(hCompatibleDC, bottomHbitmap));
            SkImageInfo info = SkImageInfo::MakeN32Premul(w, h);
            surface = SkSurface::MakeRasterDirect(info, pixelData, w*4);    
            auto canvas = surface->getCanvas();
            canvas->clear(GetColor(248, 248, 248));
        }
        BITMAPINFO info = { sizeof(BITMAPINFOHEADER), w, 0 - h, 1, 32, BI_RGB, w * 4 * h, 0, 0, 0, 0 };
        SetDIBits(hdc, bottomHbitmap, 0, h, pixelData, &info, DIB_RGB_COLORS); //ʹ��ָ����DIB��ɫ����������λͼ�е�����
        BLENDFUNCTION blend = { .BlendOp{AC_SRC_OVER},.SourceConstantAlpha{255},.AlphaFormat{AC_SRC_ALPHA} };//��ͨ�����
        POINT pSrc = { 0, 0 };
        SIZE sizeWnd = { w, h };
        UpdateLayeredWindow(hWnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);//���·ֲ㴰��
        ReleaseDC(hWnd, hdc);
        return 1;
    }
    case WM_MOUSEMOVE: {
        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);
        SkRect rect = SkRect::MakeXYWH(x, y, 20, 20);
        SkPaint paint;
        paint.setColor(SK_ColorBLUE);
        auto canvas = surface->getCanvas();
        canvas->drawRect(rect, paint);
        PostMessage(hWnd, WM_REFRESH, NULL, NULL);
        return true;
    }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}