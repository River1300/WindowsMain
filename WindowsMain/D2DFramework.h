#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

class com_exception : public std::exception
{
	HRESULT result;

public:
	com_exception(HRESULT hr) : result(hr) {}

	virtual const char* what() const override
	{
		static char str[64] = {};
		sprintf_s(
			str, "Failure with HRESULT of %08X",
			static_cast<unsigned int>(result)
		);
		return str;
	}
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr)) throw com_exception(hr);
}

class D2DFramework
{
	const LPCWSTR WindowClassName{ L"D2DWindowClass" };

protected:
	HWND mHwnd;

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
};