#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include "Timer.h"
#include "Input.h"

#pragma comment (lib, "d3d11.lib")

class D3DFramework
{
private:
	const std::wstring mClassName{ L"MyWindowClass" };
	const std::wstring mTitle{ L"Direct3D Example" };

protected:
	int mScreenWidth{ 800 };
	int mScreenHeight{ 600 };

	bool mMinimized{ false };
	bool mMaximized{ false };
	bool mResizing{ false };
	bool mPaused{ false };

	std::wstring mTitleText{};
	MyUtil::Timer mTimer;
	MyUtil::Input mInput;

	HWND mHwnd{};
	HINSTANCE mInstance{};

	Microsoft::WRL::ComPtr<ID3D11Device>		mspDevice{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mspDeviceContext{};
	Microsoft::WRL::ComPtr<IDXGISwapChain>		mspSwapChain{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mspRenderTarget{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mspRenderTargetView{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mspDepthStencil{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mspDepthStencilView{};

private:
	void InitWindow(HINSTANCE hInstance);
	void InitD3D();
	void CalculateFPS();

protected:
	void RenderFrame();
	virtual void Render() {};
	virtual void Update(float delta) {};
	void OnResize();

public:
	virtual void Initialize(HINSTANCE hInstance, int width = 800, int height = 600);
	virtual void Destroy();

	void GameLoop();

public:
	LRESULT CALLBACK MessageHandle(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);