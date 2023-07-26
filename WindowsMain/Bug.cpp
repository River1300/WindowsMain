#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) :
	Actor(pFramework, L"Data/bug1_1.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));
}

void Bug::Draw()
{
	if (mSteps++ > 5)
	{
		mSteps = 0;
		int dir = static_cast<int>(mDirection);
		int dir_count = static_cast<int>(Direction::COUNT);
		dir += (1 - rand() % 3);
		dir = (dir + dir_count) % dir_count;	// 방향 값을 넘어가지 않도록 순환 연산

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

	Actor::Draw();
}