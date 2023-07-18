#include <windows.h>
#include "BugBit.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int ret{};

	try {
		BugBit myFramework;

		if (SUCCEEDED(myFramework.Initialize(hInstance)))
		{
			ret = myFramework.GameLoop();
		}
	}
	catch (const com_exception& e) {
		static wchar_t wstr[64]{};
		size_t len;

		mbstowcs_s(&len, wstr, e.what(), 64);
		OutputDebugString(wstr);
	}
	return ret;
}