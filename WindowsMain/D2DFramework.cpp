#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib, "d2d1.lib")


HRESULT D2DFramework::Init(HWND hwnd)
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        gpD2DFactory.ReleaseAndGetAddressOf());

    // #6. Factory가 정상적으로 만들어젔는지 확인한다.
    //      => 예외 처리는 다른 곳에서 해준다.
    ThrowIfFailed(hr);

    RECT wr;
    GetClientRect(hwnd, &wr);
    hr = gpD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        &gpRenderTarget
    );

    // #7. RenderTarget이 정상적으로 만들어젔는지 확인한다.
    //      => 예외 처리는 다른 곳에서 해준다.
    ThrowIfFailed(hr);

    return S_OK;
}


void D2DFramework::Release()
{
    gpRenderTarget.Reset();
    gpD2DFactory.Reset();
}


void D2DFramework::Render()
{
    gpRenderTarget->BeginDraw();
    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    gpRenderTarget->EndDraw();
}


void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
    std::wostringstream oss;

    oss << "ERROR : 0x" << std::hex << error << std::endl;
    oss << msg;

    OutputDebugString(oss.str().c_str());
}
