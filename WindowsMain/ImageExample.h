#pragma once
#include "D2DFramework.h"

// #1. BMP 파일을 열기
// #2. BITMAPFILEHEADER 읽기
// #3. BITMAPINFOHEADER 읽기
// #4. 실제 픽셀 찾기
//		=> BITMAPFILEHEADER.bfOffBits 멤버( 파일 시작부터 픽셀까지의 Offset )
// #5. 픽셀 읽기

class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;
	// #. 그림을 그리기 위한 인터페이스를 포인터 멤버로 구성한다.
	//		=> 파일로부터 데이터를 읽고 mspBitmap에 올려 놓고 그림을 만들 예정

public:
	HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Image Example",
		UINT width = 1024,
		UINT height = 768) override;
	void Render() override;
	// #. 기본적인 프레임 워크의 기능 초기화와 그리기
	//		=> 비트맵을 추가적으로 그릴 예정이므로 오버라이딩 해준다.

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	// #. 그림파일을 불러오는 함수
	//		=> 파일을 읽어와서 mspBitmap에 그림을 그릴 예정
	//		=> 비트맵을 받아서 내부에서 인터페이스 메모리를 생성해 준다.
};