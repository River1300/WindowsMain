#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D Example" };
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

HWND gHwnd{};
HINSTANCE gInstance{};

Microsoft::WRL::ComPtr<ID3D11Device>		gspDevice{};
Microsoft::WRL::ComPtr<ID3D11DeviceContext> gspDeviceContext{};
Microsoft::WRL::ComPtr<IDXGISwapChain>		gspSwapChain{};

Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspRenderTarget{};
Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	gspRenderTargetView{};

Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspDepthStencil{};
Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	gspDepthStencilView{};

void InitD3D();
void DestroyD3D();
void Render();
void OnResize();

bool gMinimized{ false };
bool gMaximized{ false };
bool gResizing{ false };

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

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
	// 윈도우가 기본 윈도우를 그리고 이후에 게임을 그리는데 이는 두 번의 그리기 작업을 실행하는 것으로 낭비다.
	//		=> 그러므로 배경 그리기를 지운다.
	wc.hbrBackground = nullptr;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register Window Class", L"ERROR", MB_OK);
		return 0;
	}

	RECT wr{ 0, 0, gScreenWidth, gScreenHeight };
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

	SetForegroundWindow(gHwnd);
	SetFocus(gHwnd);

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
	case WM_PAINT:
		// 사이즈를 변경 중일 때는 Render로 그려주고
		if (gResizing)
		{
			Render();
		}
		else
		{
			// 변경 중이 아닐 때는 윈도우 기본 그리기를 호출한다.
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_ENTERSIZEMOVE:
		gResizing = true;
		break;

	case WM_SIZE:
		gScreenWidth = LOWORD(lParam);
		gScreenHeight = HIWORD(lParam);

		if (gspDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				gMinimized = true;
				gMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				gMinimized = false;
				gMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (gMinimized)
				{
					gMinimized = false;
					OnResize();
				}
				else if (gMaximized)
				{
					gMaximized = false;
					OnResize();
				}
				else if (gResizing)
				{

				}
				else
				{
					OnResize();
				}
			}
		}
		break;

	case WM_EXITSIZEMOVE:
		gResizing = false;
		OnResize();
		break;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
		break;

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
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = gScreenWidth;
	scd.BufferDesc.Height = gScreenHeight;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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

	OnResize();
}

void DestroyD3D()
{
	gspSwapChain->SetFullscreenState(FALSE, nullptr);

	gspDepthStencil.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspRenderTargetView.Reset();

	gspSwapChain.Reset();
	gspDeviceContext.Reset();
	gspDevice.Reset();

	DestroyWindow(gHwnd);
	UnregisterClass(gClassName, gInstance);
}

void Render()
{
	const float clear_color[4]{ 0.f,0.2f,0.4f,1.0f };
	gspDeviceContext->ClearRenderTargetView(
		gspRenderTargetView.Get(),
		clear_color
	);
	gspDeviceContext->ClearDepthStencilView(
		gspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	gspSwapChain->Present(0, 0);
}

void OnResize()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	gspDeviceContext->
		OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	gspRenderTargetView.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspDepthStencil.Reset();

	gspDeviceContext->Flush();

	gspSwapChain->ResizeBuffers(
		0,
		gScreenWidth,
		gScreenHeight,
		DXGI_FORMAT_UNKNOWN,
		0
	);

	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.ReleaseAndGetAddressOf()));
	gspDevice->CreateRenderTargetView(
		gspRenderTarget.Get(),
		nullptr,
		gspRenderTargetView.ReleaseAndGetAddressOf()
	);

	CD3D11_TEXTURE2D_DESC dsd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		gScreenWidth,
		gScreenHeight,
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

	gspDeviceContext->OMSetRenderTargets(
		1,
		gspRenderTargetView.GetAddressOf(),
		gspDepthStencilView.Get()
	);

	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		static_cast<float>(gScreenWidth),
		static_cast<float>(gScreenHeight)
	);

	gspDeviceContext->RSSetViewports(1, &viewport);
}