#include "Bug.h"

// 현재 방향으로 5번 이동하고 임의로 방향 전환

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Data/bug1_1.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));

	/*mDirection = Direction::UP;
	mSteps = 0;*/
	mSteps = 0;
	mRotation = 0.0f;
}

void Bug::Draw()
{
	/*if (mSteps++ > 20)
	{
		mSteps = 0;

		int dir = static_cast<int>(mDirection);
		int dir_count = static_cast<int>(Direction::COUNT);
		dir += (rand() % 3 - 1);
		dir = (dir + dir_count) % dir_count;

		mDirection = static_cast<Direction>(dir);
	}

	switch (mDirection)
	{
	case Direction::UP:
		mY--;
		break;
	case Direction::UP_RIGHT:
		mY--;
		mX++;
		break;
	case Direction::RIGHT:
		mX++;
		break;
	case Direction::DOWN_RIGHT:
		mY++;
		mX++;
		break;
	case Direction::DOWN:
		mY++;
		break;
	case Direction::DOWN_LEFT:
		mY++;
		mX--;
		break;
	case Direction::LEFT:
		mX--;
		break;
	case Direction::UP_LEFT:
		mY--;
		mX--;
		break;
	}

	Actor::Draw();*/

	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{
		0,0,
		static_cast<float>(size.width),
		static_cast<float>(size.height)
	};

	// 회전
	if (mSteps++ > 20)
	{
		mSteps = 0;
		mRotation += (rand() % 3 - 1) * 45.0f;
	}

	// 전진
	auto targetPos = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += targetPos.x;
	mY += targetPos.y;

	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation,
		D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });

	mpFramework->GetRenderTarget()->SetTransform(matRotation * matTranslate);

	mpFramework->GetRenderTarget()->DrawBitmap(
		mpBitmap,
		rect,
		mOpacity
	);
}
