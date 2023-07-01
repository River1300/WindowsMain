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
//// #1. < 윈도우 클래스 생성 > : 윈도우 클래스라는 틀을 만들어 놓는다.
////		=> 윈도우의 모양, 기능들을 묶어놓은 개념 집합
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
//// #1-1. < 윈도우 클래스 등록 >
////		=> 등록된 윈도우 클래스를 통해 윈도우를 찍어낼 수 있다.
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(nullptr, L"Failed to register class!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2. 윈도우 생성
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
//// #2-1. 윈도우 생성 확인
////		=> CreateWindowEx함수를 통해 윈도우 포인터를 받지 못하였을 경우
//	if (hWnd == nullptr)
//	{
//		MessageBox(nullptr, L"Failed to create Window!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2-2. 윈도우가 정상적으로 만들어 졌을 경우
////		=> 화면에 윈도우를 보여주고 윈도우를 갱신( 다시 그리기 )해준다.
//	ShowWindow(hWnd, nShowCmd);
//	UpdateWindow(hWnd);
//
//// #3. 메시지 루프
////		=> 운영체제에 메세지가 있는지 물어보고 있으면 가져온다.
////		=> 메세지가 없을 때까지 이 과정을 반복한다.
//	MSG msg;
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//// #3-1. 프로그램 종료
////		=> wParam : 현재 앱의 상태를 담고있는 포인터
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
< 윈도우OS의 작동 원리 >
	#. 윈도우는 여러 개의 응용프로그램들이 한꺼번에 실행된다.
	#. 이런 멀티태스킹 환경을 위해 개발한 것이 이벤트 메시지 시스템이다.
	#. 먼저 키보드 or 마우스를 통한 입력을 '입력 큐'라는 큐 공간에 저장한다.
	#. 그리고 운영체제 윈도우 관리자가 실행되는 입력만 차례대로 뽑아낸다.
	#. 실행중인 응용프로그램에 차례대로 전달한다.
	#. 정리하자면 운영체제에서 조작에 필요한 정보들을 받아서 실행중인 응용프로그램에게 각각 메시지를 보내주고
	#. 응용프로그램은 그 메시지를 받아와서 필요한 처리를 하는 방식이다.
	#. 이때 보내는 측에서 보면 메시지를 보낸다고 하며, 받는 측에서는 이벤트가 발생했다고 판단한다.

< Message&Event > : 함수는 프로그래머가 만들지만 호출은 운영체제가 한다.
	#. 함수 포인터를 운영체제에 넘겨준다.
	#. 입력에 따른 이벤트가 발생할때 마다 CALLBACK이 발생하며 함수를 호출한다.
		#. 이벤트( CALLBACK ) : 함수를 등록해 놓고 특정 상황에 함수를 대신 호출해줌
		#. 메시지 : 여러개의 응용 프로그래밍에 전달하기 위해서 '누구 한테 전달할 꺼임?'을 정하는 것

< MSG구조체의 형태 >
	#. < HWND hwnd; > : 윈도우 핸들
	#. < UINT message; > : 메세지의 유형을 나타내는 정수값
		=> WM_( 윈도우 메세지 ) : 비트 플래그 형식으로 정의 되어 있는 메세지를 switch문에서 호출할 수 있다.
	#. < WPARAM wParam; > : 메세지에 대한 추가 정보
		=> WORD : 레지스터의 단위를 WORD라고 표현한다.
			=> Register 명령 기억 : 단어 단위로 기억( 어쎔블리어 )
			=> 16비트 컴퓨터에서 1WORD는 16bit이다.
			=> 32비트 컴퓨터에서 1WORD는 32bit이다.
	#. < LPARAM lParam; > : 메세지에 대한 추가 정보
		=> LONG : WPARAM이 담지 못하는 좀더 큰 것을 담기 위한 더 큰 단위
			#. WPARAM은 WORD타입의 Parameter라고 할 수 있고
			#. LPARAM은 LONG타입의 Parameter라고 할 수 있다.
		#. 그런데 현재는 WPARAM과 LPARAM의 크기가 같다. 다만 명칭만 다른 뿐이다.
	#. < DWORD time; > : Double WORD타입으로 WORD두개 크기의 타입이라고 할 수 있다.
		#. 메세지가 전달되는 시간을 저장하는 공간이다.
		=> 그런데 64bit윈도우에서 DWORD는 128bit가 아니라 똑같이 64bit이다.
	#. < POINT pt; > : x와 y좌표를 갖고있는 구조체이다.
		#. 메세지가 발생할 당시의 마우스 위치가 저장되는 공간이다.

< HIWORD&LOWORD > : 4byte의 메모리 공간을 반으로 쪼개어 16bit씩 나누어 사용한다.
	#. 그럼 하나의 메모리 공간에 x, y값을 각각 저장할 수 있다.
	#. DWORD의 앞에 있는 메모리 공간을 HIWORD가
	#. DWORD의 뒤에 있는 메모리 공간을 LOWORD가 가지고 있다.
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
//// #1. < 윈도우 클래스 생성 > : 윈도우 클래스라는 틀을 만들어 놓는다.
////		=> 윈도우의 모양, 기능들을 묶어놓은 개념 집합
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
//// #1-1. < 윈도우 클래스 등록 >
////		=> 등록된 윈도우 클래스를 통해 윈도우를 찍어낼 수 있다.
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(nullptr, L"Failed to register Class!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2. 윈도우 생성
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
//// #2-1. 윈도우 생성 확인
////		=> CreateWindowEx함수를 통해 윈도우 포인터를 받지 못하였을 경우
//	if (hWnd == nullptr)
//	{
//		MessageBox(nullptr, L"Failed to Create Window!", L"Error", MB_OK);
//		return 0;
//	}
//
//// #2-2. 윈도우가 정상적으로 만들어 졌을 경우
////		=> 화면에 윈도우를 보여주고 윈도우를 갱신( 다시 그리기 )해준다.
//	ShowWindow(hWnd, nShowCmd);
//	UpdateWindow(hWnd);
//
//// #3. 메시지 루프
////		=> 운영체제에 메세지가 있는지 물어보고 있으면 가져온다.
////		=> 메세지가 없을 때까지 이 과정을 반복한다.
//	MSG msg;
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//// #. 윈도우에서 실행되는 모든 App에는 '메세지 큐'라는 것이 있다.
//// #. 운영체제는 계속해서 수십개의 메세지를 전달할 것이다. 그럼 이 메세지를 쌓아 놓고 하나씩 처리할 필요가 있다.
//// #. 이 메세지 큐에 메세지를 담아 놓고 하나씩 처리할 것이다.
//// #. 이 작업은 프로그램이 종료될때 까지 계속해서 반복한다.
//		TranslateMessage(&msg);
//// #. 메세지 큐에 메세지가 들어올 때는 운영체제에서 사용하는 메세지가 들어온다.
//// #. 이 메세지를 앱에서 그대로 사용하기에는 규격이 맞지 않는다.
//// #. 따라서 TranslateMessage()함수를 이용해서 번역하는 작업을 먼저 실행한다.
//// #. 이 작업을 통해서 앱에서 사용할 수 있는 포맷으로 메세지를 바꿔준다.
//		DispatchMessage(&msg);
//// #. 앱에는 여러개의 윈도우가 있을 수 있다.
//// #. 포맷이 완료된 메세지를 모든 윈도우들에게 전달하는 일을 해주는 함수가 DispatchMessage()함수다.
//	}
//
//// #3-1. 프로그램 종료
////		=> wParam : 현재 앱의 상태를 담고있는 포인터
//	return static_cast<int>(msg.wParam);
//}
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//// #. DispatchMessage()함수를 거치고 나면 윈도우에 메세지가 전달된다.
//// #. 윈도우 핸들을 통해 전달된 메세지를 switch문을 통해서 처리한다.
//
//	switch (message)
//	{
//		case WM_KEYDOWN:
//		{
//			std::wostringstream oss;
//			oss << "Virtual Key = " << wParam << std::endl;
//			OutputDebugString(oss.str().c_str());
//// #. wParam에 저장된 가상 키보드의 키 값이 디버그 메시지 창에 출력된다.
//			break;
//		}
//		case WM_LBUTTONDOWN:
//		{
//			std::wostringstream oss;
//			oss << "X : " << LOWORD(lParam) << ", Y : " << HIWORD(lParam);
//			OutputDebugString(oss.str().c_str());
//// #. lParam에 저장된 x, y좌표를 디버그 메시지 창에 출력한다.
//			break;
//		}
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//// #. 관심이 있는 항목은 케이스 레이블을 만들어서 따로 처리해 준다.
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//// #. 일반적인 항목은 기본 처리장치를 통해서 처리해 준다.
//			break;
//	}
//	return 0;
//}

/* ------ < GDI( Graphics Device Interface ) > ----- */

/*
< GDI > : 윈도우즈에 붙어있는 어떤 그래픽 장치에도 다 그림을 그릴 수 있다라는 개념

< Device Context > : 그림에 대한 정보들을 담아 놓은 장치
	#. < HDC > : DC의 핸들
	#. < GetDC() > : 어떤 장치에 그릴 것인지 지정하는 함수 HDC를 반환한다.

< 좌표계( Coordinates ) >
	#. Logical coordinates : 앱 윈도우 상의 좌표 지점
	#. Physical coordinates : 모니터 상의 좌표 지점

< PAINTSTRUCT 구조체 > : 그림을 그릴 영역이나 공간에 대한 정보를 가지고 있다.
	#. BOOL fErase : 지우느냐 않지우느냐, 즉 배경을 지울 것인지
	#. RECT rcPaint : RECT는 좌상단과 우하단의 좌표지점을 갖는 구조체이다.
		=> 어디에 그릴지 사각형 영역을 지정한다.

< Client Rect > : 실제로 그림이 그려지는 윈도우의 영역
< Window Rect > : 제목 표시줄과 종료, 최소화키 등이 포함된 윈도우 영역

< hatch > : 직선

< CreateSolidBrush > :
	#. < RGB > : HIWORD/ROWORD와 같은 개념으로 메모리 공간을 3등분 하여 원하는 값을 저장할 수 있다.
		=> GetRValue(rgb)/GetGValue(rgb)/GetBValue(rgb)함수를 통해 바이트로 형변환하여 잘라버리고 추출할 수도 있다.

< 그리는 과정 >
	#1. 필요한 오브젝트를 만든다.
	#2. 만들어진 오브젝트를 적용한다.
	#3. 그림을 그린다.
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
// #1. < 윈도우 클래스 생성 > : 윈도우 클래스라는 틀을 만들어 놓는다.
//		=> 윈도우의 모양, 기능들을 묶어놓은 개념 집합
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

// #1-1. < 윈도우 클래스 등록 >
//		=> 등록된 윈도우 클래스를 통해 윈도우를 찍어낼 수 있다.
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to Register Class!", L"Error", MB_OK);
		return 0;
	}

// #2. 윈도우 생성
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

// #2-1. 윈도우 생성 확인
//		=> CreateWindowEx함수를 통해 윈도우 포인터를 받지 못하였을 경우
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to Create Window!", L"Error", MB_OK);
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

// #. 그리기와 관련된 기능을 처리하는 함수를 만들어 준다.
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
// #. 어디에 어느 부분만큼 그릴지 지정한다.
// #. 그림을 시작하고 끝나는 라인을 지정한다.

	HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, blueBrush);
	Rectangle(hdc, 0, 0, 100, 100);
// #. 브러쉬 메모리 공간을 할당 받고 핸들로 저장한다.
// #. 생성한 브러쉬를 그리기에 사용하도록 SelectObject()함수로 등록해 준다.
// #. 사각형을 그릴때 등록한 브러쉬가 자동으로 사용된다.

	DeleteObject(blueBrush);
	EndPaint(hWnd, &ps);
}

// #4. 윈도우 프로시져
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
// #. 마우스 왼쪽 버튼을 누르면 사각형을 그리는 명령어
// #. hWnd를 전달하여 현재 만든 앱의 윈도우 화면에 그리도록 지정한다.
// #. Rectangle()함수를 통해 hdc에 사각형을 그려준다.
// #. GetDC로 인해 동적할당된 메모리를 ReleaseDC를 통해 동적해제한다.
// #. 어떤 윈도우에 어떤 DC핸들을 해제할 것인지 매개 변수로 전달한다.
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