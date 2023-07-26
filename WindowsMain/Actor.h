#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	ID2D1Bitmap* mpBitmap;	// �޸� �������� �Ŵ����� �����ϰ� �ְ� Actor�� ������ �� ���̹Ƿ� �Ϲ� �����ͷ� ����

	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();

private:
	// �̹����� �Ŵ����� �θ�
	void Draw(float x, float y, float opacity = 1.0f);

public:
	virtual void Draw();

public:
	// position ������ Getter/Setter�� �����.
	//		=> Direct2D���� ����ϴ� Vector 2���� float
	D2D_VECTOR_2F GetPosition() const { return D2D_VECTOR_2F{ mX,mY }; }
	void SetPosition(D2D_VECTOR_2F& pos) { mX = pos.x; mY = pos.y; }
	void SetPosition(float x, float y) { mX = x; mY = y; }
};