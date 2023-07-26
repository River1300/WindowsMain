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
}

void Bug::Draw()
{	// 회전
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
	// 평행 이동 (0, 0) -> (mX, mY)로 이동
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	// 비트맵의 회전 중심은 비트맵의 한 가운데를 사용해야 한다.
	//		=> size.width * 0.5f, ...를 넘겨주어서 가운데를 지정한다.
	auto matRotation = D2D1::Matrix3x2F::Rotation(
		mRotation,
		D2D1_POINT_2F{ size.width * 0.5f, size.height * 0.5f }
	);
	// 렌더타겟 전체에 적용
	//		=> 한 번 지정하면 이후의 모든 그리기에 영향을 준다.
	mpFramework->GetRenderTarget()->SetTransform(matRotation * matTranslate);
	//		=> 회전은 원점이 있다는 것이 포인트, 회전 행렬의 원점은 이미지의 중심
	//		=> 회전을 수행한 후에 이동
	mpFramework->GetRenderTarget()->DrawBitmap(
		mpBitmap,
		rect,
		mOpacity
	);
}