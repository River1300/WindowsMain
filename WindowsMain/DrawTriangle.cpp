#include <fstream>
#include <vector>
#include "DrawTriangle.h"

void DrawTriangle::InitTriangle()
{	// 텍스쳐 좌표 추가
	//	U : 0.0 -> 1.0
	//	V : 0.0 -> 1.0
	VERTEX vertices[]{
	{-0.45f,  0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.45f,  0.5f, 0.0f, 1.0f, 0.0f },
	{-0.45f, -0.5f, 0.0f, 0.0f, 1.0f },
	{ 0.45f, -0.5f, 0.0f, 1.0f, 1.0f }
	};

	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 4,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE
	);

	mspDevice->CreateBuffer(
		&bd,
		nullptr,
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);

	D3D11_MAPPED_SUBRESOURCE ms;
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);

	memcpy(ms.pData, vertices, sizeof(vertices));
	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);
}

void DrawTriangle::InitPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

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

	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// 버텍스 구조가 변경되었으므로 파이프라인에 넘겨줄 입력-레이아웃도 수정
	D3D11_INPUT_ELEMENT_DESC ied[]{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	mspDevice->CreateInputLayout(
		ied,
		2,
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		mspInputLayout.ReleaseAndGetAddressOf()
	);
	mspDeviceContext->IASetInputLayout(mspInputLayout.Get());
}

HRESULT DrawTriangle::CreateTextureFromBMP()
{
	// 1. 파일 열기
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	BITMAPFILEHEADER bmh;
	BITMAPINFOHEADER bmi;

	// 2. BITMAPFILEHEADER 읽기
	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));

	// 3. BITMAPINFOHEADER 읽기
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));

	if (bmh.bfType != 0x4D42)
	{
		return E_FAIL;
	}
	if (bmi.biBitCount != 32)
	{
		return E_FAIL;
	}

	std::vector<char> pPixels(bmi.biSizeImage);

	// 4. 픽셀로 건너뛰기
	file.seekg(bmh.bfOffBits);

	// 5. 비트맵 읽기
	int pitch = bmi.biWidth * (bmi.biBitCount / 8);
	for (int y = bmi.biHeight - 1; y >= 0; y--)
	{
		file.read(&pPixels[y * pitch], pitch);
	}

	file.close();

	// D3D11_TEXTURE2D_DESC 구조체의 헬퍼 구조체
	CD3D11_TEXTURE2D_DESC tex_desc(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		bmi.biWidth,
		bmi.biHeight,
		1,
		1
	);

	// 하위 리소스를 초기화 할 때 사용하는 정보에 대한 구조체, 리소스는 여러 개의 하위 리소스로 구분외어 있으므로 하위 리소스 개념을 사용
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &pPixels[0];	// 초기화 데이터에 대한 포인터
	initData.SysMemPitch = pitch;				// 다음 라인까지의 바이트 거리
	initData.SysMemSlicePitch = 0;				// 다음 단계 까지의 바이트 거리

	// 텍스쳐를 생성, 함수의 두번째 파라미터는 초기값, 위에서 생성한 하위 리소스 데이터를 넘겨주면 그 데이터로 텍스쳐를 생성
	//		=> 즉, BMP파일의 픽셀로 텍스쳐를 생성하는 것
	mspDevice->CreateTexture2D(&tex_desc, &initData, mspTexture.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC srv_desc(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		tex_desc.Format,
		0,
		1
	);

	mspDevice->CreateShaderResourceView(mspTexture.Get(), &srv_desc, mspTextureView.ReleaseAndGetAddressOf());

	return S_OK;
}

void DrawTriangle::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitTriangle();

	// 초기화에서 텍스쳐 로딩 함수를 불러 주고, 종료할 때 인터페이스를 해제해 준다.
	CreateTextureFromBMP();
}

void DrawTriangle::Destroy()
{
	mspTextureView.Reset();
	mspTexture.Reset();

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

	mspDeviceContext->IASetVertexBuffers(
		0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset
	);
	mspDeviceContext->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
	// 셰이더 리소스 배열을 픽셀 셰이더 스테이지로 바인딩
	mspDeviceContext->PSSetShaderResources(0, 1, mspTextureView.GetAddressOf());
	mspDeviceContext->Draw(4, 0);
}