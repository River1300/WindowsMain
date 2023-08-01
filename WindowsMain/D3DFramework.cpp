#include "D3DFramework.h"

void D3DFramework::Initialize(HINSTANCE hInstance, int width, int height)
{
	gScreenWidth = width;
	gScreenHeight = height;

	InitWindow(hInstance);
	InitD3D();
}

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	gInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName.c_str();
	wc.hInstance = gInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register Window Class", L"ERROR", MB_OK);
		return;
	}

	RECT wr{ 0, 0, gScreenWidth, gScreenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	gHwnd = CreateWindowEx(
		NULL,
		gClassName.c_str(),
		gTitle.c_str(),
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
		return;
	}

	SetWindowLongPtr(gHwnd, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this));

	ShowWindow(gHwnd, SW_SHOW);

	SetForegroundWindow(gHwnd);
	SetFocus(gHwnd);

	UpdateWindow(gHwnd);
}

void D3DFramework::InitD3D()
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

void D3DFramework::RenderFrame()
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

void D3DFramework::OnResize()
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

void D3DFramework::Destroy()
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
	UnregisterClass(gClassName.c_str(), gInstance);
}

void D3DFramework::GameLoop()
{
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
			RenderFrame();
		}
	}
}

LRESULT D3DFramework::MessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		if (gResizing)
		{
			RenderFrame();
		}
		else
		{
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

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto pFramework = reinterpret_cast<D3DFramework*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pFramework->MessageHandle(hWnd, message, wParam, lParam);
}