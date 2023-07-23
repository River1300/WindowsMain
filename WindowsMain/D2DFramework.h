#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

// #. Direct2D 리소스
//		=> 하드웨어의 성능을 활용한 그리기 API는 CPU 중심의 리소스들과 CPU에서도 잘 실행되는 그리기 명령들을 가지고 있으며
//		=> 그 중 일부만 하드웨어 가속을 지원한다. GPU는 제작사별로 성능이나 스펙이 다르기 때문에 하드웨어 가속 리소스를 사용하지
//		=> 못하는 경우도 존재하므로 가능할 때는 GPU 리소스를 사용하고 그렇지 못하면 CPU의 리소스를 사용할 수 있어야 한다.
// #. Direct2D는 이러한 개념을 위해 리소스를 크게 2가지로 구분한다.
//		=> 장치 독립적인 리소스 : ID2D1Factory와 같이 CPU에서 보관되는 리소스
//		=> 장치 의존적인 리소스 : ID2D1RenderTarget과 같이 GPU에 보관되는 리소스
//			=> 하드웨어 가속이 사용가능하다면 GPU의 리소스에 매팽되어 빠른 그리기가 가능
// #. 장치 손실
//		=> 하드웨어 가속 리소스는 빠르고 높은 품질이라는 장점 대신 CPU/RAM에 비해 매우 한정적인 GPU/V-RAM을 사용하다 보니 리소스 부족 현상을 격게 된다.
//		=> ALT + TAB으로 작업 전환을 하거나 시스템 윈도우가 만들어질 때, 절전 모드가 작동하거나 전체화면 모드로 다른 앱이 실행되거나 등 그래픽에 민감한 작업을 할 경우 Device Lost가 발생하게 된다.
//		=> 특히 게임은 엄청난 GPU/V-RAM을 사용하므로 이런 현상이 매우 자주 발생한다. 이런 장치 손실이 발생하면, 장치 의존적인 리소스는 아무것도 사용할 수 없는 상태가 되기 때문에
//		=> 기존 리소스들은 모두 해제하고 다시 만들어야 한다.
// #. DirectX 11
//		=> 마이크로소프트에서 이런 불편함을 해소할 기능을 추가 했다.
//		=> 가상 그래픽 디바이스 인터페이스라는 개념을 사용하여 여러 개의 앱이 하나의 장치를 사용해도 문제가 없도록 개선했기 때문에 거의 발생하지 않는다.
//		=> 하지만, 실제 물리적인 장치와 연결이 끊어지는 다음과 같은 경우는 어쩔 수 없이 장치 손실이 발생한다.
//			=> 그래픽 카드 드라이버 업그레이드
//			=> 절전용 그래픽카드에서 고성능 그래픽 카드로 변경될 때
//			=> 요즘은 CPU에 내장 그래픽카드가 달린 경우가 많다. 성능이 당연히 떨어지므로 게임은 별도의 그래픽카드를 사용하는 형태 등으로 두 그래픽카드가 스위칭 되는 경우이다.
//			=> 실제 장치가 응답이 없을 경우
//			=> 외장 그래픽카드가 실제로 제거되거나 추가될 때
//		=> 기존과 달리 ALT + TAB 등의 작업 전환에서는 장치 손실이 발생하지 않는다.
//		=> Direct2D에서는 장치 손실을 체크할 수 있는 시점은 모든 그리기가 끝나서 화면에 표시하는 시점, 즉 ID2D1HwndRenderTarget::EndDraw() 함수의 반환값으로 알 수 있다.
//			=> 이 반환값이 S_OK 아니라면 장치 손실에 대한 처리를 해야 한다.
//		=> 이런 장치 의존 리소스들은 모두 모아서 한 번에 관리하고 해제해줘야 하며, 손실되면 복구해줘야 한다.
//		=> 다행히 현재 상태에서는 렌더 타겟만이 장치 의존적이다.
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