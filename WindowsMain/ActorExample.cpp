#include <windows.h>
#include "D2DFramework.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	// #6. WinMain작성
	int ret{};

	try {
		D2DFramework myFramework;

		if (SUCCEEDED(myFramework.Initialize(hInstance)))
		{
			// #6-1. 클래스의 초기화를 성공했다면 계속해서 GameLoop안에서 돈다.
			ret = myFramework.GameLoop();
		}
	}
	catch (const com_exception& e) {
		static wchar_t wstr[64]{};
		size_t len;

		// #6-2. 멀티 바이트 스트링을 와이드 스트링으로 바꾸어서 버그 메시지를 출력해 준다.
		mbstowcs_s(&len, wstr, e.what(), 64);
		OutputDebugString(wstr);
	}
	return ret;
}