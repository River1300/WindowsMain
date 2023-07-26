#pragma once
#include "Actor.h"

// 1. ������ ��ġ���� ����
// 2. ���� �������� ���������� �̵�
// 3. �������� 45���� ���� ��ȯ
// �����̶���� ������ ��ü���� ������ ������ �ʿ� => �Ļ��� ���ؼ� ��� Ȯ���� �ʿ�

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