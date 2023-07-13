#pragma once
#include <memory>
#include "D2DFramework.h"

// #1. 가상의 그래픽카드 해상도를 상수로 지정해 준다.
const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };

// #2. 색정보( ARGB )를 각각 어떠한 형태로 사용할지 지정해 준다.
// #2-1. 한 픽셀에 4바이트를 사용한다.
const int BITMAP_BYTECOUNT{ 4 };
// #. 한 픽셀의 바이트 수를 R, G, B, A로 구성할 예정이므로 4바이트로 정의한다.

class VirtualGraphics : public D2DFramework
{
private:
	// #3. FrameBuffer : Direct2D에서 그림을 그릴 수 있는 인터페이스 ID2D1Bitmap
	//		=> ID2D1Bitmap : BITMAP이미지를 처리할 수 있는 인터페이스
	//		=> ID2D1Bitmap은 COM오브젝트이기 때문에 스마트 포인터를 사용할 수 있다.
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap;

	// #4. BackBuffer : 메모리에 올려둘 이미지
	//		=> 하나의 픽셀을 4바이트( A, R, G, B )가 한 묶음이 된다.
	//		=> 이 때 구지 4바이트씩 묶을 필요 없이 처리하기 편하게끔 1바이트씩 동적 배열로 나열해도 된다.
	std::unique_ptr<UINT8[]> mspBackBuffer;

public:
	// #5. D2DFramework의 기능을 상속받아 사용할 기능들
	//		=> 초기화( Initialize )
	//		=> 그리기( Render )
	virtual HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Bitmap Example",
		UINT width = 1024,
		UINT height = 768) override;
	virtual void Render() override;
	// #5. 여기까지가 가상의 그래픽 카드를 만들고 BITMAP인터페이스에는 FrontBuffer를 만들고
	// #5. 메모리에다 BackBuffer를 만들어 놓은 상태이다.
	// #5. 이 상태에서 가상 그래픽 카드에 있는 FrontBuffer의 내용을 모니터에 그리는 기능을 작성한 것이다.

public:
	// #7. RAM에 있는 Byte배열( BackBuffer )를 D2DBitmap 가상 그래픽 카드로 옮기는 작업을 하는 함수
	void PresentBuffer();

	// #8. 후면 버퍼에 단색으로 색깔을 칠하는 함수
	void ClearBuffer(D2D1::ColorF color);

	// #9. 후면 버퍼에 점을 찍는 함수
	void DrawPixelToBuffer(int x, int y, D2D1::ColorF color);

	// #10. 후면 버퍼에 사각형을 그리는 함수
	void FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color);
};