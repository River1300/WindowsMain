#pragma once

#include <wincodec.h>
#include "D2DFramework.h"

// #. Windows Imaging Component

class ImageExample : public D2DFramework
{	// WIC���� Factory �����̶� Factory�� ���� ����Ʈ �����͸� ����� �����ϰ� �������� ������ �ʿ��ϹǷ� Release�� �������̵� �Ѵ�.
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