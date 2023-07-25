#pragma once

#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>
#include "ComException.h"

// #. �׸��⿡ �ʿ��� �۾�
//		Input( ���� �̸��� �޾Ƽ� ) Output( ID2D1Bitmap �������̽� )
//			=> ó�� �ε��̶�� ID2D1Bitmap �������̽��� �����ϰ� �ƴ϶�� ������ �������̽��� ��ȯ

// 3. �����ӿ�ũ���� ����ó�� ����� �и��Ѵ�.

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