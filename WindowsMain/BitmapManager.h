#pragma once
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <map>
#include <string>

class BitmapManager final
{
// #. 싱글턴 패턴 자체는 스테틱 멤버함수나 스테틱 변수로 이루어져 있기 때문에 상속 구조로 만들면 예기치 못한 문제가 발생할 수 있다.
//		=> 따라서 오직 자기 자신( Alone )혼자 만 존재해야만 한다.
public:
	static BitmapManager& Instance()
	{	// #. 딱 한 번만 만들어지는 매니저 변수
		static BitmapManager instance;
		return instance;
	}

private:
// #. 외부에서 인스턴스 생성을 맊기 위해 기본 생성자 + 복사 생성자 + 복사 대입 생성자를 모두 숨긴다.
	BitmapManager() {}
	BitmapManager(const BitmapManager&) {}
	void operator=(const BitmapManager&) {}

public:
	~BitmapManager() {}

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources{};

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();
	ID2D1Bitmap* LoadBitmap(std::wstring filename);
};