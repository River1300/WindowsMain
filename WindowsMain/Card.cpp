#include <string>
#include "Card.h"

using namespace Gdiplus;

namespace solitaire
{
// #5. ī�忡 ���� �⺻ �����ڸ� �����Ѵ�.
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd }, mIndex{ index }, mType{ type }, mX{ x }, mY{ y }, mIsFront{ false }
	{
		mBack = std::make_unique<Image>(L"Data/card_back.png");		// #. ī���� �޸��� �ʱ�ȭ �Ѵ�.

// #. ī���� �ո��� Type�� ���� �ٸ��� �׷��� �Ѵ�.
		std::wstring filename;
		switch (mType)
		{
		case Type::Wolf:
			filename = L"Data/card_creature_wolf.png";
			break;

		case Type::Dragon:
			filename = L"Data/card_creature_dragon.png";
			break;

		case Type::Bear:
			filename = L"Data/card_creature_bear.png";
			break;
		}
		mFront = std::make_unique<Image>(filename.c_str());
// #. GDI+�� �̹����� ���鶧�� �⺻ �����ڰ� ������ �ֱ� ������, ����� ����� �� �Ϲ� ������ ������� ����
// #. �����͸� ���� ���ϴ� ������ �ٸ� ���·� �׸��� �Ѱ��ְԲ� �ؾ� �Ѵ�.
	}


	bool Card::ChechCliked(int x, int y)
	{
		Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());

// #23. ī�尡 Ŭ���Ǹ� ī�尡 ��������.
//		=> ī�� ��ǥ�κ��� ���� ���� ���� �ȿ� x,y��ǥ�� �ִٸ� �����´�.
		if (rct.Contains(x, y))
		{
// #. rct���� �ȿ� x,y ��ǥ�� �ִ��� Ȯ��
			Flip(!mIsFront);
			return true;
		}
		return false;
	}


	void Card::Flip(bool isFront)
	{
// #6. ī�尡 �ո��� ��� �޸����� ī�尡 �޸��� ��� �ո����� ������ �ش�.
		mIsFront = isFront;
// #. �Ű� ���� isFront�� �ո�/�޸� ������ �����Ͽ� �����ϱ� ������ �״�� �����ϸ� �ȴ�.

// #27. ��������� ��Ű�� ������ ī�� �׸��� �ٲ��� �ϴ� ��ȿȭ �Լ��� ȣ���Ѵ�.
		Invalidate();
	}


	void Card::Draw(Gdiplus::Graphics& graphics)
	{
// #7. graphics��ü�� ���ؼ� ī�带 �׸���.
		if (mIsFront)
		{
			graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
		}
		else
		{
			graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
		}
	}


	void Card::Invalidate()
	{
// #28. �簢���� ���� ����� �� �簢�� ������ ��ȿȭ �Լ��� �־� �ش�.
		RECT rct{ mX,mY,
			static_cast<LONG>(mX + mFront->GetWidth()),
			static_cast<LONG>(mY + mFront->GetHeight()) };
		InvalidateRect(mHwnd, &rct, false);
	}
}