#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <stdio.h>

// #. Direct2D ���ҽ�
//		=> �ϵ������ ������ Ȱ���� �׸��� API�� CPU �߽��� ���ҽ���� CPU������ �� ����Ǵ� �׸��� ��ɵ��� ������ ������
//		=> �� �� �Ϻθ� �ϵ���� ������ �����Ѵ�. GPU�� ���ۻ纰�� �����̳� ������ �ٸ��� ������ �ϵ���� ���� ���ҽ��� �������
//		=> ���ϴ� ��쵵 �����ϹǷ� ������ ���� GPU ���ҽ��� ����ϰ� �׷��� ���ϸ� CPU�� ���ҽ��� ����� �� �־�� �Ѵ�.
// #. Direct2D�� �̷��� ������ ���� ���ҽ��� ũ�� 2������ �����Ѵ�.
//		=> ��ġ �������� ���ҽ� : ID2D1Factory�� ���� CPU���� �����Ǵ� ���ҽ�
//		=> ��ġ �������� ���ҽ� : ID2D1RenderTarget�� ���� GPU�� �����Ǵ� ���ҽ�
//			=> �ϵ���� ������ ��밡���ϴٸ� GPU�� ���ҽ��� ���صǾ� ���� �׸��Ⱑ ����
// #. ��ġ �ս�
//		=> �ϵ���� ���� ���ҽ��� ������ ���� ǰ���̶�� ���� ��� CPU/RAM�� ���� �ſ� �������� GPU/V-RAM�� ����ϴ� ���� ���ҽ� ���� ������ �ݰ� �ȴ�.
//		=> ALT + TAB���� �۾� ��ȯ�� �ϰų� �ý��� �����찡 ������� ��, ���� ��尡 �۵��ϰų� ��üȭ�� ���� �ٸ� ���� ����ǰų� �� �׷��ȿ� �ΰ��� �۾��� �� ��� Device Lost�� �߻��ϰ� �ȴ�.
//		=> Ư�� ������ ��û�� GPU/V-RAM�� ����ϹǷ� �̷� ������ �ſ� ���� �߻��Ѵ�. �̷� ��ġ �ս��� �߻��ϸ�, ��ġ �������� ���ҽ��� �ƹ��͵� ����� �� ���� ���°� �Ǳ� ������
//		=> ���� ���ҽ����� ��� �����ϰ� �ٽ� ������ �Ѵ�.
// #. DirectX 11
//		=> ����ũ�μ���Ʈ���� �̷� �������� �ؼ��� ����� �߰� �ߴ�.
//		=> ���� �׷��� ����̽� �������̽���� ������ ����Ͽ� ���� ���� ���� �ϳ��� ��ġ�� ����ص� ������ ������ �����߱� ������ ���� �߻����� �ʴ´�.
//		=> ������, ���� �������� ��ġ�� ������ �������� ������ ���� ���� ��¿ �� ���� ��ġ �ս��� �߻��Ѵ�.
//			=> �׷��� ī�� ����̹� ���׷��̵�
//			=> ������ �׷���ī�忡�� ���� �׷��� ī��� ����� ��
//			=> ������ CPU�� ���� �׷���ī�尡 �޸� ��찡 ����. ������ �翬�� �������Ƿ� ������ ������ �׷���ī�带 ����ϴ� ���� ������ �� �׷���ī�尡 ����Ī �Ǵ� ����̴�.
//			=> ���� ��ġ�� ������ ���� ���
//			=> ���� �׷���ī�尡 ������ ���ŵǰų� �߰��� ��
//		=> ������ �޸� ALT + TAB ���� �۾� ��ȯ������ ��ġ �ս��� �߻����� �ʴ´�.
//		=> Direct2D������ ��ġ �ս��� üũ�� �� �ִ� ������ ��� �׸��Ⱑ ������ ȭ�鿡 ǥ���ϴ� ����, �� ID2D1HwndRenderTarget::EndDraw() �Լ��� ��ȯ������ �� �� �ִ�.
//			=> �� ��ȯ���� S_OK �ƴ϶�� ��ġ �սǿ� ���� ó���� �ؾ� �Ѵ�.
//		=> �̷� ��ġ ���� ���ҽ����� ��� ��Ƽ� �� ���� �����ϰ� ��������� �ϸ�, �սǵǸ� ��������� �Ѵ�.
//		=> ������ ���� ���¿����� ���� Ÿ�ٸ��� ��ġ �������̴�.
class com_exception : public std::exception
{
	HRESULT result;

public:
	com_exception(HRESULT hr) : result(hr) {}

	virtual const char* what() const override
	{
		static char str[64] = {};
		sprintf_s(
			str, "Failure with HRESULT of %08X",
			static_cast<unsigned int>(result)
		);
		return str;
	}
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr)) throw com_exception(hr);
}

class D2DFramework
{
	const LPCWSTR WindowClassName{ L"D2DWindowClass" };

protected:
	HWND mHwnd;

	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();
	virtual HRESULT CreateDeviceResources();

public:
	virtual HRESULT Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768
	);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};