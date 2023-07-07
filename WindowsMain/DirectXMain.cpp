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

#include <windows.h>

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
// #. 다이렉트X는 기본 시스템이 아닌 추가 확장 시스템이다.
// #. 따라서 헤더와 라이브러리를 가져와야 한다.

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// 1. DirectX Factory 생성
// 2. RenderTarget 생성
// 3. 그리기( Rendering )
// 4. 리소스 해제

// #. ID2D1( Interface Direct 2D Version 1 )
// #. COM의 오브젝트들은 전부 클래스 내부에서 메모리를 관리한다.
// #. COM내부에서 메모리를 가져와서 쓰는 형태가 많기 떄문에 모든 인터페이스는 포인터로 정의해야 한다.
ID2D1Factory* gpD2DFactory{};
ID2D1HwndRenderTarget* gpRenderTarget{};

// #. 그리기 위한 도구를 다이렉트X 타입으로 만든다.
ID2D1SolidColorBrush* gpBrush{};
ID2D1RadialGradientBrush* gpRadialBrush{};

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    // #1-1. 팩토리를 만드는 도우미 함수 CreateFactory
    // #1-2. 팩토리 타입과 만들어진 팩토리에 대한 더블 포인터를 매개 변수로 전달한다.
    // #1-3. 반환은 HRESULT타입으로 반환한다.
    // #1-4. 이 도우미 함수를 통해 만들어진 Factory**를 gdD2DFactory포인터 변수로 받는다.
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory);
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to create D2D Factory!!!", L"Error", MB_OK);
        return 0;
    }

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

    // #2-1. 팩토리를 만들어 놓았기 때문에 다이렉트X와 관련된 모든것은 팩토리를 통해서 만들면 된다.
    // #2-2. 렌더타겟에 대한 속성( 렌더링 타입으로 CPU, GPU중 어떤 타입으로 할지, 픽셀 포맷이 어떤식인지,
    //      => 화면 해상도 조절, 렌더타겟의 사용처, 다이렉트X 최소버전 설치 요구 사항 )
    // #2-3. 두번째는 핸들과, 그릴 영역, V싱크를 할지 않할지
    GetClientRect(hwnd, &wr);
    hr = gpD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        &gpRenderTarget
    );
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to create Render Target!", L"Error", MB_OK);
        return 0;
    }

    // #3-2. 그리기와 관련된 것은 전부 렌더 타깃이 한다.
    hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &gpBrush);

    // #3-3. 그라데이션을 받은 변수를 만들고 색깔 포인트 지점을 지정해 준다.
    //      => 임의로 두 개의 지점을 지정해 주고 그 지점의 색깔과 위치를 지정해 준다.
    ID2D1GradientStopCollection* pStops{};
    D2D1_GRADIENT_STOP gradientStops[2];
    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
    gradientStops[0].position = 0.0f;
    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
    gradientStops[1].position = 1.0f;

    // #3-4. 그라데이션을 만들어 준다.
    //      => stop 배열을 주고, 배열의 개수를 주고, 만들어진 콜렉션을 전달해 준다.
    hr = gpRenderTarget->CreateGradientStopCollection(
        gradientStops,
        2,
        &pStops
    );

    // #3-5. 만들어진 그라데이션을 통해 브러쉬를 만든다.
    hr = gpRenderTarget->CreateRadialGradientBrush(
        D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 150), D2D1::Point2F(0, 0), 50, 50),
        pStops,
        &gpRadialBrush
    );

    if (pStops != nullptr)
    {
        pStops->Release();
        pStops = nullptr;
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // #4-1. 리소스 해제
    if (gpRadialBrush != nullptr)
    {
        gpRadialBrush->Release();
        gpRadialBrush = nullptr;
    }
    if (gpBrush != nullptr)
    {
        gpBrush->Release();
        gpBrush = nullptr;
    }
    if (gpRenderTarget != nullptr)
    {
        gpRenderTarget->Release();
        gpRenderTarget = nullptr;
    }
    if (gpD2DFactory != nullptr)
    {
        gpD2DFactory->Release();
        gpD2DFactory = nullptr;
    }

    return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
    HDC hdc;
    PAINTSTRUCT ps;
    hdc = BeginPaint(hwnd, &ps);

    // #3-1. 그리기 시작과 끝을 지정해 준다.
    //      => Clear : 배경을 깨끗하게 지운다는 의미로 지정한 색으로 지운다.
    //      => ColorF : 색을 float타입으로 지정한다.
    gpRenderTarget->BeginDraw();
    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    // #3-6. 사각형을 그리고 색칠한다.
    //      => 그릴 영역을 전달해 주고, 브러쉬를 지정해 준다.
    gpRenderTarget->FillRectangle(
        D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
        gpBrush
    );

    // #3-7. 타원형을 그리고 색칠한다.
    gpRenderTarget->FillEllipse(
        D2D1::Ellipse(D2D1::Point2F(50.0f, 150.0f), 50.0f, 50.0f),
        gpRadialBrush
    );
    gpRenderTarget->EndDraw();

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        OnPaint(hwnd);
        break;
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