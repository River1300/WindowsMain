#include "BuggyHouse.h"
#include "Bug.h"

HRESULT BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr, "Failed To Initialize");

	mspBackground = std::make_shared<Actor>(
		this, L"Data/back1_1024.png", 0.0f, 0.0f
		);

	for (int i = 0; i < 40; i++)
	{
		// Bug 객체를 만들면 자동으로 그림이 지정 위치에 저장된다.
		//	=> std::list<std::shared_ptr<Actor>> mBugList; 다형성의 원칙
		//		=> Bug는 Actor의 자식 + 포인터
		mBugList.push_back(std::make_shared<Bug>(this));
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
	{	// Actor의 Draw를 부르면 오버라이딩된 Bug의 Draw가 불린다.
		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET) CreateDeviceResources();
}