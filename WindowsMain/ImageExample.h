#pragma once
#include <wincodec.h>	// #. WIC를 사용하기 위한 헤더파일 ( 코덱 : 인코딩, 디코딩을 위한 알고리즘 )
#include "D2DFramework.h"

class ImageExample : public D2DFramework
{
	// #. WIC를 초기화 하기 위해 ComPtr로 공장을 만든다.
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;

	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

public:
	HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Image Example",
		UINT width = 1024,
		UINT height = 768) override;
	void Release() override;	// WIC 이미지를 만들었다면 해제도 짝으로 해야한다.

	void Render() override;

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);	// WIC 이미지를 만들 함수
};