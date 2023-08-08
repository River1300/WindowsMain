#include <sstream>
#include "D3DFramework.h"

void D3DFramework::Initialize(HINSTANCE hInstance, int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	mPaused = false;

	InitWindow(hInstance);
	InitD3D();
}

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	mInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = mClassName.c_str();
	wc.hInstance = mInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register Window Class", L"ERROR", MB_OK);
		return;
	}

	RECT wr{ 0, 0, mScreenWidth, mScreenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mTitleText = mTitle;

	mHwnd = CreateWindowEx(
		NULL,
		mClassName.c_str(),
		mTitleText.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		mInstance,
		NULL
	);

	if (mHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window", L"ERROR", MB_OK);
		return;
	}

	SetWindowLongPtr(mHwnd, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_SHOW);

	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	UpdateWindow(mHwnd);
}

void D3DFramework::InitD3D()
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = mScreenWidth;
	scd.BufferDesc.Height = mScreenHeight;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = mHwnd;
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
		mspSwapChain.ReleaseAndGetAddressOf(),
		mspDevice.ReleaseAndGetAddressOf(),
		NULL,
		mspDeviceContext.ReleaseAndGetAddressOf()
	);

	OnResize();
}

void D3DFramework::CalculateFPS()
{
	static int frameCnt{ 0 };
	static float timeElapsed{ 0.0f };

	frameCnt++;

	if (mTimer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mTitleText << L" - " << L"FPS : " << fps << L" , Frame Time : " << mspf << L"(ms)";

		SetWindowText(mHwnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void D3DFramework::RenderFrame()
{
	const float clear_color[4]{ 0.f,0.2f,0.4f,1.0f };
	mspDeviceContext->ClearRenderTargetView(
		mspRenderTargetView.Get(),
		clear_color
	);
	mspDeviceContext->ClearDepthStencilView(
		mspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	Render();

	mspSwapChain->Present(0, 0);
}

void D3DFramework::OnResize()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mspDeviceContext->
		OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	mspRenderTargetView.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspDepthStencil.Reset();

	mspDeviceContext->Flush();

	mspSwapChain->ResizeBuffers(
		0,
		mScreenWidth,
		mScreenHeight,
		DXGI_FORMAT_UNKNOWN,
		0
	);

	mspSwapChain->GetBuffer(0, IID_PPV_ARGS(mspRenderTarget.ReleaseAndGetAddressOf()));
	mspDevice->CreateRenderTargetView(
		mspRenderTarget.Get(),
		nullptr,
		mspRenderTargetView.ReleaseAndGetAddressOf()
	);

	CD3D11_TEXTURE2D_DESC dsd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		mScreenWidth,
		mScreenHeight,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);
	mspDevice->CreateTexture2D(&dsd, nullptr, mspDepthStencil.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);
	mspDevice->CreateDepthStencilView(
		mspDepthStencil.Get(),
		&dsvd,
		mspDepthStencilView.ReleaseAndGetAddressOf()
	);

	mspDeviceContext->OMSetRenderTargets(
		1,
		mspRenderTargetView.GetAddressOf(),
		mspDepthStencilView.Get()
	);

	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		static_cast<float>(mScreenWidth),
		static_cast<float>(mScreenHeight)
	);

	mspDeviceContext->RSSetViewports(1, &viewport);
}

void D3DFramework::Destroy()
{
	mspSwapChain->SetFullscreenState(FALSE, nullptr);

	mspDepthStencil.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspRenderTargetView.Reset();

	mspSwapChain.Reset();
	mspDeviceContext.Reset();
	mspDevice.Reset();

	DestroyWindow(mHwnd);
	UnregisterClass(mClassName.c_str(), mInstance);
}

void D3DFramework::GameLoop()
{
	mTimer.Start();
	mInput.Initialize();

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
			mTimer.Update();

			if (mPaused)
			{
				Sleep(100);
			}
			else
			{
				CalculateFPS();

				// ���� �׸��� �и��Ѵ�.
				Update(mTimer.DeltaTime());
				RenderFrame();
			}
		}
	}
}

LRESULT D3DFramework::MessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		mInput.SetKeyDown(wParam);
		break;

	case WM_KEYUP:
		mInput.SetKeyUp(wParam);
		break;

	case WM_LBUTTONDOWN:
		mInput.SetKeyDown(VK_LBUTTON);
		break;

	case WM_LBUTTONUP:
		mInput.SetKeyUp(VK_LBUTTON);
		break;

	case WM_MOUSEMOVE:
		mInput.SetCursor(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mPaused = true;
			mTimer.Stop();
		}
		else
		{
			mPaused = false;
			mTimer.Resume();
		}
		break;

	case WM_PAINT:
		if (mResizing)
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
		mResizing = true;
		mPaused = true;
		mTimer.Stop();
		break;

	case WM_SIZE:
		mScreenWidth = LOWORD(lParam);
		mScreenHeight = HIWORD(lParam);

		if (mspDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				if (!mPaused)
				{
					mTimer.Stop();
				}
				mPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mTimer.Resume();
				mPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mMinimized)
				{
					mPaused = false;
					mTimer.Resume();
					mMinimized = false;
					OnResize();
				}
				else if (mMaximized)
				{
					mPaused = false;
					mTimer.Resume();
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
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
		mPaused = false;
		mTimer.Resume();
		mResizing = false;
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