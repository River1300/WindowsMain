#include <Windows.h>
// #. 사전 작업
#include <d2d1.h>	// Direct2D를 사용하기 위해서는 <d2d1.h>를 포함시켜 줘야 한다.
#pragma comment(lib, "d2d1.lib")	// 헤더와 함께 라이브러리 포함

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// #. Direct2D 오브젝트는 프로그램의 시작/종료까지 관리할 필요가 있다. 예제를 위해서 임시로 전역 변수화 한다.
//		=> Interface Direct 2D [ID2D]의 약자 형태로 COM을 사용하는 코드에서 자주 등장한다.
ID2D1Factory* gpD2DFactory{};
ID2D1HwndRenderTarget* gpRenderTarget{};
// 간단한 그림 그려보기 1. 브러시 변수 선언
ID2D1SolidColorBrush* gpBrush{};	// GDI와 마찬가지로 브러시가 준비되어있다. 단색, 그라데이션 브러시를 선언해 보았다.
ID2D1RadialGradientBrush* gpRadialBrush{};

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// #1. Direct2D Factory 생성 -> 제거를 항상 짝으로 만든다.
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory
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

	// #2. 렌더 타겟 생성 : 그릴 대상을 렌더 타겟이라 부른다. 메모리, 파일, 화면등에 그릴 수 있다.
	//		=> 메모리에 그리면 그 메모리를 이용하여 특수 효과에 사용할 수 있고
	//		=> 파일에 그리면 스크린샷이 된다.
	//		=> 사실 화면에 그린다는 것 또한 그래픽 카드의 메모리에 그리는 것이다.
	GetClientRect(hwnd, &wr);	// 윈도우 핸들로 부터 윈도우의 클라이언트 영역을 구해 온다.

	// 윈도우 핸들에서 렌더타겟을 생성, 즉 윈도우 내부에 그래픽을 그리게 되는 것
	hr = gpD2DFactory->CreateHwndRenderTarget(
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

	// 간단한 그림 그려보기 2. 브러시 생성
		// 색 브러시를 생성한다.
	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)),
		&gpBrush
	);
	// 그라데이션은 시작지점에서 종료지점까지 색상을 자연스럽게 변형시키는 방식이다.
	// 이를 표현하기 위해서 시작지점, 끝지점에 대한 정보가 필요하며 이를 GradientStop이라 한다.
	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];
	// 그라데이션의 분기점을 GradientStop이라 부르며 이를 모아둔 배열을 GradientStopCollection이라 한다.
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
	// 원형 그라데이션 브러시 생성
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
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

	// #4. 해제
	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	// #1. Direct2D Factory 생성 -> 제거를 항상 짝으로 만든다.
	if (gpD2DFactory != nullptr)
	{
		gpD2DFactory->Release();
		gpD2DFactory = nullptr;
	}

	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	// #3. 그리기
		// BeginDraw() / EndDraw() 렌더 타겟에 그릴 준비를 한다. GDI와 비슷한 개념으로 시작과 끝은 각각 지정하면,
		//		=> gpRenderTarget 내부에서 필요한 작업들을 수행해 준다.
	gpRenderTarget->BeginDraw();

	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));	// 렌더타겟의 화면을 특정 생삭으로 지움

// 간단한 그림 그려보기 3. 사각형 및 타원 그리기
	// 속을 채운 사각형, 타원 그리기
	gpRenderTarget->FillRectangle(
		D2D1::Rect(0.0f, 0.0f, 100.0f, 100.0f),
		gpBrush
	);
	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(50.0f, 150.0f), 50.0f, 50.0f),
		gpRadialBrush
	);

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