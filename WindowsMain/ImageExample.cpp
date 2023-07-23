#include <fstream>
#include <vector>
#include "ImageExample.h"

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	LoadBMP(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());

	return S_OK;
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	mspRenderTarget->EndDraw();
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	BITMAPFILEHEADER bmh;
	BITMAPINFOHEADER bmi;

	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));
	if (bmh.bfType != 0x4D42) return E_FAIL;
	if (bmi.biBitCount != 32) return E_FAIL;

	std::vector<char> pPixels(bmi.biSizeImage);
	file.seekg(bmh.bfOffBits);
	//file.read(&pPixels[0], bmi.biSizeImage); �������� �̹����� pitch ������ �о �ùٸ� ��ġ(����)�� ����

	int pitch = bmi.biWidth * (bmi.biBitCount / 8);
	int index{};

	for (int y = bmi.biHeight - 1; y >= 0; y--)
	{
		index = y * pitch;
		for (int x = 0; x < bmi.biWidth; x++)
		{
			char r{}, g{}, b{}, a{};

			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (r == 30 || g == 199 || b == 250) r = g = b = a = 0;

			pPixels[index++] = b;
			pPixels[index++] = g;
			pPixels[index++] = r;
			pPixels[index++] = a;
		}
	}

	file.close();

	// ��� �ȼ��� �������� �о ������ ������ �����ϰ� ����� �ش�.
	//		=> �� ����Ʈ�� ��ü ��Ʈ���� �о� ���� �ڵ� �ۼ�
	//		=> �о�� ������ ������ ������ RGBA�� ��� 0���� �����.
	//		=> LoadBMP()�� ��Ʈ�� �������� ���� �Ӽ��� ������ �ش�.

	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(bmi.biWidth, bmi.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
			)
		),
		ppBitmap
	);
	ThrowIfFailed(hr);

	(*ppBitmap)->CopyFromMemory(
		nullptr,
		&pPixels[0],
		pitch
	);

	return S_OK;
}