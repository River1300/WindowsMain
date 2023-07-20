#pragma once
#include "Actor.h"


class Bug : public Actor
{
	/*enum class Direction
	{
		UP,
		UP_RIGHT,
		RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT,
		LEFT,
		UP_LEFT,
		COUNT
	};

	Direction mDirection;
	float mSteps;*/

	const D2D_POINT_2F UPVECTOR{ 0.0f,-1.0f };
	float mSteps{};
	float mRotation{};

public:
	bool mIsDelete;

public:
	Bug(D2DFramework* pFramework);

	void Draw() override;
	bool IsClicked(POINT& pt);
};

