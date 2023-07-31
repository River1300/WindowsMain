#define WIN32_LEAN_AND_MEAN

// #. 과거 Direct Fullscreen Mode
//	=> 전체화면 그래픽카드 독점 모드
//	=> 컨텍스트 스위칭( 알트 탭 )을 할 때 VRAM 전체를 사용하고 있던 게임이 지워지고 Window가 그자리를 차지함
//	=> 작업 전환에 매우 취약해 진다.

// #. 현재 Fullscreen - Windows Mode
//	=> Window와 Game이 동시에 하드웨어 가속을 받게 됨

// #. 해상도가 변경되면 영향을 받는 리소스
//		=> SwapChain
//		=> 렌더타겟( 텍스쳐 ) - 렌더타겟 뷰
//		=> Depth-Stencill - 뎁스-스텐실 뷰
//		=> 뷰포트
//	=> 파이프라인 업데이트

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D Example" };

// #. 윈도우의 창이 커지면 해상도 또한 커져야 한다.
//		=> WM_ENTERSIZEMOVE
//		=> WM_SIZE
//		=> WM_EXITSIZEMOVE
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

// 현재 윈도우 상태의 상태를 체크할 플래그
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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
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
		// 1. 사이즈 변경 중
	case WM_ENTERSIZEMOVE:
		gResizing = true;
		break;

		// 3. 사이즈 변경 처리
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

		// 2. 사이즈 변경 완료
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
	// 버퍼의 가로/세로를 지정
	scd.BufferDesc.Width = gScreenWidth;
	scd.BufferDesc.Height = gScreenHeight;
	// 윈도우 창의 모드를 전환할 수 있는 옵션
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
	// 종료하기 전에 현재 윈도우가 창모드인지 확인하여 창모드로 돌려준다.
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
	// #. 그래픽카드 정리
	// SwapChain의 버퍼 크기 수정
	//		=> DirectX는 GPU에서 사용되는 리소스를 다룬다.
	//		=> CPU가 GPU에게 명령을 내리면 VRAM에서 차례로 명령을 수행한다.
	//		=> CPU와 GPU의 명령 처리 속도가 서로 동기화되지 않는다면 화면의 크기가 바뀌어도 이전 크기의 버퍼를 사용할 수 있다.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	gspDeviceContext->
		OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	gspRenderTargetView.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspDepthStencil.Reset();

	gspDeviceContext->Flush();	// 쌓여있는 CPU의 명령을 전부 처리하고 오라는 동기화 함수

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