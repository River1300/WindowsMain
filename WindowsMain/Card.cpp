#include <string>
#include "Card.h"

using namespace Gdiplus;

namespace solitaire
{
// #5. 카드에 대한 기본 생성자를 정의한다.
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd }, mIndex{ index }, mType{ type }, mX{ x }, mY{ y }, mIsFront{ false }
	{
		mBack = std::make_unique<Image>(L"Data/card_back.png");		// #. 카드의 뒷면을 초기화 한다.

// #. 카드의 앞면은 Type에 따라 다르게 그려야 한다.
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
// #. GDI+의 이미지를 만들때는 기본 생성자가 숨겨져 있기 때문에, 멤버를 사용할 때 일반 변수를 사용하지 말고
// #. 포인터를 만들어서 원하는 시점에 다른 형태로 그림을 넘겨주게끔 해야 한다.
	}


	bool Card::ChechCliked(int x, int y)
	{
		Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());

// #23. 카드가 클리되면 카드가 뒤집힌다.
//		=> 카드 좌표로부터 가로 세로 범위 안에 x,y좌표가 있다면 뒤집는다.
		if (rct.Contains(x, y))
		{
// #. rct영역 안에 x,y 좌표가 있는지 확인
			Flip(!mIsFront);
			return true;
		}
		return false;
	}


	void Card::Flip(bool isFront)
	{
// #6. 카드가 앞면일 경우 뒷면으로 카드가 뒷면일 경우 앞면으로 뒤집어 준다.
		mIsFront = isFront;
// #. 매개 변수 isFront에 앞면/뒷면 정보를 저장하여 전달하기 때문에 그대로 대입하면 된다.

// #27. 뒤집으라고 시키면 무조건 카드 그림이 바뀌어야 하니 무효화 함수를 호출한다.
		Invalidate();
	}


	void Card::Draw(Gdiplus::Graphics& graphics)
	{
// #7. graphics객체를 통해서 카드를 그린다.
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
// #28. 사각형을 먼저 만들고 이 사각형 영역을 무효화 함수에 넣어 준다.
		RECT rct{ mX,mY,
			static_cast<LONG>(mX + mFront->GetWidth()),
			static_cast<LONG>(mY + mFront->GetHeight()) };
		InvalidateRect(mHwnd, &rct, false);
	}
}