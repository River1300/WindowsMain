#define WIN32_LEAN_AND_MEAN	// WIN32의 기능들 중 최소한의 기능들만을 불러올때 사용

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

Microsoft::WRL::ComPtr<ID3D11Device>		gspDevice{};	// 그래픽카드를 제어하고 관리
Microsoft::WRL::ComPtr<ID3D11DeviceContext> gspDeviceContext{};	// 3D화면을 2D로 렌더링하는 것을 정의
Microsoft::WRL::ComPtr<IDXGISwapChain>		gspSwapChain{};	// 스왑체인 : 후면 버퍼를 관리, 플립을 통해 교차, 렌더타겟을 후면으로 지정

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

// 1. 디바이스 생성(그래픽 카드), 디바이스 컨텍스트(DC) 포함
// 2. 모니터에 보내주는 스왑체인(SwapChain)
// 3. 렌더타겟을 만들고 지정(BackBuffer에 렌더타겟을 지정)
// 4. 깊이-스텐실 버퍼(누가 앞에 있는지 뒤에 있는지, 화면에 그릴 공간과 그리지 않을 공간)
// 5. 뷰포드(Viewport), 카메라와 비슷한 기능, 2D 공간
// 6. 화면에 그리기

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

	SetForegroundWindow(gHwnd);	// 윈도우를 최상단으로 올리고 활성상태로 만들어 준다.
	SetFocus(gHwnd);	// 지정 윈도우에 키보드 포커르를 지정한다.

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
{	// 디바이스, 디바이스 컨텍스트, 스왑체인 생성
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;	// 스왑체인에는 프론트버퍼와 백버퍼가 들어 있는데 옵션으로 1을 배정해 주면 백버퍼를 1개로 만든다.
//		=> 프론트버퍼는 필수이기 때문에 갯수를 지정하지 않아도 1개가 이미 있다. 지정하게되는 갯수는 백버퍼를 증가 시킨다.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 백버퍼는 그리기 용으로 사용될 것
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

	// 만들어진 버퍼를 렌더타겟으로 지정
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.ReleaseAndGetAddressOf()));
	gspDevice->CreateRenderTargetView(
		gspRenderTarget.Get(),
		nullptr,
		gspRenderTargetView.ReleaseAndGetAddressOf()
	);
	//		=> 스왑체인 안에 프론트버퍼와 백버퍼가 있는데 백버퍼에 그리기 위해 렌더타겟으로 백버퍼를 지정해 준다.
	//		=> 0번 버퍼(텍스쳐2D)를 가져와서 렌더타겟 변수에 저장한다.
	//		=> 리소스를 가져오면 리소스를 해석할 수 있는 View를 만들어준다.

		// Depth-Stencil 만들고 리소스를 해석할 수 있는 View를 만들기
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

	// 파이프라인 커스터마이징
	gspDeviceContext->OMSetRenderTargets(
		1,
		gspRenderTargetView.GetAddressOf(),
		gspDepthStencilView.Get()
	);
	//		=> 파이프라인의 Output Merger 단계에 만들어둔 View를 조립

		// 뷰포트 만들기 : 화면에 그려지는 영영
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