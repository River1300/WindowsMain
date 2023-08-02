#include "DrawTriangle.h"

void DrawTriangle::InitTriangle()
{
	VERTEX vertices[]{
	{  0.0f,  0.5f, 0.0f, {1.0f,0.0f,0.0f,1.0f} },
	{ 0.45f, -0.5f, 0.0f, {0.0f,1.0f,0.0f,1.0f} },
	{-0.45f, -0.5f ,0.0f, {0.0f,0.0f,1.0f,1.0f} }
	};
	// 버텍스 셰이더에서 아무것도 안 하기 때문에 사용할 버텍스의 위치는 3D 공간의 좌표가 아니라, 화면에 찍힐 좌표이다.
	//		=> 각 점이 의미하는 값을 알기 위해서는 먼저 좌표계라는 것을 알아봐야 한다.

		// 버퍼(VRAM)를 만들어서
	//		=> 실제 버텍스 데이터 구조체와 그를 담을 버텍스 버퍼를 준비한다.
	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 3,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D10_CPU_ACCESS_WRITE
	);
	//		=> 버텍스 버퍼를 건네 줄 버퍼에 대한 설명 구조체, C가 붙어 있으므로 D3D11_BUFFER_DESC을 쉽게 초기화 하는 기능

	mspDevice->CreateBuffer(
		&bd,
		nullptr,
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);
	//		=> 위에서 생성한 버퍼 설명으로 버퍼를 생성, 두번째 파라미터는 초기화 데이터, NULL을 넘겨주면 0으로 빈 공간만 확보한다.

		// 변수(RAM)를 버퍼로 매핑(VRAM)
	//		=> CreateBuffer와 함께 초기화 값을 넘겨주지 않았다면 남은 것은 구조체 값을 버퍼로 복사해야 되는데,
	//			=> DYNAMIC버퍼는 이 과정을 조금 특이하게 진행해야 한다. CPU가 값을 쓰고 GPU가 읽어 가는데, 이 과정이 서로 맞지
	//			=> 않으면 값을 쓰는 도중 GPU가 읽어 갈 수도 있고, 그럼 이상한 결과가 나오게 된다.
	//	이를 막기 위해 사용하는 것이 MAP/UNMAP
	//		=> CPU에서 값을 쓸때는 MAP을 해 둬서 GPU가 읽지 못하도록 하고, 값을 다 쓰고 나면 GPU가 가져갈 수 있도록 UNMAP을 한다.
	D3D11_MAPPED_SUBRESOURCE ms;
	//		=> 매핑된 서브 리소스에 대한 구조체, 서브 리소스란 전체 리소스의 일부분을 말한다.
	//		=> 물론 전체 리소스도 가능하다. MAP의 결과값으로 받아올 것이므로 특별히 초기화는 필요 없다.
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);
	//		=> 넘겨준 리소스를 매핑하여 매핑된 서브 리소스 구조체를 돌려 준다. 성공하면 이 리소스에 GPU의 읽기가 금지된다.

		// 위 과정들을 거쳐서 안전하게 값을 복사해도 되면 복사한다. 매핑된 서브리소스의 pData에 버텍스 구조체들을 메모리 복사한다.
	memcpy(ms.pData, vertices, sizeof(vertices));
	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);
	//	=> 복사가 끝났으니 GPU가 읽어가도록 해준다.
}

void DrawTriangle::InitPipeline()
{	// Binary Large OBject = Buffer
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// 각각 셰이더를 컴파일하는 함수
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

	// 블롭에서 각각 버텍스 셰이더와 픽셀 셰이더 인터페이스를 구한다.
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

	// 파이프 라인에서 버텍스 셰이더와 픽셀 셰이더 스테이지에 각각 셰이더들을 지정한다.
	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// Input-Assember 스테이지의 입력 값(Element, 버텍스 셰이더-> 버텍스, 픽셀 셰이더 -> 픽셀)하나가 어떤 식으로 되어 있는 가를 지정
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

	// 위 구조체 정보들로 입력 레이아웃을 만든다.
	mspDevice->CreateInputLayout(
		ied,
		2,
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		mspInputLayout.ReleaseAndGetAddressOf()
	);

	// Input-Assembler에 입력 레이아웃을 연결한다.
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

	// Input-Assembler에 버텍스 버퍼를 연결한다. GPU는 이 버텍스 버퍼를 읽는다.
	mspDeviceContext->IASetVertexBuffers(
		0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset
	);
	mspDeviceContext->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	mspDeviceContext->Draw(3, 0);
}