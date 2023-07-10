#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib, "d2d1.lib")


HRESULT D2DFramework::Init(HWND hwnd)       // #. Factory�� RenderTarget�� ����� �ش�.
{
    // #2-1. Factory�� �����.
    //      => ��� ���Ͽ� ������ �ξ��� ����Ʈ ������ gpD2DFactory�� ���� Factory �޸𸮸� ��´�.
    //      => ��� COM�� HRESULT�� ��ȯ�ϱ� ������ HRESULT hr�� ��ȯ���� �޾� �ش�.
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        gpD2DFactory.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        ShowErrorMsg(L"Failed To Create D2DFactory", hr);
        return hr;
    }

    // #2-2. �簢�� ������ ����� �� ������ RenderTarget�� �����Ѵ�.
    //      => RenderTarget�� Factory�� ���ؼ� ����� �ش�.
    //      => ������ �׸��� ������ gpRenderTarget�� ��´�.
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


void D2DFramework::Release()        // #. �Ҵ�� �޸� ������ �����Ѵ�.
{                                   //      => ComPtr�� �̿��Ͽ� �޸� ������ �����ϱ� ������ ���� �� �ʿ�� ���� �۾��̴�.
    gpRenderTarget.Reset();
    gpD2DFactory.Reset();
}


void D2DFramework::Render()     // #. �׸��� �׸��� �Լ��� �����.
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
