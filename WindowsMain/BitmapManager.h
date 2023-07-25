#pragma once

// #. ��Ʈ�� �Ŵ��� : "�̹����� �ҷ����� ��Ʈ���� �����ϴ� ��"�� "ȭ�鿡 �׸��� ��"�� �и�
//		=> ���� �̸��� �޾Ƽ� ID2D1Bitmap ��ü�� ����
//			=> ó�� �����ϴ� ���̶�� ���ο� ID2D1Bitmap ��ü�� �����ϰ� �� �����͸� ��ȯ
//			=> �̹� �����ϴ� ���̶�� �ش� ID2D1Bitmap ��ü�� ��ȯ
//		=> ���� ������ ���� ��� �ʿ�
//			=> ID2D1Bitmap ����
//			=> ���� �̸����� �˻�( �ߺ� �Ұ��� )
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <map>
#include <string>

// #. final : �ش� Ŭ�����κ��� ���̻� �ڽ��� ���� �� ����
class BitmapManager final
{
public:
	// #. �̱��� ����
	//		=> static�� Ȱ���Ͽ� �����ڰ� ������ ȣ��Ǿ �ϳ��� �����ǰ� �����.
	//		=> �ν��Ͻ��� ��ȯ�ϴ� �Լ��� �����.
	//			=> ���ο� static ������ Ŭ���� �ν��Ͻ��� ����� �ڽ��� ��ȯ�Ѵ�.
	//		=> Ŭ������ ����Լ��� ȣ���Ϸ��� �ν��Ͻ��� �ʿ� -> static �Լ��� ������ �Ѵ�.
	static BitmapManager& Instance()
	{
		static BitmapManager instance;
		return instance;
	}

private:
	// #. �̱����� �� �ϳ��� �ν��Ͻ��� �����ؾ߸� �Ѵ�.
	//		=> �⺻ ������, ���� ������, ���� ���� �����ڸ� ���ܹ�����.
	BitmapManager() {}
	BitmapManager(const BitmapManager&) {}
	void operator =(const BitmapManager&) {}

public:
	~BitmapManager() {}

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};	// �̹����� �ҷ��� �� �������� �� ����ϴ� ���� Ÿ��
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};

	// #. map�� [key, value]������ ������ �Ǿ� �ִ�. �̹����� �����ϱ� ���ؼ� <���ϸ�, ��Ʈ��> ������ ���� �ʿ�
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources;

private:
	// ���� �Լ��� �ڵ��� ����� �������Ͽ� ID2D1Bitmap�� ������ �ִ� �Լ�
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();
	// �ʿ� �����Ͱ� �ִ��� ������ Ȯ���ϰ� �����ϴ� ����� �ϴ� �Լ�
	//		=> �ε��� �ߴ��� ���ߴ��� �� �� �ְ� ���ϸ��� �Ű������� �޾Ƽ� Ȯ���Ѵ�.
	ID2D1Bitmap* LoadBitmap(std::wstring filename);
};