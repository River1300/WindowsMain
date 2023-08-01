#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>

#pragma comment (lib, "d3d11.lib")

class D3DFramework
{
private:
	const std::wstring gClassName{ L"MyWindowClass" };
	const std::wstring gTitle{ L"Direct3D Example" };

protected:
	int gScreenWidth{ 800 };
	int gScreenHeight{ 600 };

	bool gMinimized{ false };
	bool gMaximized{ false };
	bool gResizing{ false };

	HWND gHwnd{};
	HINSTANCE gInstance{};

	Microsoft::WRL::ComPtr<ID3D11Device>		gspDevice{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> gspDeviceContext{};
	Microsoft::WRL::ComPtr<IDXGISwapChain>		gspSwapChain{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspRenderTarget{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	gspRenderTargetView{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			gspDepthStencil{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	gspDepthStencilView{};

private:
	void InitWindow(HINSTANCE hInstance);
	void InitD3D();

protected:
	void RenderFrame();
	void OnResize();

public:
	virtual void Initialize(HINSTANCE hInstance, int width = 800, int height = 600);
	virtual void Destroy();

	void GameLoop();

public:
	LRESULT CALLBACK MessageHandle(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);