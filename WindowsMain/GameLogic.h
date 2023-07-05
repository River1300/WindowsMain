#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	// #9. 게임이 구동되는 진행 방식을 만들어 준다.
	class GameLogic
	{
		// #.10 윈도우 화면을 구성할 멤버를 만든다.
		const int BOARD_ROW = 5;
		const int BOARD_COLUMN = 8;
		const Gdiplus::RectF mCountRect{ 885.0f, 60.0f, 120.f,30.0f };

	private:
		// #11. 배경 이미지와 카드를 담을 컨테이너를 만들어 준다.
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;

		// #29. 게임 로직에도 윈도우 핸들을 만들어 준다.
		HWND mHwnd;

		int mFlipCount;		// #. 몇 번 클릭 했는지 카운트

// #33. 이전에 선택한 카드 정보를 갖고있다.
		Card* mpSelectedCard;
	public:
		// #12. 생성자와 소멸자가 하는 일을 임의로 멤버 함수를 만들어서 명시적으로 만들어 준다.
		void Init(HWND hwnd);
		void Release();

		void Draw(Gdiplus::Graphics& graphics);		// #. 게임 전체를 그리는 함수
		void OnClick(int x, int y);					// #. 클릭했을 때를 나타내는 함수

	private:
		void CreateCards();							// #. 카드 만드는 함수
	};
}