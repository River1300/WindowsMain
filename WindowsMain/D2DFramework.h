#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

// #. COM ������Ʈ���� �߻��ϴ� ������ ������ ���ܷ� ���鵵�� �Ѵ�.
//		=> ����� ���� ���ܸ� ����� ���� std::exception Ŭ������ ��� �޾� Ŭ������ �����.
class com_exception : public std::exception	// COM ���� Helper Class
{	// com_exception Ŭ������ ��� ������ HRESULT�� ������ ������ ������ com_exception(HRESULT)�� �������̵��� what() ��� �Լ��� �Ǿ� �ִ� Ŭ������.
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
{	// #. ��� �޾� ����ϱ� ���� ������� protected, �Լ��� virtual�� �����Ͽ���, ����Ʈ �����͸� ����.
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title);
};