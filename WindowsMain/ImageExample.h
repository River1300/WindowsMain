#pragma once

#include "D2DFramework.h"

// #. BMP ���� �ҷ��ͼ� �׸���
//	=> BMP ���� ����
//	=> BITMAPFILEHEADER �б�
//	=> BITMAPINFOHEADER �ϱ�
//	=> BITMAPFILEHEADER�� bfOffBits ��ŭ �ǳ� �ٱ�
//	=> �ȼ� ������ �б�

class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;	// BMP������ �о�� ID2D1Bitmap �������̽�

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