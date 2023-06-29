/* ------ < Win32 > ----- */

/*
< 진입점( Entry Point ) >
	#. < SAL( Source Code Annotation Language ) > : 소스코드에 코드적으로 주석을 닳은 언어
		#. WINAPI( __stdcall ) : 표준 호출
			=> cpu의 기능 중 기억(레지스터)장치에 어떤 방식으로 저장할지 지정
		#. _In_( input ) : 매개 변수를 입력용으로 사용한다는 뜻
			=> 윈도우가 앱을 실행할 때 넘겨주는 것( 나에게 들어오는 값 )
		#. _In_opt_ ( input optional ) : 매개 변수가 들어올 수도 안들어올 수도 있다는 뜻

	#. < HINSTANCE( Handle Of Instance ) > : 인스턴스에 대한 핸들
		#. HDD[메모장.exe] ->( 실행 )-> RAM[메모장] ->( 실행 )-> RAM[메모장, 메모장]
			=> 하나의 앱( 메모장 )에서 여러개의 인스턴스가 만들어 진다.
			=> 클래스가 객체를 찍어내는 개념과 같다.
			=> 운영체제 개념으로 보았을 때는 HDD에서 RAM으로 인스턴스를 찍어 낸다고 볼 수 있다.
		#. Context Switch : RAM에 올라와 있는 인스턴스를 HDD에 백업해둔다.
			#. 그러면 RAM에 백업한 만큼 공간이 생기고 이 공간에 새로운 앱을 실행시킬 수 있다.
		#. < 인스턴스 > : 윈도우즈 앱이 실행되는 하나의 메모리에 올라간 공간을 인스턴스라 부른다.
		#. < 핸들 > : 포인터
			#. 인스턴스는 메모리에 올라온다. 인스턴스마다 주소를 갖게된다.
			#. 그래서 그 포인터가 돌아오는데 이를 핸들이라고 부른다.
		#. 즉 핸들오브인스턴스는 앱을 더블클릭하면 운영체제에서 실행하였기 때문에 운영체제가 앱을 RAM에 올린다.
		#. RAM에 올라온 인스턴스의 주소값 즉 포인터를 돌려주는 타입이다.

	#. < LPSTR( Long Pointer STRing ) > : 문자열을 가리키는 포인터( char*와 같은 의미 )

< MessageBox >
	#. HWND : 윈도우의 핸들
		#. 여러개의 윈도우가 동시에 실행 될 수 있기 때문에 각각의 윈도우를 관리하기 위해서 고유한 식별 번호를 가지고 있다.
		#. 이러한 식별 번호를 핸들이라고 부른다.
			#. 운영체제에서 윈도우란 앱 하나를 말하는 것이 아니다.
			#. 비쥬얼 스튜디오를 예를 들면 상단의 메뉴 화면이 윈도우 하나, 코드를 작성하는 칸이 윈도우 하나, 버그를 나타내는 창이 하나
			#. 즉 하나의 앱에 여러개의 윈도우를 가지고 있을 수 있다.
	#. LPCTSTR : 상수형 문자열 포인터
		#. TSTR( WSTR/STR ) : 아스키코드와 유니코드 문자열
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
< 함수 >
	#. < LRESULT( Long PTR ) >  : 윈도우 함수들이 처리한 결과
	#. < WPARAM( UINT PTR ) > : 포인터
	#. < LPARAM( Long PTR ) > : 포인터

< 윈도우 클래스 >
	#. < WNDCLASSEX > : C스타일 구조체로 윈도우의 기능을 묶어놓았다.
		#. < .style > : 윈도우의 스타일을 지정해 준다.
			=> HREDRAW( Horizontal Redraw ) : 가로를 다시 그린다.
			=> VREDRAW( Vertical Redraw ) : 세로를 다시 그린다.
		#. < .lpszClassName( Long Pointer String Zero ) > : '\0'으로 끝나는 문자열 포인터
		#. < .hInstance > : 어떤 인스턴스에서 어떤 윈도우 클래스를 사용할 것인지 지정
		#. < .hbrBackground > : 브러쉬로 칠하는 배경 색
		#. < .lpfnWndProc > : 윈도우 프로시져에 대한 함수 포인터를 받는다.
		#. < .cbSize( Count Byte ) > : 구조체의 바이트 수를 받는다.

	#. < ZeroMemory > : 구조체의 주소와 크기를 전달 받아 해당 메모리를 0으로 초기화 해준다.
		=> C++스타일로 구조체 변수에 유니폼 초기화를 해주어도 동일한 작업이나 마찬가지다.
		=> 구조체를 사용할 때는 반드시 0으로 초기화를 해주어야 한다.

	#. < RegisterClassEx > : 윈도우 구조체 클래스를 등록한다.
		#. < ATOM > : 고유한 값, 윈도우 클래스에 대한 고유 값
		#. 윈도우가 정상적으로 등록되지 않았다면 0을 반환한다.

< 윈도우 생성 >
	#. < HWND > : 윈도우는 기본적으로 만들어진 윈도우에 대한 포인터가 항상 돌아온다.
		#. < CreateWindowEx > : 윈도우를 만들고 해당 윈도우에 대한 포인터를 반환한다.
			=> 반환된 포인터( 핸들 )을 사용하여 해당 윈도우를 사용하면 된다.
	#. < lpClassName > : 만들어 놓은 클래스 이름과 동일한 클래스 이름을 등록해야 한다.
	#. < HINSTANCE > : 만들어 놓은 클래스 HINSTANCE와 동일한 포인터를 등록해야 한다.

< 메시지 루프 >
	#. < MSG > : 메세지 구조체
		#. < GetMessage > : 주소값을 받아 윈도우에서 메시지를 하나 가져온다.
			=> LPMSG : 받아올 메세지 구조체의 포인터
			=> HWND : 특정한 윈도우 메시지만 받아온다. ( nullptr은 모든 윈도우 메시지를 받는다. )
			=> UINT : 메세지의 필터링 정보 최소값과 최대값을 지정해 받아온다. ( 0, 0 은 모든 메시지를 받는다. )
		#. < TranslateMessage > : 운영체제 언어로된 메세지를 앱이 이해할 수 있게 번역한다.

< 윈도우 프로시져 >
	#. < WM_ > : 윈도우 메세지의 약자
	#. < DefWindowProc > : 기본 윈도우 프로시져로 일반적인 윈도우 기능을 대신 해준다.
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
// #1. < 윈도우 클래스 생성 > : 윈도우 클래스라는 틀을 만들어 놓는다.
//		=> 윈도우의 모양, 기능들을 묶어놓은 개념 집합
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

// #1-1. < 윈도우 클래스 등록 >
//		=> 등록된 윈도우 클래스를 통해 윈도우를 찍어낼 수 있다.
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register class!", L"Error", MB_OK);
		return 0;
	}

// #2. 윈도우 생성
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

// #2-1. 윈도우 생성 확인
//		=> CreateWindowEx함수를 통해 윈도우 포인터를 받지 못하였을 경우
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create Window!", L"Error", MB_OK);
		return 0;
	}

// #2-2. 윈도우가 정상적으로 만들어 졌을 경우
//		=> 화면에 윈도우를 보여주고 윈도우를 갱신( 다시 그리기 )해준다.
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

// #3. 메시지 루프
//		=> 운영체제에 메세지가 있는지 물어보고 있으면 가져온다.
//		=> 메세지가 없을 때까지 이 과정을 반복한다.
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

// #3-1. 프로그램 종료
//		=> wParam : 현재 앱의 상태를 담고있는 포인터
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