#pragma once

// #. WIC로 이미지를 불러오고 화면에 그리는 과정
//		=> 공용 : ID2D1HwndRenderTarget( 비트맵을 그림 ), IWICImagingFactory( WIC 디코더 생성 )
//		=> 개별 : ID2D1Bitmap( 비트맵 저장 )
#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

	// 화면에 그릴 위치와 투명도를 멤버로 저장, 파생 클래스가 사용할 수 있도록 protected 지정
	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;	// Actor클래스는 D2DFramework 정보가 없으면 아무런 처리도 할 수 없다.
//		=> 따라서 이 클래스를 사용하는 프로그래머의 실수를 막기 위해 기본 생성자를 제거한다.
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();	// 파괴자는 꼭 가상함수로

private:
	HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);	// 내부용 그리기 함수

public:
	virtual void Draw();	// 외부용 그리기 함수
};