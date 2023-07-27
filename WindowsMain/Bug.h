#pragma once
#include "Actor.h"

class Bug : public Actor
{
	const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };
	float mSteps{};
	float mRotation{};

public:
	bool mIsDelete{};	// 벌레가 죽었는지 체크할 플래그

public:
	Bug(D2DFramework* pFramework);

	virtual void Draw() override;

	bool IsClicked(POINT& pt);	// 좌표를 넘겨 주고 그 좌표 벌레 내부에 있는지 확인
};