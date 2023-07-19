#pragma once
//#include <d2d1.h>
//#include <wrl/client.h>
//#include <stdio.h>
//#include <exception>
//
//class com_exception : public std::exception
//{
//private:
//	HRESULT result;
//
//public:
//	com_exception(HRESULT hr) : result(hr) {}
//
//	virtual const char* what() const override
//	{
//		static char str[64]{};
//		sprintf_s(str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
//
//		return str;
//	}
//};
//
//inline void ThrowIfFailed(HRESULT hr)
//{
//	if (FAILED(hr))
//	{
//		throw com_exception(hr);
//	}
//}
//
//class D2DFramework
//{
//private:
//	// #1. ������� ���õ� ��� ����� �����ӿ�ũ�� ���� ����
//
//	// #1-1. ���� ������ �ִ� ������ �̸��� �Ű��ش�.
//	const wchar_t* WINDOWCLASSNAME{ L"MyWindowClass" };
//	// #. �迭�� ũ��� ������Ÿ�ӿ� �����Ǿ�߸� �Ѵ�.
//	// #. Ŭ������ ����� ��������� ���� Ŭ������ ��ü�� ������� �� �̴�.
//	// #. �׸��� ��ü�� ��������� ������ ��Ÿ���̴�.
//	// #. ����� ���� �迭�� ��� ������Ÿ�ӿ� �迭�� ũ�Ⱑ �������߸� �ϱ� ������ �������� �����߸� �Ѵ�.
//
//protected:
//	HWND mHwnd;
//
//	Microsoft::WRL::ComPtr <ID2D1Factory> mspD2DFactory{};
//	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> mspRenderTarget{};
//
//protected:
//	// #1-2. �����츦 �ʱ�ȭ�� �Լ��� �����.
//	//		=> ��, ���ο����� ����ϰ� protected�� ���� public�� �ִ� Init��� �Լ��� ���� ȣ���ϰ� �����.
//	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
//	HRESULT InitD2D();
//
//public:
//	// #3. ������� ���̷�Ʈ�� �ʱ�ȭ�ϴ� �Լ��� �θ� �Լ��� �����.
//	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
//		UINT width = 1024, UINT height = 768);
//
//	virtual void Release();
//	virtual void Render();
//
//	// #4. MSG������ ������ �Լ��� ���峮.
//	virtual int GameLoop();
//
//	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
//
//	// #2. ������ ���ν����� �Ű� �ش�.
//	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//	// #. static Ŭ���� �Լ� �ȿ��� �ν��Ͻ�( this )�� ����.
//	// #. ������ �ν��Ͻ��� �����ͷ� ����� �־�� �Ѵ�.
//};

#include <d2d1.h>
// #. �׸� ������ �θ��� ���� WIC�� ����ϴµ�, �� WIC�� �����ӿ�ũ�� ���� �����̴�.
//		=> ������ �׸��� ��ü�� �����ӿ�ũ�� �и� ��ų ����
//		=> �������� �׸���� ID2D1Bitmap* pBitmap���� ����
//		=> WIC�� ���� �������
#include <wincodec.h>
#include <wrl/client.h>
#include "ComException.h"

class D2DFramework
{
private:
	const wchar_t* WINDOWCLASSNAME{ L"MyWindowClass" };

protected:
	HWND mHwnd;

	// #. WIC ���� ����
	//Microsoft::WRL::ComPtr <IWICImagingFactory> mspWICFactory{};
	Microsoft::WRL::ComPtr <ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();

	virtual HRESULT CreateDeviceResources();

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();
	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	// #. �׸��� ��ü�� �ʿ��� ���� �� �ϳ��� ����Ÿ��
	//		=> ����Ÿ���� �θ��� ���� ���͸� ����� �д�.
	ID2D1HwndRenderTarget* GetRenderTarget() { return mspRenderTarget.Get(); }
	//IWICImagingFactory* GetWICFactory() { return mspWICFactory.Get(); }
	HWND GetWindowHandle() { return mHwnd; }
};