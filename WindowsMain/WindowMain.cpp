#include <Windows.h>
// #. ���� �۾�
#include <d2d1.h>	// Direct2D�� ����ϱ� ���ؼ��� <d2d1.h>�� ���Խ��� ��� �Ѵ�.
#pragma comment(lib, "d2d1.lib")	// ����� �Բ� ���̺귯�� ����

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// #. Direct2D ������Ʈ�� ���α׷��� ����/������� ������ �ʿ䰡 �ִ�. ������ ���ؼ� �ӽ÷� ���� ����ȭ �Ѵ�.
//		=> Interface Direct 2D [ID2D]�� ���� ���·� COM�� ����ϴ� �ڵ忡�� ���� �����Ѵ�.
ID2D1Factory* gpD2DFactory{};
ID2D1HwndRenderTarget* gpRenderTarget{};
// ������ �׸� �׷����� 1. �귯�� ���� ����
ID2D1SolidColorBrush* gpBrush{};	// GDI�� ���������� �귯�ð� �غ�Ǿ��ִ�. �ܻ�, �׶��̼� �귯�ø� ������ ���Ҵ�.
ID2D1RadialGradientBrush* gpRadialBrush{};

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// #1. Direct2D Factory ���� -> ���Ÿ� �׻� ¦���� �����.
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

	// #2. ���� Ÿ�� ���� : �׸� ����� ���� Ÿ���̶� �θ���. �޸�, ����, ȭ�� �׸� �� �ִ�.
	//		=> �޸𸮿� �׸��� �� �޸𸮸� �̿��Ͽ� Ư�� ȿ���� ����� �� �ְ�
	//		=> ���Ͽ� �׸��� ��ũ������ �ȴ�.
	//		=> ��� ȭ�鿡 �׸��ٴ� �� ���� �׷��� ī���� �޸𸮿� �׸��� ���̴�.
	GetClientRect(hwnd, &wr);	// ������ �ڵ�� ���� �������� Ŭ���̾�Ʈ ������ ���� �´�.

	// ������ �ڵ鿡�� ����Ÿ���� ����, �� ������ ���ο� �׷����� �׸��� �Ǵ� ��
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

	// ������ �׸� �׷����� 2. �귯�� ����
		// �� �귯�ø� �����Ѵ�.
	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)),
		&gpBrush
	);
	// �׶��̼��� ������������ ������������ ������ �ڿ������� ������Ű�� ����̴�.
	// �̸� ǥ���ϱ� ���ؼ� ��������, �������� ���� ������ �ʿ��ϸ� �̸� GradientStop�̶� �Ѵ�.
	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];
	// �׶��̼��� �б����� GradientStop�̶� �θ��� �̸� ��Ƶ� �迭�� GradientStopCollection�̶� �Ѵ�.
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
	// ���� �׶��̼� �귯�� ����
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

	// #4. ����
	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	// #1. Direct2D Factory ���� -> ���Ÿ� �׻� ¦���� �����.
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

	// #3. �׸���
		// BeginDraw() / EndDraw() ���� Ÿ�ٿ� �׸� �غ� �Ѵ�. GDI�� ����� �������� ���۰� ���� ���� �����ϸ�,
		//		=> gpRenderTarget ���ο��� �ʿ��� �۾����� ������ �ش�.
	gpRenderTarget->BeginDraw();

	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));	// ����Ÿ���� ȭ���� Ư�� �������� ����

// ������ �׸� �׷����� 3. �簢�� �� Ÿ�� �׸���
	// ���� ä�� �簢��, Ÿ�� �׸���
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