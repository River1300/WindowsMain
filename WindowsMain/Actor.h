#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	ID2D1Bitmap* mpBitmap;	// 메모리 소유권은 매니저가 보유하고 있고 Actor는 참조만 할 것이므로 일반 포인터로 변경

	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();

private:
	// 이미지는 매니저가 부름
	void Draw(float x, float y, float opacity = 1.0f);

public:
	virtual void Draw();

public:
	// position 정보를 Getter/Setter로 만든다.
	//		=> Direct2D에서 사용하는 Vector 2차월 float
	D2D_VECTOR_2F GetPosition() const { return D2D_VECTOR_2F{ mX,mY }; }
	void SetPosition(D2D_VECTOR_2F& pos) { mX = pos.x; mY = pos.y; }
	void SetPosition(float x, float y) { mX = x; mY = y; }
};