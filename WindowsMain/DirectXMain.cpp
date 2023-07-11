/* ----- * ----- < DirectX > ----- * ----- */

/*
< DXGI( Graphics Infrastructure ) > : 그래픽스와 관련된 기반 사항들을 관리해 주는 제일 상단에 있는 것을 말한다.
	#. 운영체제가 그래픽 드라이버를 통해서 정해진 인터페이스로 그래픽 카드를 제어한다.

< Direct2D > : 2D그래픽 기능

< Direct3D > : 3D그래픽 기능

< XInput > : PC & 콘솔 & 태블릿에서 사용하는 모든 입력을 처리해준다.

< DirectMusic > : 배경 음악

< DirectAudio > : 효과음

< DirectWrite > : 글꼴 텍스트

< DirectCompute > : GPU 연산
	#. 그래픽 카드는 소수점 연산에 특화되어있다.
	#. 그래서 코어가 여러개 있고 이 코어를 쉐이더로 동작하게 해주는 기능을 한다.

< Component Base > : OOP의 단점을 극복하기 위해 나온 방식
	#. < COM( Component Object Model ) > : 컴포넌트를 사용하여 오브젝트를 만드는 방식
		=> 예를 들어 컴퓨터를 구성하는 부품들, 즉 CPU, 메인보드, RAM, 그래픽카드 등등을 Component라고 하면
		=> 이 부품들을 통해 만들어진 컴퓨터를 Object라 할 수 있다.
		=> 이러한 개념으로 내부의 구성요소를 때엇다 붙혔다 업그레이드 할 수 있게 만든 개념이 COM이다.
		=> 단, C언어로 만들어져 있다.
		=> 인터페이스에 의해 실행되는 객체( 클래스 ) : C언어 스타일( 구조체 )의 클래스로 이루어져있다.
			=> IDrect3DDevice : 접두어 I가 붙어있는 형식으로 다이렉트X에 있는 인터페이스는 대부분 I 접두어가 붙어 있다.
		=> 모든 인터페이스는 고유한 ID를 가진다.
			=> IID_IDrirect3DDevice : Interface ID라는 약자
		=> C++ 객체는 없고 다양한 도우미 함수( Helper Functions )가 있다.
			=> 객체의 수명은 COM방식으로 관리한다.( new, delete를 프로그래머가 하지 않는다. )
				=> ComPtr
			=> 모든 COM함수는 HRESULT를 반환한다.( 결과에 대한 핸들 정보 )
				=> 32비트 정수로 성공/실패 + 기타 정보를 반환한다.

< Factory Pattern > : 특정한 오브젝트를 인스턴스화 하려면 정보( 생성자에 전달할 )가 필요하다.
	#. 생성자의 매개 변수로 전달되는 정보가 다양할 때 어떤 인스턴스를 생성할지 하위 클래스가 결정하는 패턴

< DirectX Factory > : 팩토리를 통해서 대부분의 다이렉트X와 관련된 기능을 찍어낼 수 있다.

< 포인터의 포인터( 더블 포인터 ) > : 값을 바꾸고자 하는 것이 포인터라면 포인터의 주소값을 받기 위해 포인터의 포인터를 사용한다.
	#. 모든 다이렉트X의 생성과 관련된 것은 포인터의 포인터를 받는다.
*/

//#include <windows.h>
//
//#include <d2d1.h>
//#pragma comment(lib, "d2d1.lib")
//// #. 다이렉트X는 기본 시스템이 아닌 추가 확장 시스템이다.
//// #. 따라서 헤더와 라이브러리를 가져와야 한다.
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// 1. DirectX Factory 생성
//// 2. RenderTarget 생성
//// 3. 그리기( Rendering )
//// 4. 리소스 해제
//
//// #. ID2D1( Interface Direct 2D Version 1 )
//// #. COM의 오브젝트들은 전부 클래스 내부에서 메모리를 관리한다.
//// #. COM내부에서 메모리를 가져와서 쓰는 형태가 많기 떄문에 모든 인터페이스는 포인터로 정의해야 한다.
//ID2D1Factory* gpD2DFactory{};
//ID2D1HwndRenderTarget* gpRenderTarget{};
//
//// #. 그리기 위한 도구를 다이렉트X 타입으로 만든다.
//ID2D1SolidColorBrush* gpBrush{};
//ID2D1RadialGradientBrush* gpRadialBrush{};
//
//int WINAPI WinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    // #1-1. 팩토리를 만드는 도우미 함수 CreateFactory
//    // #1-2. 팩토리 타입과 만들어진 팩토리에 대한 더블 포인터를 매개 변수로 전달한다.
//    // #1-3. 반환은 HRESULT타입으로 반환한다.
//    // #1-4. 이 도우미 함수를 통해 만들어진 Factory**를 gdD2DFactory포인터 변수로 받는다.
//    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory);
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create D2D Factory!!!", L"Error", MB_OK);
//        return 0;
//    }
//
//    HWND hwnd;
//    WNDCLASSEX wc;
//    ZeroMemory(&wc, sizeof(WNDCLASSEX));
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpszClassName = gClassName;
//    wc.hInstance = hInstance;
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//    wc.lpfnWndProc = WindowProc;
//    wc.cbSize = sizeof(WNDCLASSEX);
//    if (!RegisterClassEx(&wc))
//    {
//        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    RECT wr = { 0, 0, 1024, 768 };
//    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//    hwnd = CreateWindowEx(NULL,
//        gClassName,
//        L"Direct2D",
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        wr.right - wr.left,
//        wr.bottom - wr.top,
//        NULL,
//        NULL,
//        hInstance,
//        NULL);
//    if (hwnd == nullptr)
//    {
//        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    // #2-1. 팩토리를 만들어 놓았기 때문에 다이렉트X와 관련된 모든것은 팩토리를 통해서 만들면 된다.
//    // #2-2. 렌더타겟에 대한 속성( 렌더링 타입으로 CPU, GPU중 어떤 타입으로 할지, 픽셀 포맷이 어떤식인지,
//    //      => 화면 해상도 조절, 렌더타겟의 사용처, 다이렉트X 최소버전 설치 요구 사항 )
//    // #2-3. 두번째는 핸들과, 그릴 영역, V싱크를 할지 않할지
//    GetClientRect(hwnd, &wr);
//    hr = gpD2DFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
//        &gpRenderTarget
//    );
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create Render Target!", L"Error", MB_OK);
//        return 0;
//    }
//
//    // #3-2. 그리기와 관련된 것은 전부 렌더 타깃이 한다.
//    hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &gpBrush);
//
//    // #3-3. 그라데이션을 받은 변수를 만들고 색깔 포인트 지점을 지정해 준다.
//    //      => 임의로 두 개의 지점을 지정해 주고 그 지점의 색깔과 위치를 지정해 준다.
//    ID2D1GradientStopCollection* pStops{};
//    D2D1_GRADIENT_STOP gradientStops[2];
//    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
//    gradientStops[0].position = 0.0f;
//    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
//    gradientStops[1].position = 1.0f;
//
//    // #3-4. 그라데이션을 만들어 준다.
//    //      => stop 배열을 주고, 배열의 개수를 주고, 만들어진 콜렉션을 전달해 준다.
//    hr = gpRenderTarget->CreateGradientStopCollection(
//        gradientStops,
//        2,
//        &pStops
//    );
//
//    // #3-5. 만들어진 그라데이션을 통해 브러쉬를 만든다.
//    hr = gpRenderTarget->CreateRadialGradientBrush(
//        D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 150), D2D1::Point2F(0, 0), 50, 50),
//        pStops,
//        &gpRadialBrush
//    );
//
//    if (pStops != nullptr)
//    {
//        pStops->Release();
//        pStops = nullptr;
//    }
//
//    ShowWindow(hwnd, nShowCmd);
//    UpdateWindow(hwnd);
//
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    // #4-1. 리소스 해제
//    if (gpRadialBrush != nullptr)
//    {
//        gpRadialBrush->Release();
//        gpRadialBrush = nullptr;
//    }
//    if (gpBrush != nullptr)
//    {
//        gpBrush->Release();
//        gpBrush = nullptr;
//    }
//    if (gpRenderTarget != nullptr)
//    {
//        gpRenderTarget->Release();
//        gpRenderTarget = nullptr;
//    }
//    if (gpD2DFactory != nullptr)
//    {
//        gpD2DFactory->Release();
//        gpD2DFactory = nullptr;
//    }
//
//    return static_cast<int>(msg.wParam);
//}
//
//void OnPaint(HWND hwnd)
//{
//    HDC hdc;
//    PAINTSTRUCT ps;
//    hdc = BeginPaint(hwnd, &ps);
//
//    // #3-1. 그리기 시작과 끝을 지정해 준다.
//    //      => Clear : 배경을 깨끗하게 지운다는 의미로 지정한 색으로 지운다.
//    //      => ColorF : 색을 float타입으로 지정한다.
//    gpRenderTarget->BeginDraw();
//    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
//
//    // #3-6. 사각형을 그리고 색칠한다.
//    //      => 그릴 영역을 전달해 주고, 브러쉬를 지정해 준다.
//    gpRenderTarget->FillRectangle(
//        D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
//        gpBrush
//    );
//
//    // #3-7. 타원형을 그리고 색칠한다.
//    gpRenderTarget->FillEllipse(
//        D2D1::Ellipse(D2D1::Point2F(50.0f, 150.0f), 50.0f, 50.0f),
//        gpRadialBrush
//    );
//    gpRenderTarget->EndDraw();
//
//    EndPaint(hwnd, &ps);
//}
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_PAINT:
//        OnPaint(hwnd);
//        break;
//    case WM_CLOSE:
//        DestroyWindow(hwnd);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}

/* ----- * ----- < DirectX > ----- * ----- */

/*
< sin > : 시작 지점의 각도가 0도일때 0에서 시작하는 그래프
    => 0 -> 1 -> 0 -> -1

< cos > : 시작 지점의 각도가 0도일때 1에서 시작하는 그래프
    => 1 -> 0 -> -1 -> 0

< 좌표의 이동 예시 > : 50 ~ 100까지 그림을 이동 시킨다.
    #. 기본적으로 sin은 -1에서 1을 반복한다.
    #. 이동의 좌표 를 0으로 바꾼다면 -50을 하여 0 ~ 50이 된다.
    #. sin은 1을 더하면 0 ~ 2까지가 된다.
    #. 그러므로 *25를 sin에 해주면 -25 ~ 25가 된다.
    #. 여기서 75를 sin에 추가로 더해 주면 50 ~ 100이 된다.( sin * 25 + 75 )

< 중심 > : 반복되는 운동에서 가장 중요한 것은 중심이다.
    #. 움직이는 폭을 곱해주고 ( *25 ) 50 ~ 75 ~ 100
    #. 중심을 더해 준다.

< GetMessage() >
    #. 운영체제에서 메시지를 가져옴
    #. 메시지가 없을 때는 함수 내부에서 대기

< PeekMessage() >
    #. 메시지가 있나 없나 살펴봄
    #. 메시지가 있을 때는 가져오고 없으면 바로 끝
*/

//#include <windows.h>
//
//#include <math.h>
//// #. 그림을 움직이기 위해 수학을 이용할 예정이다.
//
//#include <d2d1.h>
//#pragma comment(lib, "d2d1.lib")
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// #3. 그리기에 대한 전방 선언을 해준다.
//void OnPaint(HWND hwnd);
//
//// 1. DirectX Factory 생성
//// 2. RenderTarget 생성
//// 3. 그리기( Rendering )
//// 4. 리소스 해제
//
//ID2D1Factory* gpD2DFactory{};
//ID2D1HwndRenderTarget* gpRenderTarget{};
//
//ID2D1SolidColorBrush* gpBrush{};
//ID2D1RadialGradientBrush* gpRadialBrush{};
//
//int WINAPI WinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory);
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create D2D Factory!!!", L"Error", MB_OK);
//        return 0;
//    }
//
//    HWND hwnd;
//    WNDCLASSEX wc;
//    ZeroMemory(&wc, sizeof(WNDCLASSEX));
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpszClassName = gClassName;
//    wc.hInstance = hInstance;
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//    wc.lpfnWndProc = WindowProc;
//    wc.cbSize = sizeof(WNDCLASSEX);
//    if (!RegisterClassEx(&wc))
//    {
//        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    RECT wr = { 0, 0, 1024, 768 };
//    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//    hwnd = CreateWindowEx(NULL,
//        gClassName,
//        L"Direct2D",
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        wr.right - wr.left,
//        wr.bottom - wr.top,
//        NULL,
//        NULL,
//        hInstance,
//        NULL);
//    if (hwnd == nullptr)
//    {
//        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    GetClientRect(hwnd, &wr);
//    hr = gpD2DFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
//        &gpRenderTarget
//    );
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create Render Target!", L"Error", MB_OK);
//        return 0;
//    }
//
//    hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &gpBrush);
//
//    ID2D1GradientStopCollection* pStops{};
//    D2D1_GRADIENT_STOP gradientStops[2];
//    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
//    gradientStops[0].position = 0.0f;
//    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
//    gradientStops[1].position = 1.0f;
//
//    hr = gpRenderTarget->CreateGradientStopCollection(
//        gradientStops,
//        2,
//        &pStops
//    );
//
//    hr = gpRenderTarget->CreateRadialGradientBrush(
//        D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 150), D2D1::Point2F(0, 0), 50, 50),
//        pStops,
//        &gpRadialBrush
//    );
//
//    if (pStops != nullptr)
//    {
//        pStops->Release();
//        pStops = nullptr;
//    }
//
//    ShowWindow(hwnd, nShowCmd);
//    UpdateWindow(hwnd);
//
//    // #4. 무한 반복 while(true)
//    //      => 메시지 엿보기
//    //      => 있으면 가져와서 기존의 메시지 처리( PM_REMOVE )
//    //      => 없으면 그리기
//    MSG msg;
//    while (true)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//
//            if (msg.message == WM_QUIT)
//            {
//                break;
//            }
//        }
//        else
//        {
//            OnPaint(hwnd);
//        }
//    }
//
//    //    while (GetMessage(&msg, NULL, 0, 0))
//    //    {
//    //// #2. 지속적으로 그림을 그릴 수 있도록 반복문에 넣어 준다.
//    ////      => 단 윈도우 이벤트가 발생할 때만 움직이게 된다.
//    //        OnPaint(hwnd);
//    //        TranslateMessage(&msg);
//    //        DispatchMessage(&msg);
//    //    }
//
//    if (gpRadialBrush != nullptr)
//    {
//        gpRadialBrush->Release();
//        gpRadialBrush = nullptr;
//    }
//    if (gpBrush != nullptr)
//    {
//        gpBrush->Release();
//        gpBrush = nullptr;
//    }
//    if (gpRenderTarget != nullptr)
//    {
//        gpRenderTarget->Release();
//        gpRenderTarget = nullptr;
//    }
//    if (gpD2DFactory != nullptr)
//    {
//        gpD2DFactory->Release();
//        gpD2DFactory = nullptr;
//    }
//
//    return static_cast<int>(msg.wParam);
//}
//
//// #1. 그림을 그리는 일을 하는 운영체제가 윈도우이기 때문에 원할 떄 그림을 그리기 어렵다.
////      => OnPaint라는 기능이 계속 반복적으로 불려야만 한다.
//void OnPaint(HWND hwnd)
//{
//    HDC hdc;
//    PAINTSTRUCT ps;
//    hdc = BeginPaint(hwnd, &ps);
//
//    gpRenderTarget->BeginDraw();
//    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
//    gpRenderTarget->FillRectangle(
//        D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
//        gpBrush
//    );
//
//    // #. 원형 그림에 투명도를 줄 수 있다.
//    //      => 일반 브러쉬에서 투명도를 조정한다.
//    // #. 원형 그림을 움직이기 위해 변수를 만들어서 sin을 넘겨주는 각도를 저장한다.
//    //      => sin에 그래프에서는 각도를 증가시켜 주어야 한다.
//    // #. 라디안 : 파이는 원의 반바퀴 180도이다.
//    //      => 그럼 15도는 [파이 : 180 = x : 15] 이고 (180/15)*3.14 이다.
//    static float fAngle = 0.0f;
//    gpBrush->SetOpacity(0.5f);
//    gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
//    gpRenderTarget->FillEllipse(
//        D2D1::Ellipse(D2D1::Point2F(sin(fAngle) * 25 + 75.0f, 150.0f), 50.0f, 50.0f),
//        gpBrush
//    );
//    fAngle += 0.2f;
//    gpRenderTarget->EndDraw();
//
//    EndPaint(hwnd, &ps);
//}
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_PAINT:
//        OnPaint(hwnd);
//        break;
//    case WM_CLOSE:
//        DestroyWindow(hwnd);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}

/* ----- * ----- < DirectX > ----- * ----- */

/*
< Framework >
    #. COM Object( C Base -> Pointer = COM 내부에서 동적 할당, 해제는 직접 해야 함

< COM Smart Point > : ComPtr
    => Microsoft::WRL::ComPtr<T> == std::shared_ptr
*/

//#include <windows.h>
//#include <math.h>
//#include <d2d1.h>
//
//// #. ComPtr을 사용하기 위한 헤더 파일
//#include <wrl/client.h>
//
//#pragma comment(lib, "d2d1.lib")
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//void OnPaint(HWND hwnd);
//
//// #. 팩토리를 스마트 포인터로 바꿀 수 있다.
//Microsoft::WRL::ComPtr<ID2D1Factory> gpD2DFactory{};
//
//ID2D1HwndRenderTarget* gpRenderTarget{};
//ID2D1SolidColorBrush* gpBrush{};
//ID2D1RadialGradientBrush* gpRadialBrush{};
//
//int WINAPI WinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    // #. gpD2DFactory는 ComPtr클래스 이기 때문에 포인터의 주소값을 명확히 넘겨 주어야 한다.
//    //      => Get() : 포인터를 가져온다.
//    //      => GetAddressOf() : 포인터의 주소를 가져온다.
//    //      => ReleaseAndGetAddressOf() : 이미 인터페이스를 사용한 적이 있다면 해재한 뒤 다시 만든다.
//    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, gpD2DFactory.ReleaseAndGetAddressOf());
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create D2D Factory!!!", L"Error", MB_OK);
//        return 0;
//    }
//
//    HWND hwnd;
//    WNDCLASSEX wc;
//    ZeroMemory(&wc, sizeof(WNDCLASSEX));
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpszClassName = gClassName;
//    wc.hInstance = hInstance;
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//    wc.lpfnWndProc = WindowProc;
//    wc.cbSize = sizeof(WNDCLASSEX);
//    if (!RegisterClassEx(&wc))
//    {
//        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    RECT wr = { 0, 0, 1024, 768 };
//    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//    hwnd = CreateWindowEx(NULL,
//        gClassName,
//        L"Direct2D",
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        wr.right - wr.left,
//        wr.bottom - wr.top,
//        NULL,
//        NULL,
//        hInstance,
//        NULL);
//    if (hwnd == nullptr)
//    {
//        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//        return 0;
//    }
//
//    GetClientRect(hwnd, &wr);
//    hr = gpD2DFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
//        &gpRenderTarget
//    );
//    if (FAILED(hr))
//    {
//        MessageBox(nullptr, L"Failed to create Render Target!", L"Error", MB_OK);
//        return 0;
//    }
//
//    hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &gpBrush);
//
//    ID2D1GradientStopCollection* pStops{};
//    D2D1_GRADIENT_STOP gradientStops[2];
//    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
//    gradientStops[0].position = 0.0f;
//    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
//    gradientStops[1].position = 1.0f;
//
//    hr = gpRenderTarget->CreateGradientStopCollection(
//        gradientStops,
//        2,
//        &pStops
//    );
//
//    hr = gpRenderTarget->CreateRadialGradientBrush(
//        D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 150), D2D1::Point2F(0, 0), 50, 50),
//        pStops,
//        &gpRadialBrush
//    );
//
//    if (pStops != nullptr)
//    {
//        pStops->Release();
//        pStops = nullptr;
//    }
//
//    ShowWindow(hwnd, nShowCmd);
//    UpdateWindow(hwnd);
//
//    MSG msg;
//    while (true)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//
//            if (msg.message == WM_QUIT)
//            {
//                break;
//            }
//        }
//        else
//        {
//            OnPaint(hwnd);
//        }
//    }
//
//
//    if (gpRadialBrush != nullptr)
//    {
//        gpRadialBrush->Release();
//        gpRadialBrush = nullptr;
//    }
//    if (gpBrush != nullptr)
//    {
//        gpBrush->Release();
//        gpBrush = nullptr;
//    }
//    if (gpRenderTarget != nullptr)
//    {
//        gpRenderTarget->Release();
//        gpRenderTarget = nullptr;
//    }
//
//
//    return static_cast<int>(msg.wParam);
//}
//
//void OnPaint(HWND hwnd)
//{
//    HDC hdc;
//    PAINTSTRUCT ps;
//    hdc = BeginPaint(hwnd, &ps);
//
//    gpRenderTarget->BeginDraw();
//    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
//    gpRenderTarget->FillRectangle(
//        D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
//        gpBrush
//    );
//
//    static float fAngle = 0.0f;
//    gpBrush->SetOpacity(0.5f);
//    gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
//    gpRenderTarget->FillEllipse(
//        D2D1::Ellipse(D2D1::Point2F(sin(fAngle) * 25 + 75.0f, 150.0f), 50.0f, 50.0f),
//        gpBrush
//    );
//    fAngle += 0.2f;
//    gpRenderTarget->EndDraw();
//
//    EndPaint(hwnd, &ps);
//}
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_PAINT:
//        OnPaint(hwnd);
//        break;
//    case WM_CLOSE:
//        DestroyWindow(hwnd);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}

/* ----- * ----- < DirectX Framework > ----- * ----- */

#include <windows.h>
#include "D2DFramework.h"

const wchar_t gClassName[] = L"MyWindowClass";
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
D2DFramework myFramework;

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    HWND hwnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = gClassName;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProc;
    wc.cbSize = sizeof(WNDCLASSEX);
    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RECT wr = { 0, 0, 1024, 768 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd = CreateWindowEx(NULL,
        gClassName,
        L"Direct2D",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (hwnd == nullptr)
    {
        MessageBox(nullptr, L"Failed to create window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    try         // #. 기능 수행
    {
        myFramework.Init(hwnd);
    }
    catch (const com_exception& e)      // #. 예외 발생
    {
        // #8. com_exception으로 생성된 예외 사항들을 출력한다.
        wchar_t wstr[128];
        size_t len;
        mbstowcs_s(&len, wstr, e.what(), 128);
        MessageBox(nullptr, wstr, L"Error", MB_OK);
        // #. MBS : MultiByte String
        // #. To
        // #. WCS : Wide Character String
        // #. _s : safe( buffer overrun )
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            myFramework.Render();
        }
    }
    myFramework.Release();

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}