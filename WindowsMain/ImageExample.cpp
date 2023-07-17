#include <fstream>
#include <vector>
#include "ImageExample.h"

#pragma comment( lib, "WindowsCodecs.lib" )	// ���̺귯�� �߰�

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;

	hr = CoInitialize(nullptr);	// #. ������Ʈ ������Ʈ�� �ʱ�ȭ�ϱ� ���� �ҷ��� �ϴ� �Լ�
	ThrowIfFailed(hr);

	// #2. WIC ���� �����
	hr = CoCreateInstance(
		CLSID_WICImagingFactory, // GUID : Globally Unique ID
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);

	D2DFramework::Initialize(hInstance, title, width, height);

	//LoadBMP(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());
	// #1. WICImage�� ����Ͽ� �̹��� �ʱ�ȭ
	LoadWICImage(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());
	
	return S_OK;
}

void ImageExample::Release()
{
	D2DFramework::Release();

	mspBitmap.Reset();
	mspWICFactory.Reset();

	CoUninitialize();
}


void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();

	mspRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f)));
	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	mspRenderTarget->EndDraw();
}


HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	BITMAPFILEHEADER bmh;
	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bmi;
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));

	file.seekg(bmh.bfOffBits);

	std::vector<char> pixels(bmi.biSizeImage);

	int pitch{ bmi.biWidth * (bmi.biBitCount / 8) };

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

HRESULT ImageExample::LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. ���ڴ� ����
	//		=> Ư���� �̹��� ���Ͽ��� ��ȣȭ�� �׸��� �����´�.
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf());
	ThrowIfFailed(hr);

	// 2. ���ڴ����� ������(frame) ȹ��
	//		=> ù ��° ������ ��������
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = (bitmapDecoder->GetFrame(0, frame.GetAddressOf()));
	ThrowIfFailed(hr);

	// 3. �����͸� ����ؼ� �����͸� ��ȯ
	//		=> �׸� �׸��� ID2D1Bitmap�� �°� �ȼ� ������ ��ȯ�� �ش�.
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(mspWICFactory->CreateFormatConverter(converter.GetAddressOf()));

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0, // Threshold : �Ӱ���
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// 4. ��ȯ�� �����ͷ� BITMAP ����
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
	// #. PNG ������ ����Ѵٰ� ���� �ϸ� WIC���� �ʿ��� ���� ó������ decoder�� �ʿ��ϴ�.
	//		=> PNG ������ �� �� �ֵ��� PNG �ڵ��� ����Ͽ� Ÿ���� ��� �̹���ȭ �Ѵ�.
	//		=> decode : �ڵ�Ǯ��, ��ȣȭ
	//		=> encode : �ڵ�ȭ, ��ȣȭ
}
