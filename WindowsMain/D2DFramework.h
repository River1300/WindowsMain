#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

// #. COM 오브젝트에서 발생하는 에러는 나만의 예외로 만들도록 한다.
//		=> 사용자 정의 예외를 만들기 위해 std::exception 클래스를 상속 받아 클래스를 만든다.
class com_exception : public std::exception	// COM 예외 Helper Class
{	// com_exception 클래스는 멤버 변수로 HRESULT를 가지고 있으며 생성자 com_exception(HRESULT)와 오버라이드한 what() 멤버 함수로 되어 있는 클래스다.
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
{	// #. 상속 받아 사용하기 위해 멤버들은 protected, 함수는 virtual로 선언하였고, 스마트 포인터를 쓴다.
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title);
};