#pragma once
#include "Actor.h"

// 1. 정해진 위치에서 등장
// 2. 현재 방향으로 지속적으로 이동
// 3. 랜덤으로 45도씩 방향 전환
// 방향이라던가 각각의 객체별로 고유한 정보를 필요 => 파생을 통해서 기능 확장이 필요

class Bug : public Actor
{
	enum class Direction
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

	Direction mDirection{ Direction::UP };
	float mSteps{};

public:
	Bug(D2DFramework* pFramework);

	virtual void Draw() override;
};