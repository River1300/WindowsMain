//#include <sstream>
//#include "D2DFramework.h"
//
//#pragma comment(lib, "d2d1.lib")
//
//
//HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
//{
//    // #1-2-1. ������ Ŭ������ ������ ������ ��� �Ű� �ش�.
//    //      => ������ �߻��ϴ� WindowProc�� WindowProc�Լ��� �Ű��ָ� �ȴ�.
//    //      => ��ü ���� ȣ���ϱ� ���� static ��� �Լ��� �ٲ��ش�.
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
//    // #1-2-2. �����츦 ��� �Լ��ȿ� �ִ� ��ü( this )�� ����ų �� �ְ� �����͸� �޾ƿ´�.
//    //      => �ν��Ͻ��� �ּҰ� ������ �ڵ鿡 �ٴ´�.
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
//    // #2-1. ������ �ʱ�ȭ �Լ����� ������ �����͸� �����´�.
//    //      => �ҷ��� ������ ���´� LONG�����̱� ������ �� ��ȯ�� ���ش�.
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


// #. WindowClass + Window ����
//      => ������ �����츦 ����Ű�� hwnd�� ��� ���� mHwnd�� ����Ű�� �Ѵ�.
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


// #. Factory + RenderTarget ����
//      => ��� ���Ͽ� ����� �ξ��� ComPtr��� ������ ������ �޸� ������ ����Ų��.
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
    // #. ����̽� ���� ���ҽ��� ����� ���
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


// #. ������� ���̷�Ʈ ������ ����ϴ� ��� �Լ����� ȣ���Ͽ� ������ ��ũ�� �ʱ�ȭ �Ѵ�.
//      => �ʱ�ȭ�� �Ϸ� �� �����츦 �׷��ش�.
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


// #. Factory + RenderTarget �޸� ������ �����Ѵ�.
//      => ��� ������ ���鶧 ComPtr�� ������� ������ �޸� ������ COM���� ���� �����Ѵ�.
//      => �׷��� ���� �޸� ������ �������� �ʾƵ� COM�� �˾Ƽ� �������ش�.
//      => �ٸ� �� �Լ��� ��������� �����Ͽ����� �˷��ִ� �뵵�̴�.
void D2DFramework::Release()
{
    mspRenderTarget.Reset();
    mspD2DFactory.Reset();
}


// #. �׸���
//      => RenderTarget���� ������ �׸� ������ �׸��� �׸���.
void D2DFramework::Render()
{
    mspRenderTarget->BeginDraw();
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    // #. �׸��� ��ġ�� �ջ�Ǿ����� Ȯ���Ѵ�.
    //      => �׸� ������ �ҷ��� ���� �޸𸮿� �ø���.
    //      => ������ �߻��Ͽ� ��ġ �ս��� �߻��ϸ� �׸� ������ �ٽ� �ҷ��־�� �Ѵ�.
    //      => �� ���� �ٽ� ������ �Ѵ�.
    HRESULT hr = mspRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResources();
    }
}


// #. �޼��� �̺�Ʈ
//      => ���� ����ɶ� ���� ���� �ݺ��Ѵ�.
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
            // #. �޼����� �̺�Ʈ�� �߻��� ���� ��������� Render�� ����ؼ� ����ȴ�.
            Render();
        }
    }

    return static_cast<int>(msg.wParam);
}


// #. ����� �޼��� ���
//      => ����� �޼��� â�� ���� �޼����� ����Ѵ�.
void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
    std::wostringstream oss;

    oss << "ERROR : 0x" << std::hex << error << std::endl;
    oss << msg;

    OutputDebugString(oss.str().c_str());
}


// #. ������ ���ν��� ����
//      => ������ ���ν����� �ٸ� �޽����� ó���� �� ����ص� �ν��Ͻ� �����͸� Ȱ���� �� �ִ�.
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