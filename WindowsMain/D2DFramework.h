#pragma once
#include <d2d1.h>
#include <wrl/client.h>
// #1. �����ӿ�ũ�� �ʿ��� ��������� �ű��.

class D2DFramework
{
// ����ؼ� ����� �� �ֵ��� protected
protected:
	// #2. �����ӿ�ũ�� ���丮�� ����Ÿ���� �ű��.
	//		=> ComPtr�� ����Ͽ� ����Ʈ �����ͷ� ����� �ش�.
	Microsoft::WRL::ComPtr <ID2D1Factory> gpD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> gpRenderTarget{};

public:
	// #3. �ʱ�ȭ, ����, �׸���� ���õ� �ൿ�� �Լ��� ����� �ش�.
	//		=> ��� COM�� HRESULT�� ��ȯ�ϱ� ������ �ʱ�ȭ �Լ� ���� HRESULT�� ��ȯ�ϵ��� �Ѵ�.
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	// #4. Error�޼����� ����� �Լ��� �����.
	//		=> �޼��� �ڽ����� ��µ� �޼����� ���� �ڵ�, �׸��� ���� ������ �Ű� ������ �����Ѵ�.
	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
};