#include "WindowBase.h"

WindowBase::WindowBase()
{
}

WindowBase::~WindowBase()
{
    YGConfigFree(config);
}

void WindowBase::initWindow()
{
    const TCHAR clsName[] = L"SkiaGui";
    static WNDCLASSEX wcx;
    static bool isClsReg = false;
    auto hinstance = GetModuleHandle(NULL);
    if (!isClsReg) {
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcx.lpfnWndProc = &WindowBase::routeWinMsg;
        wcx.cbWndExtra = sizeof(WindowBase*);
        wcx.hInstance = hinstance;
        wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcx.hCursor = LoadCursor(NULL, IDC_IBEAM);
        wcx.lpszClassName = clsName;
        RegisterClassEx(&wcx);
        isClsReg = true;
    }
    //必须是WS_POPUP，不能是WS_OVERLAPPEDWINDOW，不然渲染会出问题
    hwnd = CreateWindowEx(NULL, clsName, clsName, WS_POPUP, x, y, w, h, nullptr, nullptr, hinstance, nullptr);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    BOOL attrib = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &attrib, sizeof(attrib));

    DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));
    MARGINS margins = { 1,1,1,1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    backend = Backend::create(this);
    config = YGConfigNew();
    YGConfigSetPointScaleFactor(config, 1.5f); //todo
    YGNodeSetConfig(node, config);
}
void WindowBase::show()
{
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    emit("onShown");
}

void WindowBase::layout()
{
    YGNodeCalculateLayout(node, w, h, YGDirectionLTR);
}

bool WindowBase::alphaWindow()
{
    if (!IsWindowsVistaOrGreater()) { return false; }
    BOOL isCompositionEnable = false;
    //检查DWM是否启用
    DwmIsCompositionEnabled(&isCompositionEnable);
    if (!isCompositionEnable) { return true; }
    DWORD currentColor = 0;
    BOOL isOpaque = false;
    //检查是否支持毛玻璃效果
    DwmGetColorizationColor(&currentColor, &isOpaque);
    if (!isOpaque || IsWindows8OrGreater())
    {
        HRGN region = CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        DwmEnableBlurBehindWindow(hwnd, &bb);
        DeleteObject(region);
        return true;
    }
    else // For Window7
    {
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE;
        DwmEnableBlurBehindWindow(hwnd, &bb);
        return false;
    }
}

LRESULT WindowBase::routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto obj = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!obj) {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    switch (msg)
    {
        case WM_NCCALCSIZE:
        {
            if (wParam == TRUE) {
                NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                pncsp->rgrc[0] = pncsp->rgrc[1]; //窗口客户区覆盖整个窗口
                return 0; //确认改变窗口客户区
            }
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        case WM_ERASEBKGND:
        {
            return TRUE;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY:
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
            UnregisterClass(L"SkiaInput", nullptr);
		    PostQuitMessage(0);
            return 0;
        }
        default:
        {
            return obj->processWinMsg(msg, wParam, lParam);
        }
    }
}
LRESULT WindowBase::processWinMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_NCHITTEST:
    {
        return hitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    }
    case WM_TIMER: {
        emit("onTimer",(size_t)wParam);
        return 0;
    }
    case WM_MOVE: {
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        emit("onMove", x,y);
        return 0;
    }
    case WM_SIZE: {
        //todo minimize
        w = LOWORD(lParam);
        h = HIWORD(lParam);
        backend->resize();
        layout();
        emit("onSize", w, h);
        return 0;
    }
    //case WM_SETCURSOR: {
    //    POINT pt;
    //    GetCursorPos(&pt);        
    //    ScreenToClient(hwnd, &pt);
    //    onSetCursor(pt.x, pt.y);
    //    return TRUE;
    //}
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        auto canvas = backend->getCanvas();
        for (auto& ele:children)
        {
            ele->paint(canvas);
        }
        emit("onPaint", canvas);
        backend->paint(hdc);
        ReleaseDC(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        emit("onMousePress", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (wParam & MK_LBUTTON) {
            emit("onMouseDrag", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        else {
            emit("onMouseMove", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
    }
    case WM_LBUTTONDBLCLK:
    {
        emit("onMouseDBClick", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_LBUTTONUP:
    {
        emit("onMouseRelease", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        emit("onMousePressRight", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_KEYDOWN:
    {
        if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
            emit("onKeyDownWithCtrl", (size_t)wParam);
        }
        else {
            emit("onKeyDown", (size_t)wParam);
        }
        break;
    }
    case WM_DPICHANGED:
    {
        //todo
        //YGNodeSetConfig(node, config);
        break;
    }
    default: {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
    return 0;
}

LRESULT WindowBase::hitTest(const int& x, const int& y) {
    RECT winRect;
    GetWindowRect(hwnd, &winRect);
    if (x > winRect.left && y > winRect.top && x < winRect.right && y < winRect.bottom) {
        int borderWidth = 5;
        if (x < winRect.left + borderWidth && y < winRect.top + borderWidth) return HTTOPLEFT;
        else if (x < winRect.left + borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
        else if (x > winRect.right - borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
        else if (x > winRect.right - borderWidth && y < winRect.top + borderWidth) return HTTOPRIGHT;
        else if (x < winRect.left + borderWidth) return HTLEFT;
        else if (x > winRect.right - borderWidth) return HTRIGHT;
        else if (y < winRect.top + borderWidth) return HTTOP;
        else if (y > winRect.bottom - borderWidth) return HTBOTTOM;
        //else if (IsMouseInCaptionArea(x - winRect.left, y - winRect.top))
        //{
        //    return HTCAPTION;
        //}
        //return HTCLIENT;
        return HTCAPTION;
    }
    else
    {
        return HTNOWHERE;
    }
}