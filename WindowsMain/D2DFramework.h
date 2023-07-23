#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

// #. 윈도우 헬퍼
//		1. 윈도우 생성 : Init()
//		2. 메시지 루프 : GameLoop()
//		3. 위도우 프로시저( 메시지 처리 ) : 멤버함수 추가
//	=> 윈도우 생성에 필요한 인자
//		1. HINSTANCE : WinMain 함수에서 인스턴스를 받아서 윈도우 생성
//		2. LPCWSTR : 윈도우 이름으로 캡션에 표기되는 문자열
//		3. 윈도우 크기 : 윈도우의 크기를 지정할 때 사용

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

public:
	// #. 클래스 밖에서 불러줄 초기화 함수
	virtual HRESULT Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768
	);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title);
	// #. 일반 함수는 Function(); 으로 직접 호출이 가능하지만 멤버 함수의 호출은 Instance->Function();과 같이
	//		=> 인스턴스를 통해 호출하는 방식이다. 내부적으로는 함수 호출시에 인스턴스 정보도 넘어가는 것이다.
	//		=> 그런데 lpfnWndProc의 프로시저는 일반 함수이므로 타입이 달라서 적용할 수 없다.
	//		=> 클래스 멤버 중에서도 인스턴스와 상관없이 호출할 수 있는 것, 정적 함수
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};