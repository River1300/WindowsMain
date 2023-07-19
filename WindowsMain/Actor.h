#pragma once
#include "D2DFramework.h"

class Actor
{
protected:	// #. 그리기 위한 가장 기본 적인 객체이므로 상속을 통해 플레이어, 적을 관리하도록
	// #. 생성자가 생성될 때 강제적으로 프레임워크를 전달하도록 한다.
	D2DFramework* mpFramework;
	// ID2D1Bitmap
	ID2D1Bitmap* mpBitmap;
	
	// 위치 x, y
	// 투명도
	float mX;
	float mY;
	float mOpacity;
	
public:
	// #. 그림이 없으면 Actor 클래스는 의미가 없다. 그러므로 기본 생성자를 막아버린다.
	//		=> Actor를 만들때는 반드시 이미지 파일을 넘겨 줘야 한다.
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	// #. 상속 받은 자식 객체가 소멸될 떄 부모도 함께 소멸되도록 가상으로 만든다.
	virtual ~Actor();

	virtual void Draw();

private:
	//HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);

public:
	D2D_VECTOR_2F GetPosition() const { return D2D_VECTOR_2F{ mX,mY }; }
	void SetPosition(D2D_VECTOR_2F& pos) { mX = pos.x; mY = pos.y; }
	void SetPosition(float x, float y) { mX = x; mY = y; }
};