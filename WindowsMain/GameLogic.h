#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	// #9. ������ �����Ǵ� ���� ����� ����� �ش�.
	class GameLogic
	{
		// #.10 ������ ȭ���� ������ ����� �����.
		const int BOARD_ROW = 5;
		const int BOARD_COLUMN = 8;
		const Gdiplus::RectF mCountRect{ 885.0f, 60.0f, 120.f,30.0f };

	private:
		// #11. ��� �̹����� ī�带 ���� �����̳ʸ� ����� �ش�.
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;

		// #29. ���� �������� ������ �ڵ��� ����� �ش�.
		HWND mHwnd;

		int mFlipCount;		// #. �� �� Ŭ�� �ߴ��� ī��Ʈ

// #33. ������ ������ ī�� ������ �����ִ�.
		Card* mpSelectedCard;
	public:
		// #12. �����ڿ� �Ҹ��ڰ� �ϴ� ���� ���Ƿ� ��� �Լ��� ���� ��������� ����� �ش�.
		void Init(HWND hwnd);
		void Release();

		void Draw(Gdiplus::Graphics& graphics);		// #. ���� ��ü�� �׸��� �Լ�
		void OnClick(int x, int y);					// #. Ŭ������ ���� ��Ÿ���� �Լ�

	private:
		void CreateCards();							// #. ī�� ����� �Լ�
	};
}