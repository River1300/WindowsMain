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
//	// #1. 윈도우와 관련된 모든 기능을 프레임워크에 숨길 예정
//
//	// #1-1. 전역 변수로 있던 윈도우 이름을 옮겨준다.
//	const wchar_t* WINDOWCLASSNAME{ L"MyWindowClass" };
//	// #. 배열의 크기는 컴파일타임에 지정되어야만 한다.
//	// #. 클래스의 멤버가 만들어지는 때는 클래스의 객체가 만들어질 때 이다.
//	// #. 그리고 객체가 만들어지는 시점은 런타임이다.
//	// #. 멤버가 정적 배열일 경우 컴파일타임에 배열의 크기가 정해저야만 하기 때문에 동적으로 만들어야만 한다.
//
//protected:
//	HWND mHwnd;
//
//	Microsoft::WRL::ComPtr <ID2D1Factory> mspD2DFactory{};
//	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> mspRenderTarget{};
//
//protected:
//	// #1-2. 윈도우를 초기화할 함수를 만든다.
//	//		=> 단, 내부에서만 사용하게 protected로 묶고 public에 있는 Init멤버 함수를 통해 호출하게 만든다.
//	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
//	HRESULT InitD2D();
//
//public:
//	// #3. 윈도우와 다이렉트를 초기화하는 함수를 부를 함수를 만든다.
//	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
//		UINT width = 1024, UINT height = 768);
//
//	virtual void Release();
//	virtual void Render();
//
//	// #4. MSG루프를 실행할 함수를 만드낟.
//	virtual int GameLoop();
//
//	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
//
//	// #2. 윈도우 프로시저를 옮겨 준다.
//	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//	// #. static 클래스 함수 안에는 인스턴스( this )가 없다.
//	// #. 때문에 인스턴스를 포인터로 만들어 주어야 한다.
//};

#pragma once
#include <d2d1.h>
#include <wrl/client.h>
#include <stdio.h>
#include <exception>

// #. 예외 상황을 잡아줄 클래스
class com_exception : public std::exception
{
private:
	HRESULT result;

public:
	com_exception(HRESULT hr) : result(hr) {}

	virtual const char* what() const override
	{
		static char str[64]{};
		sprintf_s(str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));

		return str;
	}
};

// #. HRESULT로 반환받은 값에 문제가 있다면 클래스 생성자를 호출하여 예외를 던진다.
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
private:
	const wchar_t* WINDOWCLASSNAME{ L"MyWindowClass" };

protected:
	HWND mHwnd;

	// #. 생성된 Factory를 mspD2DFactory로 가리킨다.
	// #. 생성된 HwndRenderTarget을 mspRenderTarget으로 가리킨다.
	Microsoft::WRL::ComPtr <ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	// #. Window 기능 : 윈도우 만들기
	// #. Direct2D 기능 : 생성( Factory, RenderTarget )
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();

	virtual HRESULT CreateDeviceResources();

public:
	// #. Window 기능 : 메시지 루프
	// #. Direct2D 기능 : 그리기( Rendering )
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();
	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");

	// #. Window 기능 : 메시지 프로시져
	//		=> 윈도우 API는 C언어로 만들어져 있고 클래스의 멤버 함수는 C++의 언어로 만들 수 있다.
	//		=> 때문에( 인스턴스 때문에 ) 함수 포인터가 서로 호환되지 않는다.
	//		=> 이 문제를 해결하기 위해 클래스 함수( static member function )로 WindowProc을 옮겨 왔다.
	//				=> 이 때 또 다른 문제가 생겼는데, 클래스 함수는 멤버 함수에 접근할 수가 없다.( 인스턴스 때문에 )
	//				=> SetWindowLongPtr을 통해서 인스턴스의 포인터를 백업하고
	//				=> 사용할 때 GetWindowLongPtr로 인스턴스의 포인터를 복원하였다.
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};