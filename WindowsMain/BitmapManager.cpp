#include "BitmapManager.h"
#include "ComException.h"

#pragma comment(lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	auto WICFactory = mspWICFactory.Get();
	auto renderTarget = mpRenderTarget;

	if (WICFactory == nullptr || renderTarget == nullptr) return E_FAIL;

	hr = WICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf());
	ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = (bitmapDecoder->GetFrame(0, frame.GetAddressOf()));
	ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(WICFactory->CreateFormatConverter(converter.GetAddressOf()));

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	hr = renderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr)
	{	// #. 랜더 타겟이 없으면 매니저는 정상적인 작동 불가
		return E_FAIL;
	}	//		=> WIC로 부터 Bitmap을 만드는 것이 매니저의 목적인데 랜더 타겟이 없다?

	mpRenderTarget = pRT;

	HRESULT hr;

	// #. WIC 공장 만들기
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf()));
	ThrowIfFailed(hr);

	return E_NOTIMPL;
}

void BitmapManager::Release()
{
	for (auto& e : mBitmapResources)
	{	// #. map으로 이루어진 리소스의 key(ComPtr)를 모두 해제한다.
		e.second.Reset();
	}
	mBitmapResources.clear();
	mspWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename)
{
	if (mspWICFactory == nullptr)
	{
		return nullptr;
	}

	// #. filename으로 이미 로딩이 되어 있다면 반환하면 되고 처음이라면 로딩해주면 된다.
	//		=> insert(), find() 서로 중복되는 작업을 실행한다.
	//		=> insert()를 할 때 유니크를 위해 find를 실행한다.
	//		=> insert()의 결과가 key가 있는지 없는지를 알려주기도 한다.	std::pair<iterator( 추가된 위치 ), bool( 추가 성공 여부 )>
	//auto iter = mBitmapResources.find(filename);

	// std::map insert 오버헤드 제거
	auto result = mBitmapResources.insert({ filename, nullptr });
	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());
		result.first->second = spBitmap;
	}

	// #. 반환된 pair의 이터레이터(first)는 즉 Key + Value를 가지고 있는 원소이고 이 원소의 Bitmap(second)를 일반 포인터 형식으로 반환해 준다.
	return result.first->second.Get();

}
