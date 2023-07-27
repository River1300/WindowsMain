#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) :
	Actor(pFramework, L"Data/bug1_1.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));

	mSteps = 0;
	mRotation = 0.0f;
	mIsDelete = false;
}

void Bug::Draw()
{
	if (mSteps++ > 10)
	{
		mSteps = 0;
		mRotation += (1 - rand() % 3) * 45;
	}

	auto targetPos = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += targetPos.x;
	mY += targetPos.y;

	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{
		0,0,
		static_cast<float>(0 + size.width),
		static_cast<float>(0 + size.height)
	};

	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(
		mRotation,
		D2D1_POINT_2F{ size.width * 0.5f, size.height * 0.5f }
	);

	mpFramework->GetRenderTarget()->SetTransform(matRotation * matTranslate);
	mpFramework->GetRenderTarget()->DrawBitmap(
		mpBitmap,
		rect,
		mOpacity
	);
}

bool Bug::IsClicked(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };

	D2D1_RECT_F rect{
		mX,mY,
		static_cast<float>(mX + size.width),
		static_cast<float>(mY + size.height)
	};

	if (pt.x >= rect.left && pt.x <= rect.right &&
		pt.y >= rect.top && pt.y <= rect.bottom)
	{
		mIsDelete = true;
		return true;
	}
	return false;
}