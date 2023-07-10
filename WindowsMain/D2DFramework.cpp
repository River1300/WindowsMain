#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib, "d2d1.lib")


HRESULT D2DFramework::Init(HWND hwnd)       // #. Factory와 RenderTarget을 만들어 준다.
{
    // #2-1. Factory를 만든다.
    //      => 헤더 파일에 선언해 두었던 스마트 포인터 gpD2DFactory에 만든 Factory 메모리를 담는다.
    //      => 모든 COM은 HRESULT를 반환하기 때문에 HRESULT hr로 반환값을 받아 준다.
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        gpD2DFactory.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        ShowErrorMsg(L"Failed To Create D2DFactory", hr);
        return hr;
    }

    // #2-2. 사각형 영역을 만들고 이 영역에 RenderTarget을 지정한다.
    //      => RenderTarget은 Factory를 통해서 만들어 준다.
    //      => 지정된 그리기 영역은 gpRenderTarget에 담는다.
    RECT wr;
    GetClientRect(hwnd, &wr);
    hr = gpD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        &gpRenderTarget
    );
    if (FAILED(hr))
    {
        ShowErrorMsg(L"Failed To Create HWNDRenderTarget", hr);
        return hr;
    }

    return S_OK;
}


void D2DFramework::Release()        // #. 할당된 메모리 공간을 해제한다.
{                                   //      => ComPtr을 이용하여 메모리 공간을 관리하기 때문에 굳이 할 필요는 없는 작업이다.
    gpRenderTarget.Reset();
    gpD2DFactory.Reset();
}


void D2DFramework::Render()     // #. 그림을 그리는 함수를 만든다.
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
