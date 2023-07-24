#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework{ pFramework }, mX{}, mY{}, mOpacity{ 1.0f }
{
	LoadWICImage(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename)
{
	this->mX = x;
	this->mY = y;
	this->mOpacity = opacity;
}

Actor::~Actor()
{
	mspBitmap.ReleaseAndGetAddressOf();
}

HRESULT Actor::LoadWICImage(LPCWSTR filename)
{
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr{};

	if (mpFramework == nullptr) return E_FAIL;

	auto pWICFactory{ mpFramework->GetWICFactory() };
	auto pRenderTarget = mpFramework->GetRenderTarget();
	if (pWICFactory == nullptr || pRenderTarget == nullptr) return E_FAIL;

	// 1. 디코더 생성
	hr = pWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. 프레임 획득
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	ThrowIfFailed(bitmapDecoder->GetFrame(0, frame.GetAddressOf()));

	// 3. 포멧 컨버터
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

	// 4. 변환된 데이터에서 비트맵 생성
	hr = mpFramework->GetRenderTarget()->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}

void Actor::Draw(float x, float y, float opacity)
{
	auto pRenderTarget = mpFramework->GetRenderTarget();

	if (pRenderTarget != nullptr)
	{
		auto size{ mspBitmap->GetPixelSize() };
		D2D1_RECT_F rect{
			x,y,
			static_cast<float>(x + size.width),
			static_cast<float>(y + size.height)
		};

		pRenderTarget->DrawBitmap(
			mspBitmap.Get(),
			rect,
			opacity
		);
	}
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}