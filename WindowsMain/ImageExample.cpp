#include <fstream>
#include <vector>
#include "ImageExample.h"

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);	// 부모 클래스의 초기화 함수 호출

	LoadBMP(L"Data/32.bmp", mspBitmap.ReleaseAndGetAddressOf());	// 그림을 불러옴

	return S_OK;
}


void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();	// 그리기 시작

	mspRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f)));	// 그리기 배경 색칠
	mspRenderTarget->DrawBitmap(mspBitmap.Get());	// mspBitmap에 있는 그림을 화면에 그리기

	mspRenderTarget->EndDraw();	// 그리기 끝
}


HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// #1-1. 파일을 읽어올 객체( file )을 만들고 binary스타일로 파일을 읽어 온다.
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// #2-1. POD형태의 구조체 객체 bmh를 만들고 통째로 파일 데이터를 읽어오고 즉시 구조체로 변환한다.
	BITMAPFILEHEADER bmh;
	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));

	// #3-1. POD형태의 구조체 객체 bmi를 만들고 통째로 파일 데이터를 읽어오고 즉시 구조체로 변환한다.
	BITMAPINFOHEADER bmi;
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));

	// #4-1. 비트맵 헤더 객체의 멤버로 Offset만큼 건너 뛴다.
	file.seekg(bmh.bfOffBits);

	// #5-1. 픽셀을 읽어 오기 위해 메모리에 픽셀 데이터를 저장해야 한다.
	//		=> 메모리 공간을 준비하고 준비된 공간에 biSizeImage까지 파일을 읽어온다.
	std::vector<char> pixels(bmi.biSizeImage);
	//file.read(&pixels[0], bmi.biSizeImage);

	int pitch{ bmi.biWidth * (bmi.biBitCount / 8) };

	// #7. 뒤집어진 그림파일을 거꾸로 읽어 와서 정상적인 그림으로 출력해준다.
		//for (int y = bmi.biHeight - 1; y >= 0; y--)
		//{
		//	file.read(&pixels[y * pitch], pitch);
		//}

	// #8. 이미지의 배경색을 지우고 나머지만 출력해 준다.
	//		=> 배경색( RGB : 30, 199, 250 )
	//		=> 한 픽셀 씩 읽어서 -> 배경색과 비교
	//		=> 배경색과 같으면 투명 - RGBA 0000
	//		=> 배경색과 다르면 그대로 사용
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

	// #6. 비트맵을 만들면서 그림의 원본을 가져온다.
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