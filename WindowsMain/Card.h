#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#pragma comment (lib, "Gdiplus.lib")

// #4. 카드라는 객체를 만든다.
namespace solitaire
{
	enum class Type
	{
		Wolf, Dragon, Bear
	};

	class Card
	{
// #4-1. 하나의 카드는 앞면과 뒷면이 있기 때문에 각각 두가지 이미지를 가지고 있다.
//		=> 그림 정보를 가져올 수 있는 Image클래스를 두 개 등록한다.
//		=> Gdiplus::Image클래스는 기본 생성자가 없다. 따라서 포인터 변수로 만들어서 일단 널값으로 초기화 해준다.
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;
// #. 클래스의 일반 변수의 경우 매개 변수 없는 객체를 만들때 기본 생성자가 불리면서 자동으로 클래스 내부 멤버가 구성된다.
// #. 그러나 게임을 만들 때 이를 피해야만 하는 경우가 있다.
// #. 이때 포인터를 사용하는데 포인터는 그저 해당 클래스 타입의 메모리 공간을 가리킬 수 있을 뿐이다.
// #. 또한 동적으로 메모리 공간을 할당및 해제하기 때문에 원하는 때에 생성 및 제거가 가능하다.
// #. GDI+는 GdiplusStartup에서 시작하고 GdiplusShutdown에서 끝난다.
// #. 그래서 포인터로 동적할당을 할 필요가 있다.

// #25. 윈도우 핸들을 멤버로 구성한다.
		HWND mHwnd;

// #32. 카드 고유 번호를 지정해 준다.
		int mIndex;

// #4-2. 카드의 상태에 대한 구성을 한다.
		int mX, mY;
		bool mIsFront;
		Type mType;

	public:
// #26. 카드를 만들때 윈도우 핸들을 추가한다.
		Card(HWND hwnd, int index, Type type, int x, int y);			// #. 카드의 타입과 좌표를 초기화 해주는 생성자

		bool ChechCliked(int x, int y);			// #. 카드를 클릿 했는지 체크하는 함수
		void Flip(bool isFront);				// #. 카드를 뒤집어 주는 함수
		void Draw(Gdiplus::Graphics& graphics); // #. 카드를 그리는 함수

// #24. InvalidateRect() : 사각형 영역을 무효화 하는 함수
//		=> 윈도우가 무효화된 영역을 다시 그리게 한다.
//		=> HWND : 다시 그릴 윈도우 핸들
//		=> const RECT * : 다시 그릴 영역의 정보
//		=> BOOL : 배경을 지울것이냐 아니냐
		void Invalidate();

// #31. 카드의 타입을 알기위한 함수
		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}