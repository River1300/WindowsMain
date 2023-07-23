#pragma once

#include <memory>
#include "D2DFramework.h"

// #. ������ ����ī�� FrameBuffer(1024 * 768 BITMAP)
// #. ȭ�� ������ ��� �ִ� �޸�(BackBuffer)
// #. ������ ���� ī�忡 �̹��� ���(BackBuffer -> FrameBuffer)
//		=> �ùķ��̼� - FrameBuffer(BITMAP)�� ����Ϳ� �׷��� Ȯ��

const int BITMAP_WIDTH = 1024;
const int BITMAP_HEIGHT = 768;
const int BITMAP_BYTECOUNT = 4;
//	=> �� �ȼ��� ����Ʈ ���� RGGA�� ������ �����̹Ƿ� 4����Ʈ�� ����

class BitmapExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap{};	// ���� ������ ���ۿ� BITMAP Ŭ����
	std::unique_ptr<UINT8[]> mspBackBuffer;	// �ĸ� ���۸� ���� �迭, �� �ȼ��� 4����Ʈ�� �� ������ �ȴ�. [Red][Green][Blue][Alpha]
//	=> �迭�� ũ��� '���� * ���� * ����Ʈ ��'�� �ȴ�.

public:
	virtual HRESULT Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768
	) override;
	void Render() override;

	// #. ����۸� ���� �������� �����.
	void ClearBuffer(D2D1::ColorF color);

	// #. �ĸ���۸� �����ӹ��۷� �̵����ִ� ������ Flip, Present�� �θ���.
	//		=. ���� �ϵ���� ���ӱ���� ������ ���� ������, �ĸ������ ������ �����ӹ��ۿ� �����ϴ� ������� ����ȭ �غ���.
	void PresentBuffer();
};