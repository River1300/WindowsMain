#include "DrawTriangle.h"

// #. ����
// ������ ����
//	1. WNDCLASSEX ����ü�� ���� ����
//	2. RegisterClass(&wc)�� Ŭ���� ���
//	3. CreateWindow("class name")���� �ش� Ŭ������ ������ ����
//	4. ShowWindow()�� ȭ�鿡 ǥ��
//	5. ������ ���ν����� �޽��� ó��
// Direct3D ���� �� �׸���
//	1. ���� ü�� ���� ����ü ����
//	2. D3D11CreateDeviceAndSwapChain���� ����ü��, ����̽�, ����̽� ���ؽ�Ʈ ����
//	3. �ĸ� ���۸� ���ͼ� ����Ÿ�� �� ����
//	4. ���� ���ٽ� ���ۿ� �ؽ���2D ����
//	5. ���� ���ٽ� ���۷� ���� ���� ���ٽ� �� ����
//	6. 3��, 5�� �並 �̿��� ���� Ÿ�� ����
//	7. ����Ʈ ����
//	8. �ĸ� ���� Ŭ����
//	9. ���� ���ٽ� ���� Ŭ����
//	10. ���ӿ��� ����� �׸��� ��ƾ
//	11. ���� ü���� Present()
// ���������ΰ� ���̴�
//	1. ���̴� ������
//	2. ���ؽ� ���̴�, �ȼ� ���̴� ����
//	3. �Է� ���̾ƿ� ���� ���ε�
//	4. ���ؽ� ���� ����( MAP UNMAP )
//	5. �׸���
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