#pragma once

// #. WIC�� �̹����� �ҷ����� ȭ�鿡 �׸��� ����
//		=> ���� : ID2D1HwndRenderTarget( ��Ʈ���� �׸� ), IWICImagingFactory( WIC ���ڴ� ���� )
//		=> ���� : ID2D1Bitmap( ��Ʈ�� ���� )
#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

	// ȭ�鿡 �׸� ��ġ�� ������ ����� ����, �Ļ� Ŭ������ ����� �� �ֵ��� protected ����
	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;	// ActorŬ������ D2DFramework ������ ������ �ƹ��� ó���� �� �� ����.
//		=> ���� �� Ŭ������ ����ϴ� ���α׷����� �Ǽ��� ���� ���� �⺻ �����ڸ� �����Ѵ�.
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();	// �ı��ڴ� �� �����Լ���

private:
	HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);	// ���ο� �׸��� �Լ�

public:
	virtual void Draw();	// �ܺο� �׸��� �Լ�
};