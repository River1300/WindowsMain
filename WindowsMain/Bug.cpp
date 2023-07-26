#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) :
	Actor(pFramework, L"Data/bug1_1.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));

	mSteps = 0;
	mRotation = 0.0f;
}

void Bug::Draw()
{	// ȸ��
	if (mSteps++ > 10)
	{
		mSteps = 0;
		mRotation += (1 - rand() % 3) * 45;
	}

	auto targetPos = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += targetPos.x;
	mY += targetPos.y;

	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{
		0,0,
		static_cast<float>(0 + size.width),
		static_cast<float>(0 + size.height)
	};
	// ���� �̵� (0, 0) -> (mX, mY)�� �̵�
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	// ��Ʈ���� ȸ�� �߽��� ��Ʈ���� �� ����� ����ؾ� �Ѵ�.
	//		=> size.width * 0.5f, ...�� �Ѱ��־ ����� �����Ѵ�.
	auto matRotation = D2D1::Matrix3x2F::Rotation(
		mRotation,
		D2D1_POINT_2F{ size.width * 0.5f, size.height * 0.5f }
	);
	// ����Ÿ�� ��ü�� ����
	//		=> �� �� �����ϸ� ������ ��� �׸��⿡ ������ �ش�.
	mpFramework->GetRenderTarget()->SetTransform(matRotation * matTranslate);
	//		=> ȸ���� ������ �ִٴ� ���� ����Ʈ, ȸ�� ����� ������ �̹����� �߽�
	//		=> ȸ���� ������ �Ŀ� �̵�
	mpFramework->GetRenderTarget()->DrawBitmap(
		mpBitmap,
		rect,
		mOpacity
	);
}