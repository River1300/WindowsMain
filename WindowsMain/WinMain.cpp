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

//#include <Windows.h>
//
//const wchar_t gClassName[]{ L"MyWindowClass" };
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//int WINAPI WinMain(
//	_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPSTR lpCmdLine,
//	_In_ int nShowCmd)
//{
//// #1. < ������ Ŭ���� ���� > : ������ Ŭ������� Ʋ�� ����� ���´�.
////		=> �������� ���, ��ɵ��� ������� ���� ����
//	WNDCLASSEX wc;
//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpszClassName = gClassName;
//	wc.hInstance = hInstance;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//	wc.lpfnWndProc = WindowProc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//// #1-1. < ������ Ŭ���� ��� >
////		=> ��ϵ� ������ Ŭ������ ���� �����츦 �� �� �ִ�.
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(nullptr, L"Failed to register class!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2. ������ ����
//	HWND hWnd;
//	hWnd = CreateWindowEx(
//		NULL,
//		gClassName,
//		L"Hellow Window",
//		WS_OVERLAPPEDWINDOW,
//		0, 0, 640, 480,
//		nullptr, nullptr,
//		hInstance,
//		nullptr
//	);
//
//// #2-1. ������ ���� Ȯ��
////		=> CreateWindowEx�Լ��� ���� ������ �����͸� ���� ���Ͽ��� ���
//	if (hWnd == nullptr)
//	{
//		MessageBox(nullptr, L"Failed to create Window!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2-2. �����찡 ���������� ����� ���� ���
////		=> ȭ�鿡 �����츦 �����ְ� �����츦 ����( �ٽ� �׸��� )���ش�.
//	ShowWindow(hWnd, nShowCmd);
//	UpdateWindow(hWnd);
//
//// #3. �޽��� ����
////		=> �ü���� �޼����� �ִ��� ����� ������ �����´�.
////		=> �޼����� ���� ������ �� ������ �ݺ��Ѵ�.
//	MSG msg;
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//// #3-1. ���α׷� ����
////		=> wParam : ���� ���� ���¸� ����ִ� ������
//	return static_cast<int>(msg.wParam);
//}
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//			break;
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//			break;
//	}
//	return 0;
//}

/* ------ < Win32 Message&Event > ----- */

/*
< ������OS�� �۵� ���� >
	#. ������� ���� ���� �������α׷����� �Ѳ����� ����ȴ�.
	#. �̷� ��Ƽ�½�ŷ ȯ���� ���� ������ ���� �̺�Ʈ �޽��� �ý����̴�.
	#. ���� Ű���� or ���콺�� ���� �Է��� '�Է� ť'��� ť ������ �����Ѵ�.
	#. �׸��� �ü�� ������ �����ڰ� ����Ǵ� �Է¸� ���ʴ�� �̾Ƴ���.
	#. �������� �������α׷��� ���ʴ�� �����Ѵ�.
	#. �������ڸ� �ü������ ���ۿ� �ʿ��� �������� �޾Ƽ� �������� �������α׷����� ���� �޽����� �����ְ�
	#. �������α׷��� �� �޽����� �޾ƿͼ� �ʿ��� ó���� �ϴ� ����̴�.
	#. �̶� ������ ������ ���� �޽����� �����ٰ� �ϸ�, �޴� �������� �̺�Ʈ�� �߻��ߴٰ� �Ǵ��Ѵ�.

< Message&Event > : �Լ��� ���α׷��Ӱ� �������� ȣ���� �ü���� �Ѵ�.
	#. �Լ� �����͸� �ü���� �Ѱ��ش�.
	#. �Է¿� ���� �̺�Ʈ�� �߻��Ҷ� ���� CALLBACK�� �߻��ϸ� �Լ��� ȣ���Ѵ�.
		#. �̺�Ʈ( CALLBACK ) : �Լ��� ����� ���� Ư�� ��Ȳ�� �Լ��� ��� ȣ������
		#. �޽��� : �������� ���� ���α׷��ֿ� �����ϱ� ���ؼ� '���� ���� ������ ����?'�� ���ϴ� ��

< MSG����ü�� ���� >
	#. < HWND hwnd; > : ������ �ڵ�
	#. < UINT message; > : �޼����� ������ ��Ÿ���� ������
		=> WM_( ������ �޼��� ) : ��Ʈ �÷��� �������� ���� �Ǿ� �ִ� �޼����� switch������ ȣ���� �� �ִ�.
	#. < WPARAM wParam; > : �޼����� ���� �߰� ����
		=> WORD : ���������� ������ WORD��� ǥ���Ѵ�.
			=> Register ��� ��� : �ܾ� ������ ���( ����� )
			=> 16��Ʈ ��ǻ�Ϳ��� 1WORD�� 16bit�̴�.
			=> 32��Ʈ ��ǻ�Ϳ��� 1WORD�� 32bit�̴�.
	#. < LPARAM lParam; > : �޼����� ���� �߰� ����
		=> LONG : WPARAM�� ���� ���ϴ� ���� ū ���� ��� ���� �� ū ����
			#. WPARAM�� WORDŸ���� Parameter��� �� �� �ְ�
			#. LPARAM�� LONGŸ���� Parameter��� �� �� �ִ�.
		#. �׷��� ����� WPARAM�� LPARAM�� ũ�Ⱑ ����. �ٸ� ��Ī�� �ٸ� ���̴�.
	#. < DWORD time; > : Double WORDŸ������ WORD�ΰ� ũ���� Ÿ���̶�� �� �� �ִ�.
		#. �޼����� ���޵Ǵ� �ð��� �����ϴ� �����̴�.
		=> �׷��� 64bit�����쿡�� DWORD�� 128bit�� �ƴ϶� �Ȱ��� 64bit�̴�.
	#. < POINT pt; > : x�� y��ǥ�� �����ִ� ����ü�̴�.
		#. �޼����� �߻��� ����� ���콺 ��ġ�� ����Ǵ� �����̴�.

< HIWORD&LOWORD > : 4byte�� �޸� ������ ������ �ɰ��� 16bit�� ������ ����Ѵ�.
	#. �׷� �ϳ��� �޸� ������ x, y���� ���� ������ �� �ִ�.
	#. DWORD�� �տ� �ִ� �޸� ������ HIWORD��
	#. DWORD�� �ڿ� �ִ� �޸� ������ LOWORD�� ������ �ִ�.
*/

//#include <Windows.h>
//#include <sstream>
//
//const wchar_t gClassName[]{ L"MyWindowClass" };
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//int WINAPI WinMain(
//	_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPSTR lpCmdLine,
//	_In_ int nShowCmd)
//{
//// #1. < ������ Ŭ���� ���� > : ������ Ŭ������� Ʋ�� ����� ���´�.
////		=> �������� ���, ��ɵ��� ������� ���� ����
//	WNDCLASSEX wc;
//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpszClassName = gClassName;
//	wc.hInstance = hInstance;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//	wc.lpfnWndProc = WindowProc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//// #1-1. < ������ Ŭ���� ��� >
////		=> ��ϵ� ������ Ŭ������ ���� �����츦 �� �� �ִ�.
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(nullptr, L"Failed to register Class!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2. ������ ����
//	HWND hWnd;
//	hWnd = CreateWindowEx(
//		NULL,
//		gClassName,
//		L"Hello Window",
//		WS_OVERLAPPEDWINDOW,
//		0, 0, 640, 480,
//		nullptr, nullptr,
//		hInstance,
//		nullptr);
//
//// #2-1. ������ ���� Ȯ��
////		=> CreateWindowEx�Լ��� ���� ������ �����͸� ���� ���Ͽ��� ���
//	if (hWnd == nullptr)
//	{
//		MessageBox(nullptr, L"Failed to Create Window!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2-2. �����찡 ���������� ����� ���� ���
////		=> ȭ�鿡 �����츦 �����ְ� �����츦 ����( �ٽ� �׸��� )���ش�.
//	ShowWindow(hWnd, nShowCmd);
//	UpdateWindow(hWnd);
//
//// #3. �޽��� ����
////		=> �ü���� �޼����� �ִ��� ����� ������ �����´�.
////		=> �޼����� ���� ������ �� ������ �ݺ��Ѵ�.
//	MSG msg;
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//// #. �����쿡�� ����Ǵ� ��� App���� '�޼��� ť'��� ���� �ִ�.
//// #. �ü���� ����ؼ� ���ʰ��� �޼����� ������ ���̴�. �׷� �� �޼����� �׾� ���� �ϳ��� ó���� �ʿ䰡 �ִ�.
//// #. �� �޼��� ť�� �޼����� ��� ���� �ϳ��� ó���� ���̴�.
//// #. �� �۾��� ���α׷��� ����ɶ� ���� ����ؼ� �ݺ��Ѵ�.
//		TranslateMessage(&msg);
//// #. �޼��� ť�� �޼����� ���� ���� �ü������ ����ϴ� �޼����� ���´�.
//// #. �� �޼����� �ۿ��� �״�� ����ϱ⿡�� �԰��� ���� �ʴ´�.
//// #. ���� TranslateMessage()�Լ��� �̿��ؼ� �����ϴ� �۾��� ���� �����Ѵ�.
//// #. �� �۾��� ���ؼ� �ۿ��� ����� �� �ִ� �������� �޼����� �ٲ��ش�.
//		DispatchMessage(&msg);
//// #. �ۿ��� �������� �����찡 ���� �� �ִ�.
//// #. ������ �Ϸ�� �޼����� ��� ������鿡�� �����ϴ� ���� ���ִ� �Լ��� DispatchMessage()�Լ���.
//	}
//
//// #3-1. ���α׷� ����
////		=> wParam : ���� ���� ���¸� ����ִ� ������
//	return static_cast<int>(msg.wParam);
//}
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//// #. DispatchMessage()�Լ��� ��ġ�� ���� �����쿡 �޼����� ���޵ȴ�.
//// #. ������ �ڵ��� ���� ���޵� �޼����� switch���� ���ؼ� ó���Ѵ�.
//
//	switch (message)
//	{
//		case WM_KEYDOWN:
//		{
//			std::wostringstream oss;
//			oss << "Virtual Key = " << wParam << std::endl;
//			OutputDebugString(oss.str().c_str());
//// #. wParam�� ����� ���� Ű������ Ű ���� ����� �޽��� â�� ��µȴ�.
//			break;
//		}
//		case WM_LBUTTONDOWN:
//		{
//			std::wostringstream oss;
//			oss << "X : " << LOWORD(lParam) << ", Y : " << HIWORD(lParam);
//			OutputDebugString(oss.str().c_str());
//// #. lParam�� ����� x, y��ǥ�� ����� �޽��� â�� ����Ѵ�.
//			break;
//		}
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//// #. ������ �ִ� �׸��� ���̽� ���̺��� ���� ���� ó���� �ش�.
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//// #. �Ϲ����� �׸��� �⺻ ó����ġ�� ���ؼ� ó���� �ش�.
//			break;
//	}
//	return 0;
//}

/* ------ < GDI( Graphics Device Interface ) > ----- */

/*
< GDI > : ������� �پ��ִ� � �׷��� ��ġ���� �� �׸��� �׸� �� �ִٶ�� ����

< Device Context > : �׸��� ���� �������� ��� ���� ��ġ
	#. < HDC > : DC�� �ڵ�
	#. < GetDC() > : � ��ġ�� �׸� ������ �����ϴ� �Լ� HDC�� ��ȯ�Ѵ�.

< ��ǥ��( Coordinates ) >
	#. Logical coordinates : �� ������ ���� ��ǥ ����
	#. Physical coordinates : ����� ���� ��ǥ ����

< PAINTSTRUCT ����ü > : �׸��� �׸� �����̳� ������ ���� ������ ������ �ִ�.
	#. BOOL fErase : ������� ���������, �� ����� ���� ������
	#. RECT rcPaint : RECT�� �»�ܰ� ���ϴ��� ��ǥ������ ���� ����ü�̴�.
		=> ��� �׸��� �簢�� ������ �����Ѵ�.

< Client Rect > : ������ �׸��� �׷����� �������� ����
< Window Rect > : ���� ǥ���ٰ� ����, �ּ�ȭŰ ���� ���Ե� ������ ����

< hatch > : ����

< CreateSolidBrush > :
	#. < RGB > : HIWORD/ROWORD�� ���� �������� �޸� ������ 3��� �Ͽ� ���ϴ� ���� ������ �� �ִ�.
		=> GetRValue(rgb)/GetGValue(rgb)/GetBValue(rgb)�Լ��� ���� ����Ʈ�� ����ȯ�Ͽ� �߶������ ������ ���� �ִ�.

< �׸��� ���� >
	#1. �ʿ��� ������Ʈ�� �����.
	#2. ������� ������Ʈ�� �����Ѵ�.
	#3. �׸��� �׸���.
*/

#include <Windows.h>
#include <sstream>

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
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

// #1-1. < ������ Ŭ���� ��� >
//		=> ��ϵ� ������ Ŭ������ ���� �����츦 �� �� �ִ�.
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to Register Class!", L"Error", MB_OK);
		return 0;
	}

// #2. ������ ����
	HWND hWnd;
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Hello Window",
		WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480,
		nullptr, nullptr,
		hInstance,
		nullptr);

// #2-1. ������ ���� Ȯ��
//		=> CreateWindowEx�Լ��� ���� ������ �����͸� ���� ���Ͽ��� ���
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to Create Window!", L"Error", MB_OK);
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

// #. �׸���� ���õ� ����� ó���ϴ� �Լ��� ����� �ش�.
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
// #. ��� ��� �κи�ŭ �׸��� �����Ѵ�.
// #. �׸��� �����ϰ� ������ ������ �����Ѵ�.

	HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, blueBrush);
	Rectangle(hdc, 0, 0, 100, 100);
// #. �귯�� �޸� ������ �Ҵ� �ް� �ڵ�� �����Ѵ�.
// #. ������ �귯���� �׸��⿡ ����ϵ��� SelectObject()�Լ��� ����� �ش�.
// #. �簢���� �׸��� ����� �귯���� �ڵ����� ���ȴ�.

	DeleteObject(blueBrush);
	EndPaint(hWnd, &ps);
}

// #4. ������ ���ν���
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			OnPaint(hWnd);
			break;
		}
		case WM_KEYDOWN:
		{
			std::wostringstream oss;
			oss << "Virtual Key = " << wParam << std::endl;
			OutputDebugString(oss.str().c_str());
			break;
		}
		case WM_LBUTTONDOWN:
		{
			std::wostringstream oss;
			oss << "X : " << LOWORD(lParam) << ", Y : " << HIWORD(lParam);
			OutputDebugString(oss.str().c_str());

			HDC hdc = nullptr;
			hdc = GetDC(hWnd);
			Rectangle(hdc, 0, 0, 100, 100);
			ReleaseDC(hWnd, hdc);
// #. ���콺 ���� ��ư�� ������ �簢���� �׸��� ��ɾ�
// #. hWnd�� �����Ͽ� ���� ���� ���� ������ ȭ�鿡 �׸����� �����Ѵ�.
// #. Rectangle()�Լ��� ���� hdc�� �簢���� �׷��ش�.
// #. GetDC�� ���� �����Ҵ�� �޸𸮸� ReleaseDC�� ���� ���������Ѵ�.
// #. � �����쿡 � DC�ڵ��� ������ ������ �Ű� ������ �����Ѵ�.
			break;
		}
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