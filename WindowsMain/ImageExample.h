#pragma once

#include "D2DFramework.h"

// #. BMP 파일 불러와서 그리기
//	=> BMP 파일 열기
//	=> BITMAPFILEHEADER 읽기
//	=> BITMAPINFOHEADER 일기
//	=> BITMAPFILEHEADER의 bfOffBits 만큼 건너 뛰기
//	=> 픽셀 데이터 읽기

class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;	// BMP파일을 읽어올 ID2D1Bitmap 인터페이스

public:
	virtual HRESULT Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768
	) override;
	void Render() override;

public:
	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};