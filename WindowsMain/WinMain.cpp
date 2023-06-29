/* ------ < Win32 > ----- */

/*
< ������( Entry Point ) >
	#. < SAL( Source Code Annotation Language ) > : �ҽ��ڵ忡 �ڵ������� �ּ��� ���� ���
		#. WINAPI( __stdcall ) : ǥ�� ȣ��
			=> cpu�� ��� �� ���(��������)��ġ�� � ������� �������� ����
		#. _In_( input ) : �Ű� ������ �Է¿����� ����Ѵٴ� ��
			=> �����찡 ���� ������ �� �Ѱ��ִ� ��( ������ ������ �� )
		#. _In_opt_ ( input optional ) : �Ű� ������ ���� ���� �ȵ��� ���� �ִٴ� ��

	#. < HINSTANCE( Handle Of Instance ) > : �ν��Ͻ��� ���� �ڵ�
		#. HDD[�޸���.exe] ->( ���� )-> RAM[�޸���] ->( ���� )-> RAM[�޸���, �޸���]
			=> �ϳ��� ��( �޸��� )���� �������� �ν��Ͻ��� ����� ����.
			=> Ŭ������ ��ü�� ���� ����� ����.
			=> �ü�� �������� ������ ���� HDD���� RAM���� �ν��Ͻ��� ��� ���ٰ� �� �� �ִ�.
		#. Context Switch : RAM�� �ö�� �ִ� �ν��Ͻ��� HDD�� ����صд�.
			#. �׷��� RAM�� ����� ��ŭ ������ ����� �� ������ ���ο� ���� �����ų �� �ִ�.
		#. < �ν��Ͻ� > : �������� ���� ����Ǵ� �ϳ��� �޸𸮿� �ö� ������ �ν��Ͻ��� �θ���.
		#. < �ڵ� > : ������
			#. �ν��Ͻ��� �޸𸮿� �ö�´�. �ν��Ͻ����� �ּҸ� ���Եȴ�.
			#. �׷��� �� �����Ͱ� ���ƿ��µ� �̸� �ڵ��̶�� �θ���.
		#. �� �ڵ�����ν��Ͻ��� ���� ����Ŭ���ϸ� �ü������ �����Ͽ��� ������ �ü���� ���� RAM�� �ø���.
		#. RAM�� �ö�� �ν��Ͻ��� �ּҰ� �� �����͸� �����ִ� Ÿ���̴�.

	#. < LPSTR( Long Pointer STRing ) > : ���ڿ��� ����Ű�� ������( char*�� ���� �ǹ� )

< MessageBox >
	#. HWND : �������� �ڵ�
		#. �������� �����찡 ���ÿ� ���� �� �� �ֱ� ������ ������ �����츦 �����ϱ� ���ؼ� ������ �ĺ� ��ȣ�� ������ �ִ�.
		#. �̷��� �ĺ� ��ȣ�� �ڵ��̶�� �θ���.
			#. �ü������ ������� �� �ϳ��� ���ϴ� ���� �ƴϴ�.
			#. ����� ��Ʃ����� ���� ��� ����� �޴� ȭ���� ������ �ϳ�, �ڵ带 �ۼ��ϴ� ĭ�� ������ �ϳ�, ���׸� ��Ÿ���� â�� �ϳ�
			#. �� �ϳ��� �ۿ� �������� �����츦 ������ ���� �� �ִ�.
	#. LPCTSTR : ����� ���ڿ� ������
		#. TSTR( WSTR/STR ) : �ƽ�Ű�ڵ�� �����ڵ� ���ڿ�
*/

//#include <Windows.h>
//
//int WINAPI WinMain(
//	_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPSTR lpCmdLine,
//	_In_ int nShowCmd)
//{
//	MessageBox(nullptr, L"Hello World", L"SimpleWindow", MB_OK);
//	MessageBox(nullptr, L"Hello World", L"SimpleWindow", MB_YESNO);
//
//	return 0;
//}

/* ------ < Win32 DEFAULT > ----- */

/*
< �Լ� >
	#. < LRESULT( Long PTR ) >  : ������ �Լ����� ó���� ���
	#. < WPARAM( UINT PTR ) > : ������
	#. < LPARAM( Long PTR ) > : ������

< ������ Ŭ���� >
	#. < WNDCLASSEX > : C��Ÿ�� ����ü�� �������� ����� ������Ҵ�.
		#. < .style > : �������� ��Ÿ���� ������ �ش�.
			=> HREDRAW( Horizontal Redraw ) : ���θ� �ٽ� �׸���.
			=> VREDRAW( Vertical Redraw ) : ���θ� �ٽ� �׸���.
		#. < .lpszClassName( Long Pointer String Zero ) > : '\0'���� ������ ���ڿ� ������
		#. < .hInstance > : � �ν��Ͻ����� � ������ Ŭ������ ����� ������ ����
		#. < .hbrBackground > : �귯���� ĥ�ϴ� ��� ��
		#. < .lpfnWndProc > : ������ ���ν����� ���� �Լ� �����͸� �޴´�.
		#. < .cbSize( Count Byte ) > : ����ü�� ����Ʈ ���� �޴´�.

	#. < ZeroMemory > : ����ü�� �ּҿ� ũ�⸦ ���� �޾� �ش� �޸𸮸� 0���� �ʱ�ȭ ���ش�.
		=> C++��Ÿ�Ϸ� ����ü ������ ������ �ʱ�ȭ�� ���־ ������ �۾��̳� ����������.
		=> ����ü�� ����� ���� �ݵ�� 0���� �ʱ�ȭ�� ���־�� �Ѵ�.

	#. < RegisterClassEx > : ������ ����ü Ŭ������ ����Ѵ�.
		#. < ATOM > : ������ ��, ������ Ŭ������ ���� ���� ��
		#. �����찡 ���������� ��ϵ��� �ʾҴٸ� 0�� ��ȯ�Ѵ�.

< ������ ���� >
	#. < HWND > : ������� �⺻������ ������� �����쿡 ���� �����Ͱ� �׻� ���ƿ´�.
		#. < CreateWindowEx > : �����츦 ����� �ش� �����쿡 ���� �����͸� ��ȯ�Ѵ�.
			=> ��ȯ�� ������( �ڵ� )�� ����Ͽ� �ش� �����츦 ����ϸ� �ȴ�.
	#. < lpClassName > : ����� ���� Ŭ���� �̸��� ������ Ŭ���� �̸��� ����ؾ� �Ѵ�.
	#. < HINSTANCE > : ����� ���� Ŭ���� HINSTANCE�� ������ �����͸� ����ؾ� �Ѵ�.

< �޽��� ���� >
	#. < MSG > : �޼��� ����ü
		#. < GetMessage > : �ּҰ��� �޾� �����쿡�� �޽����� �ϳ� �����´�.
			=> LPMSG : �޾ƿ� �޼��� ����ü�� ������
			=> HWND : Ư���� ������ �޽����� �޾ƿ´�. ( nullptr�� ��� ������ �޽����� �޴´�. )
			=> UINT : �޼����� ���͸� ���� �ּҰ��� �ִ밪�� ������ �޾ƿ´�. ( 0, 0 �� ��� �޽����� �޴´�. )
		#. < TranslateMessage > : �ü�� ���ε� �޼����� ���� ������ �� �ְ� �����Ѵ�.

< ������ ���ν��� >
	#. < WM_ > : ������ �޼����� ����
	#. < DefWindowProc > : �⺻ ������ ���ν����� �Ϲ����� ������ ����� ��� ���ش�.
*/

#include <Windows.h>

const wchar_t gClassName[]{ L"MyWindowClass" };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
// #1. < ������ Ŭ���� ���� > : ������ Ŭ������� Ʋ�� ����� ���´�.
//		=> �������� ���, ��ɵ��� ������� ���� ����
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

// #1-1. < ������ Ŭ���� ��� >
//		=> ��ϵ� ������ Ŭ������ ���� �����츦 �� �� �ִ�.
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register class!", L"Error", MB_OK);
		return 0;
	}

// #2. ������ ����
	HWND hWnd;
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Hellow Window",
		WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480,
		nullptr, nullptr,
		hInstance,
		nullptr
	);

// #2-1. ������ ���� Ȯ��
//		=> CreateWindowEx�Լ��� ���� ������ �����͸� ���� ���Ͽ��� ���
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create Window!", L"Error", MB_OK);
		return 0;
	}

// #2-2. �����찡 ���������� ����� ���� ���
//		=> ȭ�鿡 �����츦 �����ְ� �����츦 ����( �ٽ� �׸��� )���ش�.
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

// #3. �޽��� ����
//		=> �ü���� �޼����� �ִ��� ����� ������ �����´�.
//		=> �޼����� ���� ������ �� ������ �ݺ��Ѵ�.
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

// #3-1. ���α׷� ����
//		=> wParam : ���� ���� ���¸� ����ִ� ������
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}
	return 0;
}