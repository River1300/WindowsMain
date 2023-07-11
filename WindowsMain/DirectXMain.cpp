/* ----- * ----- < DirectX > ----- * ----- */

/*
< DXGI( Graphics Infrastructure ) > : �׷��Ƚ��� ���õ� ��� ���׵��� ������ �ִ� ���� ��ܿ� �ִ� ���� ���Ѵ�.
	#. �ü���� �׷��� ����̹��� ���ؼ� ������ �������̽��� �׷��� ī�带 �����Ѵ�.

< Direct2D > : 2D�׷��� ���

< Direct3D > : 3D�׷��� ���

< XInput > : PC & �ܼ� & �º����� ����ϴ� ��� �Է��� ó�����ش�.

< DirectMusic > : ��� ����

< DirectAudio > : ȿ����

< DirectWrite > : �۲� �ؽ�Ʈ

< DirectCompute > : GPU ����
	#. �׷��� ī��� �Ҽ��� ���꿡 Ưȭ�Ǿ��ִ�.
	#. �׷��� �ھ ������ �ְ� �� �ھ ���̴��� �����ϰ� ���ִ� ����� �Ѵ�.

< Component Base > : OOP�� ������ �غ��ϱ� ���� ���� ���
	#. < COM( Component Object Model ) > : ������Ʈ�� ����Ͽ� ������Ʈ�� ����� ���
		=> ���� ��� ��ǻ�͸� �����ϴ� ��ǰ��, �� CPU, ���κ���, RAM, �׷���ī�� ����� Component��� �ϸ�
		=> �� ��ǰ���� ���� ������� ��ǻ�͸� Object�� �� �� �ִ�.
		=> �̷��� �������� ������ ������Ҹ� ������ ������ ���׷��̵� �� �� �ְ� ���� ������ COM�̴�.
		=> ��, C���� ������� �ִ�.
		=> �������̽��� ���� ����Ǵ� ��ü( Ŭ���� ) : C��� ��Ÿ��( ����ü )�� Ŭ������ �̷�����ִ�.
			=> IDrect3DDevice : ���ξ� I�� �پ��ִ� �������� ���̷�ƮX�� �ִ� �������̽��� ��κ� I ���ξ �پ� �ִ�.
		=> ��� �������̽��� ������ ID�� ������.
			=> IID_IDrirect3DDevice : Interface ID��� ����
		=> C++ ��ü�� ���� �پ��� ����� �Լ�( Helper Functions )�� �ִ�.
			=> ��ü�� ������ COM������� �����Ѵ�.( new, delete�� ���α׷��Ӱ� ���� �ʴ´�. )
				=> ComPtr
			=> ��� COM�Լ��� HRESULT�� ��ȯ�Ѵ�.( ����� ���� �ڵ� ���� )
				=> 32��Ʈ ������ ����/���� + ��Ÿ ������ ��ȯ�Ѵ�.

< Factory Pattern > : Ư���� ������Ʈ�� �ν��Ͻ�ȭ �Ϸ��� ����( �����ڿ� ������ )�� �ʿ��ϴ�.
	#. �������� �Ű� ������ ���޵Ǵ� ������ �پ��� �� � �ν��Ͻ��� �������� ���� Ŭ������ �����ϴ� ����

< DirectX Factory > : ���丮�� ���ؼ� ��κ��� ���̷�ƮX�� ���õ� ����� �� �� �ִ�.

< �������� ������( ���� ������ ) > : ���� �ٲٰ��� �ϴ� ���� �����Ͷ�� �������� �ּҰ��� �ޱ� ���� �������� �����͸� ����Ѵ�.
	#. ��� ���̷�ƮX�� ������ ���õ� ���� �������� �����͸� �޴´�.
*/

//#include <windows.h>
//
//#include <d2d1.h>
//#pragma comment(lib, "d2d1.lib")
//// #. ���̷�ƮX�� �⺻ �ý����� �ƴ� �߰� Ȯ�� �ý����̴�.
//// #. ���� ����� ���̺귯���� �����;� �Ѵ�.
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// 1. DirectX Factory ����
//// 2. RenderTarget ����
//// 3. �׸���( Rendering )
//// 4. ���ҽ� ����
//
//// #. ID2D1( Interface Direct 2D Version 1 )
//// #. COM�� ������Ʈ���� ���� Ŭ���� ���ο��� �޸𸮸� �����Ѵ�.
//// #. COM���ο��� �޸𸮸� �����ͼ� ���� ���°� ���� ������ ��� �������̽��� �����ͷ� �����ؾ� �Ѵ�.
//ID2D1Factory* gpD2DFactory{};
//ID2D1HwndRenderTarget* gpRenderTarget{};
//
//// #. �׸��� ���� ������ ���̷�ƮX Ÿ������ �����.
//ID2D1SolidColorBrush* gpBrush{};
//ID2D1RadialGradientBrush* gpRadialBrush{};
//
//int WINAPI WinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    // #1-1. ���丮�� ����� ����� �Լ� CreateFactory
//    // #1-2. ���丮 Ÿ�԰� ������� ���丮�� ���� ���� �����͸� �Ű� ������ �����Ѵ�.
//    // #1-3. ��ȯ�� HRESULTŸ������ ��ȯ�Ѵ�.
//    // #1-4. �� ����� �Լ��� ���� ������� Factory**�� gdD2DFactory������ ������ �޴´�.
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
//    // #2-1. ���丮�� ����� ���ұ� ������ ���̷�ƮX�� ���õ� ������ ���丮�� ���ؼ� ����� �ȴ�.
//    // #2-2. ����Ÿ�ٿ� ���� �Ӽ�( ������ Ÿ������ CPU, GPU�� � Ÿ������ ����, �ȼ� ������ �������,
//    //      => ȭ�� �ػ� ����, ����Ÿ���� ���ó, ���̷�ƮX �ּҹ��� ��ġ �䱸 ���� )
//    // #2-3. �ι�°�� �ڵ��, �׸� ����, V��ũ�� ���� ������
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
//    // #3-2. �׸���� ���õ� ���� ���� ���� Ÿ���� �Ѵ�.
//    hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &gpBrush);
//
//    // #3-3. �׶��̼��� ���� ������ ����� ���� ����Ʈ ������ ������ �ش�.
//    //      => ���Ƿ� �� ���� ������ ������ �ְ� �� ������ ����� ��ġ�� ������ �ش�.
//    ID2D1GradientStopCollection* pStops{};
//    D2D1_GRADIENT_STOP gradientStops[2];
//    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
//    gradientStops[0].position = 0.0f;
//    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
//    gradientStops[1].position = 1.0f;
//
//    // #3-4. �׶��̼��� ����� �ش�.
//    //      => stop �迭�� �ְ�, �迭�� ������ �ְ�, ������� �ݷ����� ������ �ش�.
//    hr = gpRenderTarget->CreateGradientStopCollection(
//        gradientStops,
//        2,
//        &pStops
//    );
//
//    // #3-5. ������� �׶��̼��� ���� �귯���� �����.
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
//    // #4-1. ���ҽ� ����
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
//    // #3-1. �׸��� ���۰� ���� ������ �ش�.
//    //      => Clear : ����� �����ϰ� ����ٴ� �ǹ̷� ������ ������ �����.
//    //      => ColorF : ���� floatŸ������ �����Ѵ�.
//    gpRenderTarget->BeginDraw();
//    gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
//
//    // #3-6. �簢���� �׸��� ��ĥ�Ѵ�.
//    //      => �׸� ������ ������ �ְ�, �귯���� ������ �ش�.
//    gpRenderTarget->FillRectangle(
//        D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
//        gpBrush
//    );
//
//    // #3-7. Ÿ������ �׸��� ��ĥ�Ѵ�.
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
< sin > : ���� ������ ������ 0���϶� 0���� �����ϴ� �׷���
    => 0 -> 1 -> 0 -> -1

< cos > : ���� ������ ������ 0���϶� 1���� �����ϴ� �׷���
    => 1 -> 0 -> -1 -> 0

< ��ǥ�� �̵� ���� > : 50 ~ 100���� �׸��� �̵� ��Ų��.
    #. �⺻������ sin�� -1���� 1�� �ݺ��Ѵ�.
    #. �̵��� ��ǥ �� 0���� �ٲ۴ٸ� -50�� �Ͽ� 0 ~ 50�� �ȴ�.
    #. sin�� 1�� ���ϸ� 0 ~ 2������ �ȴ�.
    #. �׷��Ƿ� *25�� sin�� ���ָ� -25 ~ 25�� �ȴ�.
    #. ���⼭ 75�� sin�� �߰��� ���� �ָ� 50 ~ 100�� �ȴ�.( sin * 25 + 75 )

< �߽� > : �ݺ��Ǵ� ����� ���� �߿��� ���� �߽��̴�.
    #. �����̴� ���� �����ְ� ( *25 ) 50 ~ 75 ~ 100
    #. �߽��� ���� �ش�.

< GetMessage() >
    #. �ü������ �޽����� ������
    #. �޽����� ���� ���� �Լ� ���ο��� ���

< PeekMessage() >
    #. �޽����� �ֳ� ���� ���캽
    #. �޽����� ���� ���� �������� ������ �ٷ� ��
*/

//#include <windows.h>
//
//#include <math.h>
//// #. �׸��� �����̱� ���� ������ �̿��� �����̴�.
//
//#include <d2d1.h>
//#pragma comment(lib, "d2d1.lib")
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// #3. �׸��⿡ ���� ���� ������ ���ش�.
//void OnPaint(HWND hwnd);
//
//// 1. DirectX Factory ����
//// 2. RenderTarget ����
//// 3. �׸���( Rendering )
//// 4. ���ҽ� ����
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
//    // #4. ���� �ݺ� while(true)
//    //      => �޽��� ������
//    //      => ������ �����ͼ� ������ �޽��� ó��( PM_REMOVE )
//    //      => ������ �׸���
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
//    //// #2. ���������� �׸��� �׸� �� �ֵ��� �ݺ����� �־� �ش�.
//    ////      => �� ������ �̺�Ʈ�� �߻��� ���� �����̰� �ȴ�.
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
//// #1. �׸��� �׸��� ���� �ϴ� �ü���� �������̱� ������ ���� �� �׸��� �׸��� ��ƴ�.
////      => OnPaint��� ����� ��� �ݺ������� �ҷ��߸� �Ѵ�.
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
//    // #. ���� �׸��� ������ �� �� �ִ�.
//    //      => �Ϲ� �귯������ ������ �����Ѵ�.
//    // #. ���� �׸��� �����̱� ���� ������ ���� sin�� �Ѱ��ִ� ������ �����Ѵ�.
//    //      => sin�� �׷��������� ������ �������� �־�� �Ѵ�.
//    // #. ���� : ���̴� ���� �ݹ��� 180���̴�.
//    //      => �׷� 15���� [���� : 180 = x : 15] �̰� (180/15)*3.14 �̴�.
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
    #. COM Object( C Base -> Pointer = COM ���ο��� ���� �Ҵ�, ������ ���� �ؾ� ��

< COM Smart Point > : ComPtr
    => Microsoft::WRL::ComPtr<T> == std::shared_ptr
*/

//#include <windows.h>
//#include <math.h>
//#include <d2d1.h>
//
//// #. ComPtr�� ����ϱ� ���� ��� ����
//#include <wrl/client.h>
//
//#pragma comment(lib, "d2d1.lib")
//
//const wchar_t gClassName[] = L"MyWindowClass";
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//void OnPaint(HWND hwnd);
//
//// #. ���丮�� ����Ʈ �����ͷ� �ٲ� �� �ִ�.
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
//    // #. gpD2DFactory�� ComPtrŬ���� �̱� ������ �������� �ּҰ��� ��Ȯ�� �Ѱ� �־�� �Ѵ�.
//    //      => Get() : �����͸� �����´�.
//    //      => GetAddressOf() : �������� �ּҸ� �����´�.
//    //      => ReleaseAndGetAddressOf() : �̹� �������̽��� ����� ���� �ִٸ� ������ �� �ٽ� �����.
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

    try         // #. ��� ����
    {
        myFramework.Init(hwnd);
    }
    catch (const com_exception& e)      // #. ���� �߻�
    {
        // #8. com_exception���� ������ ���� ���׵��� ����Ѵ�.
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