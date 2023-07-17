#include <windows.h>
#include "D2DFramework.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	// #6. WinMain�ۼ�
	int ret{};

	try {
		D2DFramework myFramework;

		if (SUCCEEDED(myFramework.Initialize(hInstance)))
		{
			// #6-1. Ŭ������ �ʱ�ȭ�� �����ߴٸ� ����ؼ� GameLoop�ȿ��� ����.
			ret = myFramework.GameLoop();
		}
	}
	catch (const com_exception& e) {
		static wchar_t wstr[64]{};
		size_t len;

		// #6-2. ��Ƽ ����Ʈ ��Ʈ���� ���̵� ��Ʈ������ �ٲپ ���� �޽����� ����� �ش�.
		mbstowcs_s(&len, wstr, e.what(), 64);
		OutputDebugString(wstr);
	}
	return ret;
}