#include <Windows.h>
#include <d2d1.h>
#include <wrl/client.h>	// ComPtr�� ����ϱ� ���� ���
#include <math.h>
#pragma comment(lib, "d2d1.lib")

// #. ������ ��ũ : ����, ü�踦 ���Ѵ�.
//		=> ���α׷��ֿ����� Ư�� �ü��, Ư�� �׷��� ���̺귯���� ���� �������α׷� ǥ�� ������ ������ Ŭ������ ���̺귯���� ���ø����̼� �����ӿ�ũ�� �θ���.
// Library : ���α׷����� ���� ����� ���� �Լ� �� ��ɵ��� ��Ƶ� ��� (.h)(.lib)
// Application Programming Interface : �� ������ ���� �ַ� �ü������ �ʿ��� ��ɵ��� �����ϴ� �������̽�, �۰� �ϵ��� ������ �ִ� ����
// Software Develoment Kit : API�� ���� ���� �ǹ������� ������ �����ִ� �پ��� ������ ����
// Framework : �� �������� ���� ������ �����ִ� ����̷� ���� ������ �帧���� �������� �ʴ´�.
//		=> �׷��� ���̺귯��, ���� ���̺귯�� ��� ���� Ư���� ����� ���ϰ� ���� �� �ְ� ������ ���̴�.
//		=> ������ �����ӿ�ũ�� ������ �ٸ���. �̷� �������� ��Ƽ� �پ��� ���� ���� �� �ֵ��� ���� �⺻ �帧�� �����Ѵ�. ������ �����Ϳ� ����� ���� ���� ���� ��쵵 �����ӿ�ũ�� ������ ���� ����ؾ߸� �Ѵ�.
//		=> �׷���, ����� ���� ������ ��ɵ��� ���̺귯���� SDK�� ������ �ް�, �⺻���� �ʱ�ȭ ���� �۾��� �̸� �۾��� �θ� �װ��� �����ӿ�ũ�� �ȴ�.

// #. Microsoft::WRL::ComPtr
//		=> COM������Ʈ�� ���� ����Ʈ ������ ����� �߰�
//		=> std::shared_ptr�� ���� ������� ���ο� ���� ī��Ʈ�� ������ ������ ���������� ����ϴٰ� ���� ī��Ʈ�� 0�̵Ǹ� �����Ǵ� ����� ���ϰ� �ִ�.
const wchar_t gClassName[] = L"MyWindowClass";

void OnPaint(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};	// ComPtr�� Factory�� ����
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
	// �ʱ�ȭ �� ����
	//		=> �Ϲ������� COM��ü�� Factory �Ǵ� ���� �Լ��� ���ؼ� �����ǰ�, �Ű������� �������̽��� ���� �������� �ּ�(**)�� �Ѱ��ִ� �����̴�.
	//		=> ComPtr�� �̷� Ư���� GetAddressOf() �޼���� �����Ѵ�.
	//		=> ���� ComPtr ������Ʈ�� Ŭ���� ����� ����ϰ� �ְ�, �̹� �ʱ�ȭ�� �Ǿ� �ִ��� ��Ȯ���� ������ �����ϰ� ReleaseAndGetAddressOf() �޼��带 ����� �� �ִ�.
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

	// ������ �ڵ����� ���־� ������ ��������� �����ϰ� ���� ���� Reset()�޼��带 ����Ѵ�.
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