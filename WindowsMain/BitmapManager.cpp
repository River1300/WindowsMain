#include "BitmapManager.h"
#include "ComException.h"

#pragma comment ( lib, "WindowsCodecs.lib" )

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr{};

	auto pWICFactory = mspWICFactory.Get();
	auto pRenderTarget = mpRenderTarget;
	if (pWICFactory == nullptr || pRenderTarget == nullptr) return E_FAIL;

	hr = pWICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	ThrowIfFailed(bitmapDecoder->GetFrame(0, frame.GetAddressOf()));

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(
		pWICFactory->CreateFormatConverter(converter.GetAddressOf())
	);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr) return E_FAIL;

	mpRenderTarget = pRT;

	HRESULT hr = ::CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);

	return hr;
}

void BitmapManager::Release()
{	// 로딩된 비트맵 이미지들을 해제
	for (auto& bitmap : mBitmapResources) bitmap.second.Reset();

	mBitmapResources.clear();
	mspWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename)
{
	if (mspWICFactory == nullptr) return nullptr;

	ID2D1Bitmap* pBitmap{};
	// map은 고유한 키를 가지도록 만들기 위해 insert할때 해당 키로 탐색을 수행한다.
	//		=> ID2D1Bitmap 타입에 들어간 second값이 이미 있다면 넣을 필요도 없다.
	//		=> 처음에 insert를 할 때에는 아무것도 넣지 않는다.
	auto result = mBitmapResources.insert(
		pair<wstring, ComPtr<ID2D1Bitmap>>(filename, nullptr)
	);

	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());
		result.first->second = spBitmap;
	}

	pBitmap = result.first->second.Get();

	return pBitmap;
}