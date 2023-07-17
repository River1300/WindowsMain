#include <fstream>
#include <vector>
#include "ImageExample.h"

#pragma comment( lib, "WindowsCodecs.lib" )	// 라이브러리 추가

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;

	hr = CoInitialize(nullptr);	// #. 컴포넌트 오브젝트를 초기화하기 위해 불러야 하는 함수
	ThrowIfFailed(hr);

	// #2. WIC 공장 만들기
	hr = CoCreateInstance(
		CLSID_WICImagingFactory, // GUID : Globally Unique ID
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);

	D2DFramework::Initialize(hInstance, title, width, height);

	//LoadBMP(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());
	// #1. WICImage를 사용하여 이미지 초기화
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
	// 1. 디코더 생성
	//		=> 특정한 이미지 파일에서 암호화된 그림을 꺼내온다.
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf());
	ThrowIfFailed(hr);

	// 2. 디코더에서 프레임(frame) 획득
	//		=> 첫 번째 프레임 가져오기
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = (bitmapDecoder->GetFrame(0, frame.GetAddressOf()));
	ThrowIfFailed(hr);

	// 3. 컨버터를 사용해서 데이터를 변환
	//		=> 그린 그림이 ID2D1Bitmap에 맞게 픽셀 포멧을 변환해 준다.
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(mspWICFactory->CreateFormatConverter(converter.GetAddressOf()));

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0, // Threshold : 임계점
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// 4. 변환된 데이터로 BITMAP 생성
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
	// #. PNG 파일을 사용한다고 가정 하면 WIC에서 필요한 것은 처음으로 decoder가 필요하다.
	//		=> PNG 파일을 열 수 있도록 PNG 코덱을 사용하여 타입을 열어서 이미지화 한다.
	//		=> decode : 코드풀이, 복호화
	//		=> encode : 코드화, 암호화
}
