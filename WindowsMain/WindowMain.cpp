#include <Windows.h>
#include <d2d1.h>
#include <wrl/client.h>	// ComPtr을 사용하기 위한 헤더
#include <math.h>
#pragma comment(lib, "d2d1.lib")

// #. 프레임 워크 : 뼈대, 체계를 뜻한다.
//		=> 프로그래밍에서는 특정 운영체제, 특정 그래픽 라이브러리를 위한 응용프로그램 표준 구조를 구현한 클래스와 라이브러리를 애플리케이션 프레임워크라 부른다.
// Library : 프로그램에서 자주 사용할 편리한 함수 및 기능들을 모아둔 모듈 (.h)(.lib)
// Application Programming Interface : 앱 개발을 위해 주로 운영체제에서 필요한 기능들을 제공하는 인터페이스, 앱과 하드웨어를 연결해 주는 역할
// Software Develoment Kit : API와 거의 같은 의미이지만 개발을 도와주는 다양한 도구를 제공
// Framework : 위 도구들은 게임 개발을 도와주는 도우미로 실제 게임의 흐름에는 관여하지 않는다.
//		=> 그래픽 라이브러리, 사운드 라이브러리 등과 같이 특정한 기능을 편리하게 만들 수 있게 도와줄 뿐이다.
//		=> 하지만 프레임워크는 개념이 다르다. 이런 도구들을 모아서 다양한 앱을 만들 수 있도록 앱의 기본 흐름을 규정한다. 심지어 데이터에 사용할 파일 포멧 같은 경우도 프레임워크가 지정한 것을 사용해야만 한다.
//		=> 그래픽, 사운드와 같은 복잡한 기능들은 라이브러리나 SDK의 도움을 받고, 기본적인 초기화 해제 작업을 미리 작업해 두면 그것이 프레임워크가 된다.

// #. Microsoft::WRL::ComPtr
//		=> COM오브젝트에 대한 스마트 포인터 기능을 추가
//		=> std::shared_ptr과 같은 방식으로 내부에 참조 카운트를 가지고 있으며 여러곳에서 사용하다가 참조 카운트가 0이되면 해제되는 방식을 취하고 있다.
const wchar_t gClassName[] = L"MyWindowClass";

void OnPaint(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};	// ComPtr로 Factory를 선언
ID2D1HwndRenderTarget* gpRenderTarget{};
ID2D1SolidColorBrush* gpBrush{};
ID2D1RadialGradientBrush* gpRadialBrush{};

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 초기화 및 해제
	//		=> 일반적으로 COM객체는 Factory 또는 헬퍼 함수를 통해서 생성되고, 매개변수로 인터페이스에 대한 포인터의 주소(**)를 넘겨주는 형태이다.
	//		=> ComPtr은 이런 특성을 GetAddressOf() 메서드로 제공한다.
	//		=> 만약 ComPtr 오브젝트를 클래스 멤버로 사용하고 있고, 이미 초기화가 되어 있는지 명확하지 않으면 안전하게 ReleaseAndGetAddressOf() 메서드를 사용할 수 있다.
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		MessageBox(
			nullptr, L"Failed to Create D2D Factory!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND hwnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RECT wr = { 0, 0, 1024, 768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Direct2D",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create Window Class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	GetClientRect(hwnd, &wr);
	hr = mspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
		&gpRenderTarget
	);

	if (FAILED(hr))
	{
		MessageBox(
			nullptr, L"Failed To Create D2D RenderTarget!", L"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)),
		&gpBrush
	);

	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1);
	gradientStops[1].position = 1.0f;

	hr = gpRenderTarget->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);
	if (SUCCEEDED(hr))
	{
		hr = gpRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(50, 150),
				D2D1::Point2F(0, 0),
				50,
				50),
			pGradientStops,
			&gpRadialBrush
		);
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			OnPaint(hwnd);
		}

	}

	if (pGradientStops != nullptr)
	{
		pGradientStops->Release();
		pGradientStops = nullptr;
	}

	if (gpRadialBrush != nullptr)
	{
		gpRadialBrush->Release();
		gpRadialBrush = nullptr;
	}

	if (gpBrush != nullptr)
	{
		gpBrush->Release();
		gpBrush = nullptr;
	}

	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	//if (gpD2DFactory != nullptr)
	//{
	//	gpD2DFactory->Release();
	//	gpD2DFactory = nullptr;
	//}

	// 해제는 자동으로 이주어 지지만 명시적으로 수행하고 싶을 때는 Reset()메서드를 사용한다.
	mspD2DFactory.Reset();
	mspD2DFactory = nullptr;

	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	gpBrush->SetOpacity(1.0f);
	gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));
	gpRenderTarget->FillRectangle(
		D2D1::Rect(0.0f, 0.0f, 100.0f, 100.0f),
		gpBrush
	);
	gpBrush->SetOpacity(0.5f);
	gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
	gpRenderTarget->FillRectangle(
		D2D1::Rect(50.0f, 50.0f, 150.0f, 150.0f),
		gpBrush
	);

	static float fAngle = 0.0f;
	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(75.0f + sinf(fAngle) * 25, 150.0f), 50.0f, 50.0f),
		gpRadialBrush
	);
	fAngle += 0.2f;

	gpRenderTarget->EndDraw();
	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}