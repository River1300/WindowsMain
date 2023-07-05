#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#pragma comment (lib, "Gdiplus.lib")

// #4. ī���� ��ü�� �����.
namespace solitaire
{
	enum class Type
	{
		Wolf, Dragon, Bear
	};

	class Card
	{
// #4-1. �ϳ��� ī��� �ո�� �޸��� �ֱ� ������ ���� �ΰ��� �̹����� ������ �ִ�.
//		=> �׸� ������ ������ �� �ִ� ImageŬ������ �� �� ����Ѵ�.
//		=> Gdiplus::ImageŬ������ �⺻ �����ڰ� ����. ���� ������ ������ ���� �ϴ� �ΰ����� �ʱ�ȭ ���ش�.
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;
// #. Ŭ������ �Ϲ� ������ ��� �Ű� ���� ���� ��ü�� ���鶧 �⺻ �����ڰ� �Ҹ��鼭 �ڵ����� Ŭ���� ���� ����� �����ȴ�.
// #. �׷��� ������ ���� �� �̸� ���ؾ߸� �ϴ� ��찡 �ִ�.
// #. �̶� �����͸� ����ϴµ� �����ʹ� ���� �ش� Ŭ���� Ÿ���� �޸� ������ ����ų �� ���� ���̴�.
// #. ���� �������� �޸� ������ �Ҵ�� �����ϱ� ������ ���ϴ� ���� ���� �� ���Ű� �����ϴ�.
// #. GDI+�� GdiplusStartup���� �����ϰ� GdiplusShutdown���� ������.
// #. �׷��� �����ͷ� �����Ҵ��� �� �ʿ䰡 �ִ�.

// #25. ������ �ڵ��� ����� �����Ѵ�.
		HWND mHwnd;

// #32. ī�� ���� ��ȣ�� ������ �ش�.
		int mIndex;

// #4-2. ī���� ���¿� ���� ������ �Ѵ�.
		int mX, mY;
		bool mIsFront;
		Type mType;

	public:
// #26. ī�带 ���鶧 ������ �ڵ��� �߰��Ѵ�.
		Card(HWND hwnd, int index, Type type, int x, int y);			// #. ī���� Ÿ�԰� ��ǥ�� �ʱ�ȭ ���ִ� ������

		bool ChechCliked(int x, int y);			// #. ī�带 Ŭ�� �ߴ��� üũ�ϴ� �Լ�
		void Flip(bool isFront);				// #. ī�带 ������ �ִ� �Լ�
		void Draw(Gdiplus::Graphics& graphics); // #. ī�带 �׸��� �Լ�

// #24. InvalidateRect() : �簢�� ������ ��ȿȭ �ϴ� �Լ�
//		=> �����찡 ��ȿȭ�� ������ �ٽ� �׸��� �Ѵ�.
//		=> HWND : �ٽ� �׸� ������ �ڵ�
//		=> const RECT * : �ٽ� �׸� ������ ����
//		=> BOOL : ����� ������̳� �ƴϳ�
		void Invalidate();

// #31. ī���� Ÿ���� �˱����� �Լ�
		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}