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
	{	// #. ���� Ÿ���� ������ �Ŵ����� �������� �۵� �Ұ�
		return E_FAIL;
	}	//		=> WIC�� ���� Bitmap�� ����� ���� �Ŵ����� �����ε� ���� Ÿ���� ����?

	mpRenderTarget = pRT;

	HRESULT hr;

	// #. WIC ���� �����
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf()));
	ThrowIfFailed(hr);

	return E_NOTIMPL;
}

void BitmapManager::Release()
{
	for (auto& e : mBitmapResources)
	{	// #. map���� �̷���� ���ҽ��� key(ComPtr)�� ��� �����Ѵ�.
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

	// #. filename���� �̹� �ε��� �Ǿ� �ִٸ� ��ȯ�ϸ� �ǰ� ó���̶�� �ε����ָ� �ȴ�.
	//		=> insert(), find() ���� �ߺ��Ǵ� �۾��� �����Ѵ�.
	//		=> insert()�� �� �� ����ũ�� ���� find�� �����Ѵ�.
	//		=> insert()�� ����� key�� �ִ��� �������� �˷��ֱ⵵ �Ѵ�.	std::pair<iterator( �߰��� ��ġ ), bool( �߰� ���� ���� )>
	//auto iter = mBitmapResources.find(filename);

	// std::map insert ������� ����
	auto result = mBitmapResources.insert({ filename, nullptr });
	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());
		result.first->second = spBitmap;
	}

	// #. ��ȯ�� pair�� ���ͷ�����(first)�� �� Key + Value�� ������ �ִ� �����̰� �� ������ Bitmap(second)�� �Ϲ� ������ �������� ��ȯ�� �ش�.
	return result.first->second.Get();

}
