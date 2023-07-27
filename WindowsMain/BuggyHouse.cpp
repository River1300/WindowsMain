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

	CheckBugs();

	mspBackground->Draw();
	for (auto& bug : mBugList)
	{
		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET) CreateDeviceResources();
}

void BuggyHouse::CheckBugs()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)	// ���콺 ���� ��ư�� ���ȴ��� üũ
	{	// ���� ���콺�� ��ġ�� Ŭ���̾�Ʈ ��ǥ��� ���� �´�.
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mHwnd, &pt);

		// erase-remove idiom�� ������ ���� ó��, ó������ ������ �ݺ��ϸ鼭 ���ǿ� �´� ���Ҹ� ����
		auto it = std::remove_if(mBugList.begin(), mBugList.end(), [&](auto actor)
			{
				Bug* pBug = static_cast<Bug*>(actor.get());
				pBug->IsClicked(pt);

				if (pBug->mIsDelete) return true;
				else return false;
			}
		);

		mBugList.erase(it, mBugList.end());
	}
}