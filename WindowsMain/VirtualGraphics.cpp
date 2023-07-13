#include "VirtualGraphics.h"

HRESULT VirtualGraphics::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	// #5-1. 기본적인 기능( 윈도우, 다이렉트x )을 부모로 부터 불러와 만들어 준다.
	D2DFramework::Initialize(hInstance, title, width, height);

	// #5-2. HEAP에 BackBuffer로 사용할 배열을 만들어 준다.
	mspBackBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);

	// #5-3. FrameBuffer를 만들어 준다.
	//		=> ID2D1Bitmap은 RenderTarget을 통해서 그려 준다.
	//		=> 다이렉트2D의 BITMAP인터페이스 만들기
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(						// #. PixelFormat이 어떤 형태인지 알려 준다.
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE)
		),
		mspFrameBitmap.ReleaseAndGetAddressOf()		// #. 일반 주소를 전달하는 것이 아닌 Comptr 형식의 주소를 전달해 준다.
	);

	ThrowIfFailed(hr);

	return hr;
}

void VirtualGraphics::Render()
{
	// #5-4. 그림의 시작과 끝을 만들어 준다.
	mspRenderTarget->BeginDraw();

	// #5-5. 그림을 한 번 지워 준다.
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	// #8-2. 후면 버퍼에 색을 칠한다.
	ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightPink));

	// #9-2. 후면 버퍼 100, 100좌표에 점을 찍어 준다.
	DrawPixelToBuffer(100, 100, D2D1::ColorF(D2D1::ColorF::Black));

	// #10-3. 후면 버퍼에 사각형을 그린다.
	FillRectToBuffer(10, 10, 100, 100, D2D1::ColorF::Blue);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(0, 1, 0, 0.5f));

	// #7-2. 그래픽 카드의 Flip이라는 기능을 대신하는 함수
	PresentBuffer();

	// #5-6. 렌더타겟에 비트맵을 그린다. 좌표는 생략된 기본 매개 변수 0, 0에 출력된다.
	//		=> 일단 멤버 변수로 선언한 비트맵만 전달해 준다.
	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void VirtualGraphics::PresentBuffer()
{
	// #7-1. BackBuffer의 내용을 FrameBuffer로 옮긴다.
	//		=> 이는 Byte Array메모리를 ID2D1Bitmap인터페이스로 옮기는 과정이다.
	mspFrameBitmap->CopyFromMemory(nullptr, &mspBackBuffer[0], BITMAP_WIDTH * BITMAP_BYTECOUNT);
	// #. < Pitch > : 메모리 공간에 올려진 데이터의 너비
	//		=> 가로( W ) X ByteCount = Pitch
}

void VirtualGraphics::ClearBuffer(D2D1::ColorF color)
{
	// #8-1. 첫번째 원소의 포인터를 받아와서 모눈종이를 하나씩 칠해 간다.
	auto pCurrentByte = &mspBackBuffer[0];
	for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; count++)
	{
		// #. 메모리 공간에 직접 접근하여 한 픽셀당 RGBA에 해당하는 색을 저장해 준다.
		//		=> ColorF는 0 ~ 1까지의 소수점으로 되어있는 rgba값이다.
		//		=> 따라서 255의 값을 곱하여 값을 맞추어 주어야 한다.
		*pCurrentByte = static_cast<UINT8>(color.r * 255);
		*(pCurrentByte + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrentByte + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrentByte + 3) = static_cast<UINT8>(color.a * 255);

		// #. 다음 픽셀로 넘어 간다.
		pCurrentByte += BITMAP_BYTECOUNT;
	}
}

void VirtualGraphics::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;

	// #9-1. x, y좌표를 지정해 준다.
	//		=> y는 pitch만큼 건너 뛰어진다.
	//		=> x는 ByteCount만큼 건너 뛰어진다.
	int index = y * pitch + x * BITMAP_BYTECOUNT;
	// #. 현재 하나의 픽셀은 4개의 바이트로 이루어진 상태다.
	// #. 즉 한 픽셀을 건너뛰는 x는 4개의 바이트만큼 건너 뛰는 거다.
	// #. 그리고 세로 층인 y는 가로 너비 만큼 건너 뛰는 거다.

	// #11. 알파 값을 적용해 보자.
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
	// #10-1. 가로 세로 좌표를 for문으로 DrawPixelToBuffer함수에 보내주면 알아서 찍어 준다.
	for (int x = left; x < left + width; x++)
	{
		for (int y = top; y < top + height; y++)
		{
			DrawPixelToBuffer(x, y, color);
		}
	}
}