#pragma once
#include <wincodec.h>	// #. WIC�� ����ϱ� ���� ������� ( �ڵ� : ���ڵ�, ���ڵ��� ���� �˰��� )
#include "D2DFramework.h"

class ImageExample : public D2DFramework
{
	// #. WIC�� �ʱ�ȭ �ϱ� ���� ComPtr�� ������ �����.
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;

	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

public:
	HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Image Example",
		UINT width = 1024,
		UINT height = 768) override;
	void Release() override;	// WIC �̹����� ������ٸ� ������ ¦���� �ؾ��Ѵ�.

	void Render() override;

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);	// WIC �̹����� ���� �Լ�
};