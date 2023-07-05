#include <string>
#include <vector>
#include <random>
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;

		mpSelectedCard = nullptr;

		// #13. 배경을 불러와서 mBackground에 넣어 초기화 해준다.
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		// #14. 40장의 카드를 만든다.
		CreateCards();
	}


	void GameLogic::Release()
	{
		// #15. 만들어준 이미지를 모두 해제한다.
		mDeck.clear();
		mBackground.reset();
	}


	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		// #16. 메모리 공간에 할당된 이미지를 윈도우에 그려준다. 배경 먼저 그리고 그 다음에 카드
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		// #17. 클릭수를 화면에 그려준다.
		Gdiplus::PointF pos(895.0f, 20.0f);							// #. 좌표를 정해 주고
		Gdiplus::Font font(L"맑은 고딕", 20);						// #. 폰트를 정해 주고
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));		// #. 브러쉬를 정해 주고
		graphics.DrawString(L"클릭수: ", -1, &font, pos, &brush);	// #. 이제 그린다.

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);		// #. 가로너비 중앙 정렬
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);	// #. 세로너비 중앙 정렬
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, mCountRect, &format, &brush);
	}


	void GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};

		// #18. 클릭 되었을 때 관리하고 있는 40장의 카드에 클릭 되었는지 물어보는 코드
		for (auto& card : mDeck)
		{
			if (card.ChechCliked(x, y))
			{
				// #34. 현재 선택된 카드를 찾으면 for문을 빠져 나온다.
				pCard = &card;
				break;
			}
		}
		if (pCard != nullptr)	// # 카드를 정상적으로 선택하였을 경우
		{
			mFlipCount++;
			RECT rct{
				static_cast<LONG>(mCountRect.GetLeft()),
				static_cast<LONG>(mCountRect.GetTop()),
				static_cast<LONG>(mCountRect.GetRight()),
				static_cast<LONG>(mCountRect.GetBottom())
			};
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)		// #. 이전에 선택한 카드가 없을 경우
			{
				mpSelectedCard = pCard;
			}
			else
			{
				if (mpSelectedCard == pCard)	// #. 동일한 카드를 중복해서 선택하였을 경우
				{
					mpSelectedCard == nullptr;
				}
				else
				{
					if (pCard->GetType() == mpSelectedCard->GetType())	// #. 선택된 두 개의 카드가 같은 타입일 경우
					{
						// #36. 카드를 지우기 전에 윈도우를 갱신해 준다.
						pCard->Invalidate();
						mpSelectedCard->Invalidate();

						// #35. 컨테이너에 있는 알고리즘을 활용하여 동일한 카드를 지운다.
						//		=> 고유 번호가 지정된( index )카드를 지울 수 있는 람다식 함수
						mDeck.remove_if([&](Card& card)
							{	// #. mpSelecedCard는 람다식 밖에 있는 변수이기 때문에 캡쳐절을 참조하여 받는다.
								return card.GetIndex() == mpSelectedCard->GetIndex();
							}
						);
						mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
						//		=> 카드 40장이 있는 컨테이너( mDeck )에서 동일한 index를 갖는 Card를 2장 지운다.

						mpSelectedCard = nullptr;
					}
					else
					{
						//	#37. 카드가 다를때 바로 뒤집히면 어떤 카드인지 확인이 어렵기 때문에 잠시 멈추어 준다.
						UpdateWindow(mHwnd);
						Sleep(500);

						pCard->Flip(false);
						mpSelectedCard->Flip(false);

						mpSelectedCard = nullptr;
					}
				}
			}
		}
	}


	void GameLogic::CreateCards()
	{
		// #30. 벡터에 40장의 카드를 만들어 놓는다.
		//		=> 랜덤으로 하나 골라서 2장씩 만들것
		//		=> 벡터안에 카드를 섞어준다.
		//		=> 벡터 정보를 가지고 카드 리스트를 만들어 준다.
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_ROW * BOARD_COLUMN))
		{
			int mod = types.size() % 6;
			switch (mod)
			{
			case 0:
				types.push_back(Type::Bear);
				types.push_back(Type::Bear);
				break;

			case 2:
				types.push_back(Type::Wolf);
				types.push_back(Type::Wolf);
				break;

			case 4:
				types.push_back(Type::Dragon);
				types.push_back(Type::Dragon);
				break;
			}
		}

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(types.begin(), types.end(), g);

		int posX{ 15 }, posY{ 10 };
		int index{};
		for (int x = 0; x < BOARD_COLUMN; x++)
		{
			posY = 10;
			for (int y = 0; y < BOARD_ROW; y++)
			{
				mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
				posY += 150;
			}
			posX += 110;
		}
	}
}