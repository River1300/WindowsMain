#include "DrawTriangle.h"

void DrawTriangle::InitTriangle()
{
	VERTEX vertices[]{
	{  0.0f,  0.5f, 0.0f, {1.0f,0.0f,0.0f,1.0f} },
	{ 0.45f, -0.5f, 0.0f, {0.0f,1.0f,0.0f,1.0f} },
	{-0.45f, -0.5f ,0.0f, {0.0f,0.0f,1.0f,1.0f} }
	};
	// ���ؽ� ���̴����� �ƹ��͵� �� �ϱ� ������ ����� ���ؽ��� ��ġ�� 3D ������ ��ǥ�� �ƴ϶�, ȭ�鿡 ���� ��ǥ�̴�.
	//		=> �� ���� �ǹ��ϴ� ���� �˱� ���ؼ��� ���� ��ǥ���� ���� �˾ƺ��� �Ѵ�.

		// ����(VRAM)�� ����
	//		=> ���� ���ؽ� ������ ����ü�� �׸� ���� ���ؽ� ���۸� �غ��Ѵ�.
	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 3,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D10_CPU_ACCESS_WRITE
	);
	//		=> ���ؽ� ���۸� �ǳ� �� ���ۿ� ���� ���� ����ü, C�� �پ� �����Ƿ� D3D11_BUFFER_DESC�� ���� �ʱ�ȭ �ϴ� ���

	mspDevice->CreateBuffer(
		&bd,
		nullptr,
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);
	//		=> ������ ������ ���� �������� ���۸� ����, �ι�° �Ķ���ʹ� �ʱ�ȭ ������, NULL�� �Ѱ��ָ� 0���� �� ������ Ȯ���Ѵ�.

		// ����(RAM)�� ���۷� ����(VRAM)
	//		=> CreateBuffer�� �Բ� �ʱ�ȭ ���� �Ѱ����� �ʾҴٸ� ���� ���� ����ü ���� ���۷� �����ؾ� �Ǵµ�,
	//			=> DYNAMIC���۴� �� ������ ���� Ư���ϰ� �����ؾ� �Ѵ�. CPU�� ���� ���� GPU�� �о� ���µ�, �� ������ ���� ����
	//			=> ������ ���� ���� ���� GPU�� �о� �� ���� �ְ�, �׷� �̻��� ����� ������ �ȴ�.
	//	�̸� ���� ���� ����ϴ� ���� MAP/UNMAP
	//		=> CPU���� ���� ������ MAP�� �� �ּ� GPU�� ���� ���ϵ��� �ϰ�, ���� �� ���� ���� GPU�� ������ �� �ֵ��� UNMAP�� �Ѵ�.
	D3D11_MAPPED_SUBRESOURCE ms;
	//		=> ���ε� ���� ���ҽ��� ���� ����ü, ���� ���ҽ��� ��ü ���ҽ��� �Ϻκ��� ���Ѵ�.
	//		=> ���� ��ü ���ҽ��� �����ϴ�. MAP�� ��������� �޾ƿ� ���̹Ƿ� Ư���� �ʱ�ȭ�� �ʿ� ����.
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);
	//		=> �Ѱ��� ���ҽ��� �����Ͽ� ���ε� ���� ���ҽ� ����ü�� ���� �ش�. �����ϸ� �� ���ҽ��� GPU�� �бⰡ �����ȴ�.

		// �� �������� ���ļ� �����ϰ� ���� �����ص� �Ǹ� �����Ѵ�. ���ε� ���긮�ҽ��� pData�� ���ؽ� ����ü���� �޸� �����Ѵ�.
	memcpy(ms.pData, vertices, sizeof(vertices));
	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);
	//	=> ���簡 �������� GPU�� �о���� ���ش�.
}

void DrawTriangle::InitPipeline()
{	// Binary Large OBject = Buffer
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// ���� ���̴��� �������ϴ� �Լ�
	D3DCompileFromFile(
		L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_4_0_level_9_3",
		0,
		0,
		spVS.GetAddressOf(),
		nullptr
	);
	D3DCompileFromFile(
		L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_4_0_level_9_3",
		0,
		0,
		spPS.GetAddressOf(),
		nullptr
	);

	// ��ӿ��� ���� ���ؽ� ���̴��� �ȼ� ���̴� �������̽��� ���Ѵ�.
	mspDevice->CreateVertexShader(
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		nullptr,
		mspVertexShader.ReleaseAndGetAddressOf()
	);
	mspDevice->CreatePixelShader(
		spPS->GetBufferPointer(),
		spPS->GetBufferSize(),
		nullptr,
		mspPixelShader.ReleaseAndGetAddressOf()
	);

	// ������ ���ο��� ���ؽ� ���̴��� �ȼ� ���̴� ���������� ���� ���̴����� �����Ѵ�.
	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// Input-Assember ���������� �Է� ��(Element, ���ؽ� ���̴�-> ���ؽ�, �ȼ� ���̴� -> �ȼ�)�ϳ��� � ������ �Ǿ� �ִ� ���� ����
	D3D11_INPUT_ELEMENT_DESC ied[]{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// �� ����ü ������� �Է� ���̾ƿ��� �����.
	mspDevice->CreateInputLayout(
		ied,
		2,
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		mspInputLayout.ReleaseAndGetAddressOf()
	);

	// Input-Assembler�� �Է� ���̾ƿ��� �����Ѵ�.
	mspDeviceContext->IASetInputLayout(mspInputLayout.Get());
}

void DrawTriangle::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitTriangle();
}

void DrawTriangle::Destroy()
{
	mspVertexBuffer.Reset();
	mspInputLayout.Reset();
	mspPixelShader.Reset();
	mspVertexShader.Reset();

	D3DFramework::Destroy();
}

void DrawTriangle::Render()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	// Input-Assembler�� ���ؽ� ���۸� �����Ѵ�. GPU�� �� ���ؽ� ���۸� �д´�.
	mspDeviceContext->IASetVertexBuffers(
		0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset
	);
	mspDeviceContext->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	mspDeviceContext->Draw(3, 0);
}