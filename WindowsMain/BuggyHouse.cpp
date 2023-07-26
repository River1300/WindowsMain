#include "BuggyHouse.h"

HRESULT BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr, "Failed To Initialize");

	mspBackground = std::make_shared<Actor>(
		this, L"Data/back1_1024.png", 0.0f, 0.0f
		);

	float sX{}, sY{};
	RECT rct{};
	GetClientRect(mHwnd, &rct);

	for (int i = 0; i < 40; i++)
	{
		sX = static_cast<float>(rand() % (rct.right - rct.left));
		sY = static_cast<float>(rand() % (rct.bottom - rct.top));
		mBugList.push_back(
			std::make_shared<Actor>(this, L"Data/bug1_1.png", sX, sY)
		);
	}

	return S_OK;
}

void BuggyHouse::Release()
{
	for (auto bug : mBugList) bug.reset();

	mBugList.clear();
	mspBackground.reset();

	D2DFramework::Release();
}

void BuggyHouse::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();

	mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspBackground->Draw();
	for (auto& bug : mBugList)
	{	// 매 프레임마다 파리의 위치를 갱신
		auto pt = bug->GetPosition();
		pt.x += static_cast<FLOAT>(1 - rand() % 3);
		pt.y += static_cast<FLOAT>(1 - rand() % 3);
		bug->SetPosition(pt);

		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET) CreateDeviceResources();
}