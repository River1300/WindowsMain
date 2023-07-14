#include <fstream>
#include <vector>
#include "ImageExample.h"

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);	// �θ� Ŭ������ �ʱ�ȭ �Լ� ȣ��

	LoadBMP(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());	// �׸��� �ҷ���

	return S_OK;
}


void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();	// �׸��� ����

	mspRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f)));	// �׸��� ��� ��ĥ
	mspRenderTarget->DrawBitmap(mspBitmap.Get());	// mspBitmap�� �ִ� �׸��� ȭ�鿡 �׸���

	mspRenderTarget->EndDraw();	// �׸��� ��
}


HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// #1-1. ������ �о�� ��ü( file )�� ����� binary��Ÿ�Ϸ� ������ �о� �´�.
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// #2-1. POD������ ����ü ��ü bmh�� ����� ��°�� ���� �����͸� �о���� ��� ����ü�� ��ȯ�Ѵ�.
	BITMAPFILEHEADER bmh;
	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));

	// #3-1. POD������ ����ü ��ü bmi�� ����� ��°�� ���� �����͸� �о���� ��� ����ü�� ��ȯ�Ѵ�.
	BITMAPINFOHEADER bmi;
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));

	// #4-1. ��Ʈ�� ��� ��ü�� ����� Offset��ŭ �ǳ� �ڴ�.
	file.seekg(bmh.bfOffBits);

	// #5-1. �ȼ��� �о� ���� ���� �޸𸮿� �ȼ� �����͸� �����ؾ� �Ѵ�.
	//		=> �޸� ������ �غ��ϰ� �غ�� ������ biSizeImage���� ������ �о�´�.
	std::vector<char> pixels(bmi.biSizeImage);
	//file.read(&pixels[0], bmi.biSizeImage);

	int pitch{ bmi.biWidth * (bmi.biBitCount / 8) };

	// #7. �������� �׸������� �Ųٷ� �о� �ͼ� �������� �׸����� ������ش�.
		//for (int y = bmi.biHeight - 1; y >= 0; y--)
		//{
		//	file.read(&pixels[y * pitch], pitch);
		//}

	// #8. �̹����� ������ ����� �������� ����� �ش�.
	//		=> ����( RGB : 30, 199, 250 )
	//		=> �� �ȼ� �� �о -> ������ ��
	//		=> ������ ������ ���� - RGBA 0000
	//		=> ������ �ٸ��� �״�� ���
	int index{};
	for (int y = bmi.biHeight - 1; y >= 0; y--)
	{
		index = y * pitch;
		for (int x = 0; x < bmi.biWidth; x++)
		{
			unsigned char r{}, g{}, b{}, a{};
			file.read(reinterpret_cast<char*>(&b), 1);
			file.read(reinterpret_cast<char*>(&g), 1);
			file.read(reinterpret_cast<char*>(&r), 1);
			file.read(reinterpret_cast<char*>(&a), 1);

			if (r == 30 && g == 199 && b == 250)
			{
				r = g = b = a = 0;
			}
			pixels[index++] = b;
			pixels[index++] = g;
			pixels[index++] = r;
			pixels[index++] = a;
		}
	}


	file.close();

	// #6. ��Ʈ���� ����鼭 �׸��� ������ �����´�.
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(bmi.biWidth, bmi.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		),
		ppBitmap
	);

	ThrowIfFailed(hr);

	(*ppBitmap)->CopyFromMemory(nullptr, &pixels[0], pitch);

	return S_OK;
}