#include <Windows.h>
#include "BuggyHouse.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	int ret{};

	try
	{
		BuggyHouse myFramework;

		HRESULT hr;
		hr = myFramework.Initialize(hInstance);

		if (SUCCEEDED(hr))
		{
			ret = myFramework.GameLoop();
			myFramework.Release();
		}
	}
	catch (const com_exception& e)
	{
		static wchar_t wstr[128] = {};
		size_t len;

		mbstowcs_s(&len, wstr, e.what(), 128);
		MessageBox(
			nullptr, wstr, L"DirectX Exception",
			MB_ICONEXCLAMATION | MB_OK
		);
	}

	return static_cast<int>(ret);
}