#pragma once
#include <d2d1.h>
#include <wrl/client.h>
#include <stdio.h>
#include <exception>		// #1. ���� ó���� ���� ��� ������ �߰��Ѵ�.
//		=> C++���� ����ϴ� ������ ��� Ŭ������ ���ԵǾ� �ִ�.

// #2. ���� ��� Ŭ������ ��� ���� Ŭ������ ����� �ش�.
class com_exception : public std::exception
{
private:
	HRESULT result;

public:
	// #3. ���ܿ� ���� �����ڸ� ����� �ش�.
	//		=> �����ڸ� ���ؼ� ���� �ڵ带 ������ �ش� �ڵ带 ���( resutl )�� ������ �� �ִ�.
	com_exception(HRESULT hr) : result(hr) {}

	// #4. ������ ����ϴ� �Լ��� �����.
	//		=> ���� �ڵ�( result )�� 16������ ��ȯ�Ͽ� str�� �����Ѵ�.
	//		=> ������ ���� �ڼ��� ������ str�� ��ȯ�Ͽ� ����� �ش�.
	virtual const char* what() const override
	{
		static char str[64]{};
		sprintf_s(str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));

		return str;
	}
};

// #5. �����Ͽ��� �� ���ܸ� ������ �Լ��� �����.
//		=> HRESULT�� �Ű� ������ �޾Ƽ� ���࿡ hr�� ���� ���� ���
//		=> com_exception Ŭ������ hr�� �����ڷ� �Ѱ��ش�. �� ���� ������ com_exception���� ������.
inline void ThrowIfFailed(HRESULT hr)
{
	// #. < inline > : �Լ��� STACK�޸� ������ ���̰� ���������� ������ �����Ѵ�.
	//		=> ������ ����� ��� inline�� Ȱ���ϸ� �ӵ��� �� ��������.
	//		=> �������� �ʴ� ��� �����Ϸ��� �ź��� ���� �ִ�.
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
protected:
	Microsoft::WRL::ComPtr <ID2D1Factory> gpD2DFactory{};
	Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> gpRenderTarget{};

public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title = L"ERROR");
};