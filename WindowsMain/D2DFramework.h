#pragma once
#include <d2d1.h>
#include <wrl/client.h>
#include <stdio.h>
#include <exception>		// #1. 예외 처리를 위해 헤더 파일을 추가한다.
//		=> C++에서 사용하는 예외의 기반 클래스가 포함되어 있다.

// #2. 예외 기반 클래스를 상속 받은 클래스를 만들어 준다.
class com_exception : public std::exception
{
private:
	HRESULT result;

public:
	// #3. 에외에 대한 생성자를 만들어 준다.
	//		=> 생성자를 통해서 에러 코드를 받으면 해당 코드를 멤버( resutl )에 저장할 수 있다.
	com_exception(HRESULT hr) : result(hr) {}

	// #4. 에러를 출력하는 함수를 만든다.
	//		=> 에러 코드( result )를 16진수로 변환하여 str에 대입한다.
	//		=> 에러에 대한 자세한 사항을 str을 반환하여 출력해 준다.
	virtual const char* what() const override
	{
		static char str[64]{};
		sprintf_s(str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));

		return str;
	}
};

// #5. 실패하였을 때 예외를 던지는 함수를 만든다.
//		=> HRESULT를 매개 변수로 받아서 만약에 hr이 실패 했을 경우
//		=> com_exception 클래스를 hr을 생성자로 넘겨준다. 즉 에러 사항을 com_exception으로 던진다.
inline void ThrowIfFailed(HRESULT hr)
{
	// #. < inline > : 함수가 STACK메모리 공간에 쌓이고 빠져나가는 과정을 생략한다.
	//		=> 간단한 기능일 경우 inline을 활용하면 속도가 더 빨라진다.
	//		=> 간단하지 않는 경우 컴파일러가 거부할 수도 있다.
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
protected:
	Microsoft::WRL::ComPtr <ID2D1Factory> gpD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> gpRenderTarget{};

public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
};