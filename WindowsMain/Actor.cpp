#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework{ pFramework },
	mX{}, mY{}, mOpacity{ 1.0f }
{
	LoadWICImage(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename)
{
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor()
{
	mspBitmap.Reset();
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}

HRESULT Actor::LoadWICImage(LPCWSTR filename)
{
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	if (mpFramework == nullptr) return E_FAIL;

	auto WICFactory = mpFramework->GetWICFactory();
	auto renderTarget = mpFramework->GetRenderTarget();

	if (WICFactory == nullptr || renderTarget == nullptr) return E_FAIL;

	hr = WICFactory->CreateDecoderFromFilename(
		filename,
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
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}

void Actor::Draw(float x, float y, float opacity)
{
	auto renderTarget = mpFramework->GetRenderTarget();

	if (renderTarget != nullptr)
	{
		auto size{ mspBitmap->GetPixelSize() };
		D2D1_RECT_F rect{ x,y,
			static_cast<float>(x + size.width), 
			static_cast<float>(y + size.height) };

		renderTarget->DrawBitmap(
			mspBitmap.Get(),
			rect,
			opacity
		);
	}
}