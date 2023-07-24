#pragma once

#include <wincodec.h>
#include "D2DFramework.h"

// #. Windows Imaging Component

class ImageExample : public D2DFramework
{	// WIC역시 Factory 패턴이라 Factory를 위한 스마트 포인터를 멤버로 선언하고 개별적인 해제가 필요하므로 Release를 오버라이딩 한다.
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

public:
	virtual HRESULT Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768
	) override;
	void Render() override;

public:
	void Release() override;

public:
	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};