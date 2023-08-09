#include <fstream>
#include <vector>
#include "DrawTriangle.h"

void DrawTriangle::InitTriangle()
{
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

	bd = CD3D11_BUFFER_DESC(
		sizeof(MatrixBuffer),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DEFAULT
	);
	mspDevice->CreateBuffer(&bd, nullptr, mspConstantBuffer.ReleaseAndGetAddressOf());
	// 버텍스 셰이더 스테이지에서 사용할 상수 버퍼를 지정
	mspDeviceContext->VSSetConstantBuffers(0, 1, mspConstantBuffer.GetAddressOf());

	mX = mY = 0.0f;
	mRotationZ = 0.0f;
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

	float border[4]{ 0.0f,0.0f,0.0f,0.0f };
	CD3D11_SAMPLER_DESC sampler_desc(
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		1,
		D3D11_COMPARISON_ALWAYS,
		border,
		0,
		1
	);

	mspDevice->CreateSamplerState(&sampler_desc, mspSamplerState.ReleaseAndGetAddressOf());
	mspDeviceContext->PSSetSamplers(0, 1, mspSamplerState.GetAddressOf());

	// 블렌드 상태를 생성하기 위한 구조체
	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	mspDevice->CreateBlendState(&blend_desc, mspBlendState.ReleaseAndGetAddressOf());
	mspDeviceContext->OMSetBlendState(mspBlendState.Get(), nullptr, 0xffffffff);	// Output-Merger 스테이지에 사용할 블렌딩 상태를 생성
}

HRESULT DrawTriangle::CreateTextureFromBMP()
{
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	BITMAPFILEHEADER bmh;
	BITMAPINFOHEADER bmi;

	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));

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

	file.seekg(bmh.bfOffBits);

	int pitch = bmi.biWidth * (bmi.biBitCount / 8);
	int index{};
	char r{}, g{}, b{}, a{};
	for (int y{ bmi.biHeight - 1 }; y >= 0; y--)
	{
		index = y * pitch;
		for (int x{ 0 }; x < bmi.biWidth; x++)
		{
			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (static_cast<unsigned char>(r) == 30 &&
				static_cast<unsigned char>(g) == 199 &&
				static_cast<unsigned char>(b) == 250)
			{
				pPixels[index] = 0;
				pPixels[index + 1] = 0;
				pPixels[index + 2] = 0;
				pPixels[index + 3] = 0;
			}
			else
			{
				pPixels[index] = b;
				pPixels[index + 1] = g;
				pPixels[index + 2] = r;
				pPixels[index + 3] = a;
			}
			index += 4;
		}
	}

	file.close();

	CD3D11_TEXTURE2D_DESC tex_desc(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		bmi.biWidth,
		bmi.biHeight,
		1,
		1
	);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &pPixels[0];
	initData.SysMemPitch = pitch;
	initData.SysMemSlicePitch = 0;

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

	CreateTextureFromBMP();
}

void DrawTriangle::Destroy()
{
	mspBlendState.Reset();

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
	mspDeviceContext->PSSetShaderResources(0, 1, mspTextureView.GetAddressOf());

	MatrixBuffer mb;
	mb.world = DirectX::XMMatrixTranspose(mWorld);	// 전치행렬(행과 열을 바꿔서)을 만들어서 넘긴다.
	mspDeviceContext->UpdateSubresource(
		mspConstantBuffer.Get(), 0, nullptr, &mb, 0, 0
	);

	mspDeviceContext->Draw(4, 0);
}

void DrawTriangle::Update(float dt)
{
	if (mInput.IsKeyDown('Q'))
	{
		mRotationZ -= DirectX::XM_PI * dt;
	}
	else if (mInput.IsKeyDown('E'))
	{
		mRotationZ += DirectX::XM_PI * dt;
	}

	if (mInput.IsKeyDown(VK_LEFT))
	{
		mX -= 1.0f * dt;
	}
	else if (mInput.IsKeyDown(VK_RIGHT))
	{
		mX += 1.0f * dt;
	}

	if (mInput.IsKeyDown(VK_UP))
	{
		mY += 1.0f * dt;
	}
	else if (mInput.IsKeyDown(VK_DOWN))
	{
		mY -= 1.0f * dt;
	}

	if (mInput.IsKeyDown('1'))
	{
		mTimer.SetScale(1.0f);
	}
	if (mInput.IsKeyDown('2'))
	{
		mTimer.SetScale(2.0f);
	}
	if (mInput.IsKeyDown('3'))
	{
		mTimer.SetScale(3.0f);
	}

	mWorld = DirectX::XMMatrixIdentity();
	mWorld *= DirectX::XMMatrixRotationZ(mRotationZ);
	mWorld *= DirectX::XMMatrixTranslation(mX, mY, 0.0f);
}
// #. 행렬의 순서 통일
//		=> 우리가 넘겨주는 position 값은 벡터로 1차 배열이기 때문에 따로 전치하지 않는다. 즉 벡터는 행/열 기준으로
//		=> 변환이 자유롭다는 의미이다. 전치 행렬을 넘겨 주지 않고 버텍스 셰이더의 mul 순서를 바꾼다면 동일한 결과가 될 것이다.
//		=> 하지만 계산에 사용할 행렬을 여러 개 넘겨 줄 때는 이런 변환 계통을 통일시켜 줄 필요가 있다.
