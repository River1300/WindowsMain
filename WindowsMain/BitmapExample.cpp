#include "BitmapExample.h"

HRESULT BitmapExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);	// 기반 클래스의 가상함수를 호출해서 기본 초기화를 수행

	mspBackBuffer = std::make_unique<UINT8[]>(
		BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT
		);	// 힙에 후면버퍼로 사용할 배열을 생성, 원소는 UINT8 이며, 크기는 가로 * 세로 * 픽셀

	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			)
		),
		mspFrameBitmap.ReleaseAndGetAddressOf()
	);	// 렌더타겟에 비트맵을 그린다. 좌표는 생략된 기본 매개변수

	ThrowIfFailed(hr);

	return S_OK;
}

void BitmapExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightPink));
	PresentBuffer();

	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapExample::ClearBuffer(D2D1::ColorF color)
{
	UINT8* pCurrent = &mspBackBuffer[0];
	for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; count++)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

		pCurrent += BITMAP_BYTECOUNT;
	}
}

void BitmapExample::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(
		nullptr,
		&mspBackBuffer[0],
		BITMAP_WIDTH * BITMAP_BYTECOUNT
	);	// BITMAP리소스에 메모리 버퍼의 내용을 복사
}