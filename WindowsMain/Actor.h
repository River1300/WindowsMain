#pragma once
#include "D2DFramework.h"

class Actor
{
protected:	// #. �׸��� ���� ���� �⺻ ���� ��ü�̹Ƿ� ����� ���� �÷��̾�, ���� �����ϵ���
	// #. �����ڰ� ������ �� ���������� �����ӿ�ũ�� �����ϵ��� �Ѵ�.
	D2DFramework* mpFramework;
	// ID2D1Bitmap
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;
	
	// ��ġ x, y
	// ����
	float mX;
	float mY;
	float mOpacity;
	
public:
	// #. �׸��� ������ Actor Ŭ������ �ǹ̰� ����. �׷��Ƿ� �⺻ �����ڸ� ���ƹ�����.
	//		=> Actor�� ���鶧�� �ݵ�� �̹��� ������ �Ѱ� ��� �Ѵ�.
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	// #. ��� ���� �ڽ� ��ü�� �Ҹ�� �� �θ� �Բ� �Ҹ�ǵ��� �������� �����.
	virtual ~Actor();

	virtual void Draw();

private:
	HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);
};