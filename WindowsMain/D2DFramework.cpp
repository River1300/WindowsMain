//#include <sstream>
//#include "D2DFramework.h"
//
//#pragma comment(lib, "d2d1.lib")
//
//
//HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
//{
//    // #1-2-1. 윈도우 클래스와 윈도우 생성을 모두 옮겨 준다.
//    //      => 에러가 발생하는 WindowProc는 WindowProc함수를 옮겨주면 된다.
//    //      => 객체 없이 호출하기 위해 static 멤버 함수로 바꿔준다.
//    HWND hwnd;
//    WNDCLASSEX wc;
//    ZeroMemory(&wc, sizeof(WNDCLASSEX));
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpszClassName = WINDOWCLASSNAME;
//    wc.hInstance = hInstance;
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//    wc.lpfnWndProc = D2DFramework::WindowProc;
//    wc.cbSize = sizeof(WNDCLASSEX);
//    if (!RegisterClassEx(&wc))
//    {
//        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return E_FAIL;
//    }
//
//    RECT wr = { 0, 0,
//        static_cast<LONG>(width), static_cast<LONG>(height) };
//
//    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//    hwnd = CreateWindowEx(NULL,
//        WINDOWCLASSNAME,
//        title,
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        wr.right - wr.left,
//        wr.bottom - wr.top,
//        NULL,
//        NULL,
//        hInstance,
//        NULL);
//    if (hwnd == nullptr)
//    {
//        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return E_FAIL;
//    }
//
//    // #1-2-2. 윈도우를 멤버 함수안에 있는 객체( this )로 가리킬 수 있게 포인터를 받아온다.
//    //      => 인스턴스의 주소가 윈도우 핸들에 붙는다.
//    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
//
//    mHwnd = hwnd;
//
//    return S_OK;
//}
//
//HRESULT D2DFramework::InitD2D()
//{
//    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
//        mspD2DFactory.ReleaseAndGetAddressOf());
//
//    ThrowIfFailed(hr);
//
//    RECT wr;
//    GetClientRect(mHwnd, &wr);
//    hr = mspD2DFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(mHwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
//        &mspRenderTarget
//    );
//
//    ThrowIfFailed(hr);
//
//    return S_OK;
//}
//
//
//HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
//{
//    HRESULT hr;
//
//    hr = InitWindow(hInstance, title, width, height);
//    ThrowIfFailed(hr);
//
//    hr = InitD2D();
//    ThrowIfFailed(hr);
//
//    ShowWindow(mHwnd, SW_NORMAL);
//    UpdateWindow(mHwnd);
//
//    return hr;
//}
//
//void D2DFramework::Release()
//{
//    mspRenderTarget.Reset();
//    mspD2DFactory.Reset();
//}
//
//
//void D2DFramework::Render()
//{
//    mspRenderTarget->BeginDraw();
//    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
//
//    mspRenderTarget->EndDraw();
//}
//
//
//int D2DFramework::GameLoop()
//{
//    MSG msg;
//    while (true)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//
//            if (msg.message == WM_QUIT)
//            {
//                break;
//            }
//        }
//        else
//        {
//            Render();
//        }
//    }
//
//    return static_cast<int>(msg.wParam);
//}
//
//
//void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
//{
//    std::wostringstream oss;
//
//    oss << "ERROR : 0x" << std::hex << error << std::endl;
//    oss << msg;
//
//    OutputDebugString(oss.str().c_str());
//}
//
//LRESULT D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    // #2-1. 윈도우 초기화 함수에서 지정한 포인터를 가져온다.
//    //      => 불러온 포인터 형태는 LONG형태이기 때문에 형 변환을 해준다.
//    auto pFramework = reinterpret_cast<D2DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
//
//    switch (message)
//    {
//    case WM_CLOSE:
//        DestroyWindow(hwnd);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}

#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib, "d2d1.lib")


// #. WindowClass + Window 생성
//      => 생성된 윈도우를 가리키는 hwnd는 멤버 변수 mHwnd가 가리키게 한다.
HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
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

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    mHwnd = hwnd;

    return S_OK;
}


// #. Factory + RenderTarget 생성
//      => 헤더 파일에 만들어 두었던 ComPtr멤버 변수로 생성된 메모리 공간을 가리킨다.
HRESULT D2DFramework::InitD2D()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        mspD2DFactory.ReleaseAndGetAddressOf());

    ThrowIfFailed(hr);

    hr = CreateDeviceResources();

    return hr;
}


HRESULT D2DFramework::CreateDeviceResources()
{
    // #. 디바이스 종속 리소스를 만드는 기능
    RECT wr;
    GetClientRect(mHwnd, &wr);
    HRESULT hr = mspD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mHwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        &mspRenderTarget
    );

    ThrowIfFailed(hr);

    return hr;
}


// #. 윈도우와 다이렉트 생성을 담당하는 멤버 함수들을 호출하여 프레임 워크를 초기화 한다.
//      => 초기화를 완료 후 윈도우를 그려준다.
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


// #. Factory + RenderTarget 메모리 공간을 해제한다.
//      => 멤버 변수로 만들때 ComPtr로 만들었기 때문에 메모리 공간을 COM에서 직접 관리한다.
//      => 그래서 굳이 메모리 공간을 해제하지 않아도 COM이 알아서 해제해준다.
//      => 다만 이 함수는 명시적으로 해제하였음을 알려주는 용도이다.
void D2DFramework::Release()
{
    mspRenderTarget.Reset();
    mspD2DFactory.Reset();
}


// #. 그리기
//      => RenderTarget으로 지정한 그릴 영역에 그림을 그린다.
void D2DFramework::Render()
{
    mspRenderTarget->BeginDraw();
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    // #. 그리기 장치가 손상되었는지 확인한다.
    //      => 그림 파일을 불러서 비디오 메모리에 올린다.
    //      => 문제가 발생하여 장치 손실이 발생하면 그림 파일을 다시 불러주어야 한다.
    //      => 즉 전부 다시 만들어야 한다.
    HRESULT hr = mspRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResources();
    }
}


// #. 메세지 이벤트
//      => 앱이 종료될때 까지 무한 반복한다.
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
            // #. 메세지는 이벤트가 발생할 때만 실행되지만 Render는 계속해서 실행된다.
            Render();
        }
    }

    return static_cast<int>(msg.wParam);
}


// #. 디버그 메세지 출력
//      => 디버그 메세지 창에 에러 메세지를 출력한다.
void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
    std::wostringstream oss;

    oss << "ERROR : 0x" << std::hex << error << std::endl;
    oss << msg;

    OutputDebugString(oss.str().c_str());
}


// #. 윈도우 프로시져 실행
//      => 윈도우 프로시져의 다른 메시지를 처리할 때 백업해둔 인스턴스 포인터를 활용할 수 있다.
LRESULT D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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