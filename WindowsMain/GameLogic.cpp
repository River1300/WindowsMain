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

		// #13. ����� �ҷ��ͼ� mBackground�� �־� �ʱ�ȭ ���ش�.
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		// #14. 40���� ī�带 �����.
		CreateCards();
	}


	void GameLogic::Release()
	{
		// #15. ������� �̹����� ��� �����Ѵ�.
		mDeck.clear();
		mBackground.reset();
	}


	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		// #16. �޸� ������ �Ҵ�� �̹����� �����쿡 �׷��ش�. ��� ���� �׸��� �� ������ ī��
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		// #17. Ŭ������ ȭ�鿡 �׷��ش�.
		Gdiplus::PointF pos(895.0f, 20.0f);							// #. ��ǥ�� ���� �ְ�
		Gdiplus::Font font(L"���� ���", 20);						// #. ��Ʈ�� ���� �ְ�
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));		// #. �귯���� ���� �ְ�
		graphics.DrawString(L"Ŭ����: ", -1, &font, pos, &brush);	// #. ���� �׸���.

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);		// #. ���γʺ� �߾� ����
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);	// #. ���γʺ� �߾� ����
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, mCountRect, &format, &brush);
	}


	void GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};

		// #18. Ŭ�� �Ǿ��� �� �����ϰ� �ִ� 40���� ī�忡 Ŭ�� �Ǿ����� ����� �ڵ�
		for (auto& card : mDeck)
		{
			if (card.ChechCliked(x, y))
			{
				// #34. ���� ���õ� ī�带 ã���� for���� ���� ���´�.
				pCard = &card;
				break;
			}
		}
		if (pCard != nullptr)	// # ī�带 ���������� �����Ͽ��� ���
		{
			mFlipCount++;
			RECT rct{
				static_cast<LONG>(mCountRect.GetLeft()),
				static_cast<LONG>(mCountRect.GetTop()),
				static_cast<LONG>(mCountRect.GetRight()),
				static_cast<LONG>(mCountRect.GetBottom())
			};
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)		// #. ������ ������ ī�尡 ���� ���
			{
				mpSelectedCard = pCard;
			}
			else
			{
				if (mpSelectedCard == pCard)	// #. ������ ī�带 �ߺ��ؼ� �����Ͽ��� ���
				{
					mpSelectedCard == nullptr;
				}
				else
				{
					if (pCard->GetType() == mpSelectedCard->GetType())	// #. ���õ� �� ���� ī�尡 ���� Ÿ���� ���
					{
						// #36. ī�带 ����� ���� �����츦 ������ �ش�.
						pCard->Invalidate();
						mpSelectedCard->Invalidate();

						// #35. �����̳ʿ� �ִ� �˰����� Ȱ���Ͽ� ������ ī�带 �����.
						//		=> ���� ��ȣ�� ������( index )ī�带 ���� �� �ִ� ���ٽ� �Լ�
						mDeck.remove_if([&](Card& card)
							{	// #. mpSelecedCard�� ���ٽ� �ۿ� �ִ� �����̱� ������ ĸ������ �����Ͽ� �޴´�.
								return card.GetIndex() == mpSelectedCard->GetIndex();
							}
						);
						mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
						//		=> ī�� 40���� �ִ� �����̳�( mDeck )���� ������ index�� ���� Card�� 2�� �����.

						mpSelectedCard = nullptr;
					}
					else
					{
						//	#37. ī�尡 �ٸ��� �ٷ� �������� � ī������ Ȯ���� ��Ʊ� ������ ��� ���߾� �ش�.
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
		// #30. ���Ϳ� 40���� ī�带 ����� ���´�.
		//		=> �������� �ϳ� ��� 2�徿 �����
		//		=> ���;ȿ� ī�带 �����ش�.
		//		=> ���� ������ ������ ī�� ����Ʈ�� ����� �ش�.
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