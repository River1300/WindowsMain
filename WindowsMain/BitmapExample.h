#pragma once

#include <memory>
#include "D2DFramework.h"

// #. 가상의 비디오카드 FrameBuffer(1024 * 768 BITMAP)
// #. 화면 정보를 담고 있는 메모리(BackBuffer)
// #. 가상의 비디오 카드에 이미지 출력(BackBuffer -> FrameBuffer)
//		=> 시뮬레이션 - FrameBuffer(BITMAP)를 모니터에 그려서 확인

const int BITMAP_WIDTH = 1024;
const int BITMAP_HEIGHT = 768;
const int BITMAP_BYTECOUNT = 4;
//	=> 한 픽셀의 바이트 수로 RGGA를 구성할 예정이므로 4바이트로 정의

class BitmapExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap{};	// 가상 프레임 버퍼용 BITMAP 클래스
	std::unique_ptr<UINT8[]> mspBackBuffer;	// 후면 버퍼를 위한 배열, 한 픽셀은 4바이트가 한 묶음이 된다. [Red][Green][Blue][Alpha]
//	=> 배열의 크기는 '가로 * 세로 * 바이트 수'가 된다.

public:
	virtual HRESULT Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768
	) override;
	void Render() override;

	// #. 백버퍼를 단일 색상으로 지운다.
	void ClearBuffer(D2D1::ColorF color);

	// #. 후면버퍼를 프레임버퍼로 이동해주는 과정을 Flip, Present라 부른다.
	//		=. 직접 하드웨어 가속기능을 구현할 수는 없으니, 후면버퍼의 내용을 프레임버퍼에 복사하는 방식으로 간략화 해보자.
	void PresentBuffer();
};