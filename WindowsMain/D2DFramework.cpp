#include <sstream>
#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HWND hwnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = WindowClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = D2DFramework::WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return E_FAIL;
	}

	RECT wr = { 0, 0,
		static_cast<LONG>(width),
		static_cast<LONG>(height) };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindowEx(
		NULL,
		WindowClassName,
		title,
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
		return E_FAIL;
	}

	// #. �޽����� ó���ϴ� ���� �޽��� ó�� ��ƾ ���ο��� �����ӿ�ũ Ŭ���� ����Լ��� ȣ���� ��찡 �ִ�.
	//		=> ������ ���� �Լ��� ��쿡�� Ŭ������ ������ �� ����. �ν��Ͻ��� ���ӵ� �Լ��� �ƴ϶� Ŭ������ ���� �Լ��̱� ������
	//		=> Ű���峪 ���콺 �޽��� ó�� ���� �����ӿ�ũ�� �Ѱ���� �ϴ� �͵��� ���� ������, �����ӿ�ũ�� �ʿ�
	//		=> �����츦 �����ϴ� �Լ��� CreateWindow()�Լ����� ����ڰ� ������ �����͸� ������ �� �ִ� �Ű������� �ִµ�
	//		=> �� ������ �����찡 ������ �� WM_CREATE �̺�Ʈ���� ������ �� �ִ�.
	//	#. �����츦 ������ �� SetWindowLongPtr() �Լ��� ���� �������� ����� ���� ������ �Ӽ��� ����
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	//	#. GELP_USERDATA �Ű������� �� Ŭ������ ���� �����͸� �Ѱ��༭ �޽��� ���ν������� ����� �� �ֵ��� ����� �ش�.

	mHwnd = hwnd;

	return S_OK;
}

HRESULT D2DFramework::InitD2D()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		mspD2DFactory.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	RECT wr;
	GetClientRect(mHwnd, &wr);
	hr = mspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			mHwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		mspRenderTarget.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return hr;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;

	hr = InitWindow(hInstance, title, width, height);
	ThrowIfFailed(hr);

	hr = InitD2D();
	ThrowIfFailed(hr);

	ShowWindow(mHwnd, SW_NORMAL);
	UpdateWindow(mHwnd);

	return hr;
}

void D2DFramework::Release()
{
}

void D2DFramework::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
	mspRenderTarget->EndDraw();
}

int D2DFramework::GameLoop()
{
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
			Render();
		}
	}

	return static_cast<int>(msg.wParam);
}

void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
	std::wostringstream oss;

	oss << "ERROR : 0x" << std::hex << error << '\n';
	oss << msg;
	OutputDebugString(oss.str().c_str());

	MessageBox(
		nullptr, oss.str().c_str(), title, MB_ICONEXCLAMATION | MB_OK
	);
}

LRESULT D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	D2DFramework* pFramework = reinterpret_cast<D2DFramework*>(
		(GetWindowLongPtr(hwnd, GWLP_USERDATA))
		);

	switch (message)
	{
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