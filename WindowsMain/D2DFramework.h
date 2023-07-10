#pragma once
#include <d2d1.h>
#include <wrl/client.h>
// #1. 프레임워크에 필요한 헤더파일을 옮긴다.

class D2DFramework
{
// 상속해서 사용할 수 있도록 protected
protected:
	// #2. 프레임워크에 팩토리와 렌더타겟을 옮긴다.
	//		=> ComPtr을 사용하여 스마트 포인터로 만들어 준다.
	Microsoft::WRL::ComPtr <ID2D1Factory> gpD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> gpRenderTarget{};

public:
	// #3. 초기화, 해제, 그리기와 관련된 행동을 함수로 만들어 준다.
	//		=> 모든 COM은 HRESULT를 반환하기 때문에 초기화 함수 역시 HRESULT를 반환하도록 한다.
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	// #4. Error메세지를 출력할 함수를 만든다.
	//		=> 메세지 박스에서 출력될 메세지와 에러 코드, 그리고 에러 제목을 매개 변수로 전달한다.
	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
};