#include "DrawTriangle.h"

// #. 정리
// 윈도우 생성
//	1. WNDCLASSEX 구조체에 정보 설정
//	2. RegisterClass(&wc)로 클래스 등록
//	3. CreateWindow("class name")으로 해당 클래스에 윈도우 생성
//	4. ShowWindow()로 화면에 표시
//	5. 윈도우 프로시저로 메시지 처리
// Direct3D 생성 및 그리기
//	1. 스왑 체인 정보 구조체 설정
//	2. D3D11CreateDeviceAndSwapChain으로 스왑체인, 디바이스, 디바이스 컨텍스트 생성
//	3. 후면 버퍼를 얻어와서 렌더타겟 뷰 생성
//	4. 깊이 스텐실 버퍼용 텍스쳐2D 생성
//	5. 깊이 스텐실 버퍼로 부터 깊이 스텐실 뷰 생성
//	6. 3번, 5번 뷰를 이용해 렌더 타겟 지정
//	7. 뷰포트 지정
//	8. 후면 버퍼 클리어
//	9. 깊이 스텐실 버퍼 클리어
//	10. 게임에서 사용할 그리기 루틴
//	11. 스왑 체인의 Present()
// 파이프라인과 셰이더
//	1. 셰이더 컴파일
//	2. 버텍스 세이더, 픽셀 셰이더 생성
//	3. 입력 레이아웃 구조 바인딩
//	4. 버텍스 버퍼 생성( MAP UNMAP )
//	5. 그리기
// Device
//	=> CreateBuffer(), CreateVertexShader(), CreatePixelShader(), CreateInputLayout(), CreateRenderTargetView(), CreateTexture2D(), CreateDepthStencilView()
// Device Context
//	=> Map(), UnMap(), VSSetShader(), PSSetShader(), IASetInputLayout(), ClearRenderTargetVier(), ClearDepthStencilView(), IASetVertexBuffers(), IASetPrimitiveTopology(), Draw(), OMSetRenderTargets(), RSSetViewPorts()

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	DrawTriangle framework;

	framework.Initialize(hInstance);
	framework.GameLoop();
	framework.Destroy();

	return 0;
}