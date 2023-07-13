#include "VirtualGraphics.h"

HRESULT VirtualGraphics::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	// #5-1. �⺻���� ���( ������, ���̷�Ʈx )�� �θ�� ���� �ҷ��� ����� �ش�.
	D2DFramework::Initialize(hInstance, title, width, height);

	// #5-2. HEAP�� BackBuffer�� ����� �迭�� ����� �ش�.
	mspBackBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);

	// #5-3. FrameBuffer�� ����� �ش�.
	//		=> ID2D1Bitmap�� RenderTarget�� ���ؼ� �׷� �ش�.
	//		=> ���̷�Ʈ2D�� BITMAP�������̽� �����
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(						// #. PixelFormat�� � �������� �˷� �ش�.
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE)
		),
		mspFrameBitmap.ReleaseAndGetAddressOf()		// #. �Ϲ� �ּҸ� �����ϴ� ���� �ƴ� Comptr ������ �ּҸ� ������ �ش�.
	);

	ThrowIfFailed(hr);

	return hr;
}

void VirtualGraphics::Render()
{
	// #5-4. �׸��� ���۰� ���� ����� �ش�.
	mspRenderTarget->BeginDraw();

	// #5-5. �׸��� �� �� ���� �ش�.
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	// #8-2. �ĸ� ���ۿ� ���� ĥ�Ѵ�.
	ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightPink));

	// #9-2. �ĸ� ���� 100, 100��ǥ�� ���� ��� �ش�.
	DrawPixelToBuffer(100, 100, D2D1::ColorF(D2D1::ColorF::Black));

	// #10-3. �ĸ� ���ۿ� �簢���� �׸���.
	FillRectToBuffer(10, 10, 100, 100, D2D1::ColorF::Blue);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(0, 1, 0, 0.5f));

	// #7-2. �׷��� ī���� Flip�̶�� ����� ����ϴ� �Լ�
	PresentBuffer();

	// #5-6. ����Ÿ�ٿ� ��Ʈ���� �׸���. ��ǥ�� ������ �⺻ �Ű� ���� 0, 0�� ��µȴ�.
	//		=> �ϴ� ��� ������ ������ ��Ʈ�ʸ� ������ �ش�.
	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void VirtualGraphics::PresentBuffer()
{
	// #7-1. BackBuffer�� ������ FrameBuffer�� �ű��.
	//		=> �̴� Byte Array�޸𸮸� ID2D1Bitmap�������̽��� �ű�� �����̴�.
	mspFrameBitmap->CopyFromMemory(nullptr, &mspBackBuffer[0], BITMAP_WIDTH * BITMAP_BYTECOUNT);
	// #. < Pitch > : �޸� ������ �÷��� �������� �ʺ�
	//		=> ����( W ) X ByteCount = Pitch
}

void VirtualGraphics::ClearBuffer(D2D1::ColorF color)
{
	// #8-1. ù��° ������ �����͸� �޾ƿͼ� �����̸� �ϳ��� ĥ�� ����.
	auto pCurrentByte = &mspBackBuffer[0];
	for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; count++)
	{
		// #. �޸� ������ ���� �����Ͽ� �� �ȼ��� RGBA�� �ش��ϴ� ���� ������ �ش�.
		//		=> ColorF�� 0 ~ 1������ �Ҽ������� �Ǿ��ִ� rgba���̴�.
		//		=> ���� 255�� ���� ���Ͽ� ���� ���߾� �־�� �Ѵ�.
		*pCurrentByte = static_cast<UINT8>(color.r * 255);
		*(pCurrentByte + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrentByte + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrentByte + 3) = static_cast<UINT8>(color.a * 255);

		// #. ���� �ȼ��� �Ѿ� ����.
		pCurrentByte += BITMAP_BYTECOUNT;
	}
}

void VirtualGraphics::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;

	// #9-1. x, y��ǥ�� ������ �ش�.
	//		=> y�� pitch��ŭ �ǳ� �پ�����.
	//		=> x�� ByteCount��ŭ �ǳ� �پ�����.
	int index = y * pitch + x * BITMAP_BYTECOUNT;
	// #. ���� �ϳ��� �ȼ��� 4���� ����Ʈ�� �̷���� ���´�.
	// #. �� �� �ȼ��� �ǳʶٴ� x�� 4���� ����Ʈ��ŭ �ǳ� �ٴ� �Ŵ�.
	// #. �׸��� ���� ���� y�� ���� �ʺ� ��ŭ �ǳ� �ٴ� �Ŵ�.

	// #11. ���� ���� ������ ����.
	float inverse = 1.0f - color.a;
	UINT red2 = static_cast<UINT8>(color.r * 255);
	UINT green2 = static_cast<UINT8>(color.g * 255);
	UINT blue2 = static_cast<UINT8>(color.b * 255);

	mspBackBuffer[index] = static_cast<UINT8>(mspBackBuffer[index] * inverse + red2 * color.a);
	mspBackBuffer[index + 1] = static_cast<UINT8>(mspBackBuffer[index + 1] * inverse + green2 * color.a);
	mspBackBuffer[index + 2] = static_cast<UINT8>(mspBackBuffer[index + 2] * inverse + blue2 * color.a);
	mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);
}

void VirtualGraphics::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	// #10-1. ���� ���� ��ǥ�� for������ DrawPixelToBuffer�Լ��� �����ָ� �˾Ƽ� ��� �ش�.
	for (int x = left; x < left + width; x++)
	{
		for (int y = top; y < top + height; y++)
		{
			DrawPixelToBuffer(x, y, color);
		}
	}
}