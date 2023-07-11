#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib, "d2d1.lib")


HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    // #1-2-1. 윈도우 클래스와 윈도우 생성을 모두 옮겨 준다.
    //      => 에러가 발생하는 WindowProc는 WindowProc함수를 옮겨주면 된다.
    //      => 객체 없이 호출하기 위해 static 멤버 함수로 바꿔준다.
    HWND hwnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = WINDOWCLASSNAME;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = D2DFramework::WindowProc;
    wc.cbSize = sizeof(WNDCLASSEX);
    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return E_FAIL;
    }

    RECT wr = { 0, 0,
        static_cast<LONG>(width), static_cast<LONG>(height) };

    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd = CreateWindowEx(NULL,
        WINDOWCLASSNAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (hwnd == nullptr)
    {
        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return E_FAIL;
    }

    // #1-2-2. 윈도우를 멤버 함수안에 있는 객체( this )로 가리킬 수 있게 포인터를 받아온다.
    //      => 인스턴스의 주소가 윈도우 핸들에 붙는다.
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    mHwnd = hwnd;

    return S_OK;
}

HRESULT D2DFramework::InitD2D()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        mspD2DFactory.ReleaseAndGetAddressOf());

    ThrowIfFailed(hr);

    RECT wr;
    GetClientRect(mHwnd, &wr);
    hr = mspD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mHwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        &mspRenderTarget
    );

    ThrowIfFailed(hr);

    return S_OK;
}


HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr;

    hr = InitWindow(hInstance, title, width, height);
    ThrowIfFailed(hr);

    hr = InitD2D();
    ThrowIfFailed(hr);

    ShowWindow(mHwnd, SW_NORMAL);
    UpdateWindow(mHwnd);

    return hr;
}

void D2DFramework::Release()
{
    mspRenderTarget.Reset();
    mspD2DFactory.Reset();
}


void D2DFramework::Render()
{
    mspRenderTarget->BeginDraw();
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    mspRenderTarget->EndDraw();
}


int D2DFramework::GameLoop()
{
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Render();
        }
    }

    return static_cast<int>(msg.wParam);
}


void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
    std::wostringstream oss;

    oss << "ERROR : 0x" << std::hex << error << std::endl;
    oss << msg;

    OutputDebugString(oss.str().c_str());
}

LRESULT D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // #2-1. 윈도우 초기화 함수에서 지정한 포인터를 가져온다.
    //      => 불러온 포인터 형태는 LONG형태이기 때문에 형 변환을 해준다.
    auto pFramework = reinterpret_cast<D2DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
