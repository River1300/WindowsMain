#pragma once
#include "D2DFramework.h"

// #1. BMP ������ ����
// #2. BITMAPFILEHEADER �б�
// #3. BITMAPINFOHEADER �б�
// #4. ���� �ȼ� ã��
//		=> BITMAPFILEHEADER.bfOffBits ���( ���� ���ۺ��� �ȼ������� Offset )
// #5. �ȼ� �б�

class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;
	// #. �׸��� �׸��� ���� �������̽��� ������ ����� �����Ѵ�.
	//		=> ���Ϸκ��� �����͸� �а� mspBitmap�� �÷� ���� �׸��� ���� ����

public:
	HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Image Example",
		UINT width = 1024,
		UINT height = 768) override;
	void Render() override;
	// #. �⺻���� ������ ��ũ�� ��� �ʱ�ȭ�� �׸���
	//		=> ��Ʈ���� �߰������� �׸� �����̹Ƿ� �������̵� ���ش�.

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	// #. �׸������� �ҷ����� �Լ�
	//		=> ������ �о�ͼ� mspBitmap�� �׸��� �׸� ����
	//		=> ��Ʈ���� �޾Ƽ� ���ο��� �������̽� �޸𸮸� ������ �ش�.
};