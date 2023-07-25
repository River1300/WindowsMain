#pragma once

#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>
#include "ComException.h"

// #. 그리기에 필요한 작업
//		Input( 파일 이름을 받아서 ) Output( ID2D1Bitmap 인터페이스 )
//			=> 처음 로딩이라면 ID2D1Bitmap 인터페이스를 생성하고 아니라면 기존의 인터페이스를 반환

// 3. 프레임워크에서 예외처리 기능을 분리한다.

class D2DFramework
{
	const LPCWSTR WindowClassName{ L"D2DWindowClass" };

protected:
	HWND mHwnd;

	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();
	virtual HRESULT CreateDeviceResources();

public:
	virtual HRESULT Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768
	);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return mspRenderTarget.Get(); }
	IWICImagingFactory* GetWICFactory() { return mspWICFactory.Get(); }
};