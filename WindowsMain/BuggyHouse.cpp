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
		// Bug ��ü�� ����� �ڵ����� �׸��� ���� ��ġ�� ����ȴ�.
		//	=> std::list<std::shared_ptr<Actor>> mBugList; �������� ��Ģ
		//		=> Bug�� Actor�� �ڽ� + ������
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
	{	// Actor�� Draw�� �θ��� �������̵��� Bug�� Draw�� �Ҹ���.
		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET) CreateDeviceResources();
}