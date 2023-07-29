#define WIN32_LEAN_AND_MEAN	// WIN32�� ��ɵ� �� �ּ����� ��ɵ鸸�� �ҷ��ö� ���

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D Example" };
const int WINDOW_WIDTH{ 800 };
const int WINDOW_HEIGHT{ 600 };

HWND gHwnd{};
HINSTANCE gInstance{};

Microsoft::WRL::ComPtr<ID3D11Device>		gspDevice{};	// �׷���ī�带 �����ϰ� ����
Microsoft::WRL::ComPtr<ID3D11DeviceContext> gspDeviceContext{};	// 3Dȭ���� 2D�� �������ϴ� ���� ����
Microsoft::WRL::ComPtr<IDXGISwapChain>		gspSwapChain{};	// ����ü�� : �ĸ� ���۸� ����, �ø��� ���� ����, ����Ÿ���� �ĸ����� ����

// RenderTarget
Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspRenderTarget{};
Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	gspRenderTargetView{};

// DepthStencil
Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspDepthStencil{};
Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	gspDepthStencilView{};

void InitD3D();
void DestroyD3D();
void Render();

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// 1. ����̽� ����(�׷��� ī��), ����̽� ���ؽ�Ʈ(DC) ����
// 2. ����Ϳ� �����ִ� ����ü��(SwapChain)
// 3. ����Ÿ���� ����� ����(BackBuffer�� ����Ÿ���� ����)
// 4. ����-���ٽ� ����(���� �տ� �ִ��� �ڿ� �ִ���, ȭ�鿡 �׸� ������ �׸��� ���� ����)
// 5. ������(Viewport), ī�޶�� ����� ���, 2D ����
// 6. ȭ�鿡 �׸���

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	gInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = gInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register Window Class", L"ERROR", MB_OK);
		return 0;
	}

	RECT wr{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	gHwnd = CreateWindowEx(
		NULL,
		gClassName,
		gTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		gInstance,
		NULL
	);

	if (gHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window", L"ERROR", MB_OK);
		return 0;
	}

	ShowWindow(gHwnd, nShowCmd);

	SetForegroundWindow(gHwnd);	// �����츦 �ֻ������ �ø��� Ȱ�����·� ����� �ش�.
	SetFocus(gHwnd);	// ���� �����쿡 Ű���� ��Ŀ���� �����Ѵ�.

	UpdateWindow(gHwnd);

	InitD3D();

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			// GAME CODE
			Render();
		}
	}

	DestroyD3D();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitD3D()
{	// ����̽�, ����̽� ���ؽ�Ʈ, ����ü�� ����
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;	// ����ü�ο��� ����Ʈ���ۿ� ����۰� ��� �ִµ� �ɼ����� 1�� ������ �ָ� ����۸� 1���� �����.
//		=> ����Ʈ���۴� �ʼ��̱� ������ ������ �������� �ʾƵ� 1���� �̹� �ִ�. �����ϰԵǴ� ������ ����۸� ���� ��Ų��.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ����۴� �׸��� ������ ���� ��
	scd.OutputWindow = gHwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		0,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		gspSwapChain.ReleaseAndGetAddressOf(),
		gspDevice.ReleaseAndGetAddressOf(),
		NULL,
		gspDeviceContext.ReleaseAndGetAddressOf()
	);

	// ������� ���۸� ����Ÿ������ ����
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.ReleaseAndGetAddressOf()));
	gspDevice->CreateRenderTargetView(
		gspRenderTarget.Get(),
		nullptr,
		gspRenderTargetView.ReleaseAndGetAddressOf()
	);
	//		=> ����ü�� �ȿ� ����Ʈ���ۿ� ����۰� �ִµ� ����ۿ� �׸��� ���� ����Ÿ������ ����۸� ������ �ش�.
	//		=> 0�� ����(�ؽ���2D)�� �����ͼ� ����Ÿ�� ������ �����Ѵ�.
	//		=> ���ҽ��� �������� ���ҽ��� �ؼ��� �� �ִ� View�� ������ش�.

		// Depth-Stencil ����� ���ҽ��� �ؼ��� �� �ִ� View�� �����
	CD3D11_TEXTURE2D_DESC dsd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);
	gspDevice->CreateTexture2D(&dsd, nullptr, gspDepthStencil.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);
	gspDevice->CreateDepthStencilView(
		gspDepthStencil.Get(),
		&dsvd,
		gspDepthStencilView.ReleaseAndGetAddressOf()
	);

	// ���������� Ŀ���͸���¡
	gspDeviceContext->OMSetRenderTargets(
		1,
		gspRenderTargetView.GetAddressOf(),
		gspDepthStencilView.Get()
	);
	//		=> ������������ Output Merger �ܰ迡 ������ View�� ����

		// ����Ʈ ����� : ȭ�鿡 �׷����� ����
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		static_cast<float>(WINDOW_WIDTH),
		static_cast<float>(WINDOW_HEIGHT)
	);

	gspDeviceContext->RSSetViewports(1, &viewport);
}

void DestroyD3D()
{
	gspSwapChain.Reset();
	gspDeviceContext.Reset();
	gspDevice.Reset();

	DestroyWindow(gHwnd);
	UnregisterClass(gClassName, gInstance);
}

void Render()
{

}