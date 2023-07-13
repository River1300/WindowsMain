#pragma once
#include <memory>
#include "D2DFramework.h"

// #1. ������ �׷���ī�� �ػ󵵸� ����� ������ �ش�.
const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };

// #2. ������( ARGB )�� ���� ��� ���·� ������� ������ �ش�.
// #2-1. �� �ȼ��� 4����Ʈ�� ����Ѵ�.
const int BITMAP_BYTECOUNT{ 4 };
// #. �� �ȼ��� ����Ʈ ���� R, G, B, A�� ������ �����̹Ƿ� 4����Ʈ�� �����Ѵ�.

class VirtualGraphics : public D2DFramework
{
private:
	// #3. FrameBuffer : Direct2D���� �׸��� �׸� �� �ִ� �������̽� ID2D1Bitmap
	//		=> ID2D1Bitmap : BITMAP�̹����� ó���� �� �ִ� �������̽�
	//		=> ID2D1Bitmap�� COM������Ʈ�̱� ������ ����Ʈ �����͸� ����� �� �ִ�.
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap;

	// #4. BackBuffer : �޸𸮿� �÷��� �̹���
	//		=> �ϳ��� �ȼ��� 4����Ʈ( A, R, G, B )�� �� ������ �ȴ�.
	//		=> �� �� ���� 4����Ʈ�� ���� �ʿ� ���� ó���ϱ� ���ϰԲ� 1����Ʈ�� ���� �迭�� �����ص� �ȴ�.
	std::unique_ptr<UINT8[]> mspBackBuffer;

public:
	// #5. D2DFramework�� ����� ��ӹ޾� ����� ��ɵ�
	//		=> �ʱ�ȭ( Initialize )
	//		=> �׸���( Render )
	virtual HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Bitmap Example",
		UINT width = 1024,
		UINT height = 768) override;
	virtual void Render() override;
	// #5. ��������� ������ �׷��� ī�带 ����� BITMAP�������̽����� FrontBuffer�� �����
	// #5. �޸𸮿��� BackBuffer�� ����� ���� �����̴�.
	// #5. �� ���¿��� ���� �׷��� ī�忡 �ִ� FrontBuffer�� ������ ����Ϳ� �׸��� ����� �ۼ��� ���̴�.

public:
	// #7. RAM�� �ִ� Byte�迭( BackBuffer )�� D2DBitmap ���� �׷��� ī��� �ű�� �۾��� �ϴ� �Լ�
	void PresentBuffer();

	// #8. �ĸ� ���ۿ� �ܻ����� ������ ĥ�ϴ� �Լ�
	void ClearBuffer(D2D1::ColorF color);

	// #9. �ĸ� ���ۿ� ���� ��� �Լ�
	void DrawPixelToBuffer(int x, int y, D2D1::ColorF color);

	// #10. �ĸ� ���ۿ� �簢���� �׸��� �Լ�
	void FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color);
};