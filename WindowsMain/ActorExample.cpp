#include "ActorExample.h"

HRESULT ActorExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr);

	// ADDED
	mspBackground = std::make_unique<Actor>(this, L"Data/back1_1024.png", 0, 0);
	mspBug = std::make_unique<Actor>(this, L"Data/bug1_1.png", 100, 100);

	return S_OK;
}

void ActorExample::Release()
{
	mspBug.reset();
	mspBackground.reset();

	D2DFramework::Release();
}

void ActorExample::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	// ADDED
	mspBackground->Draw();
	mspBug->Draw();

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET) CreateDeviceResources();
}