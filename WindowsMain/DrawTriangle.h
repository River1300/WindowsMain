#pragma once

#include <d3dcompiler.h>
#include "D3DFramework.h"

#pragma comment ( lib, "d3dcompiler.lib" )

class DrawTriangle : public D3DFramework
{
	struct VERTEX
	{
		FLOAT X, Y, Z;
		FLOAT U, V;	// 텍스쳐 좌표
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			mspInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				mspVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			mspVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			mspPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				mspTexture;		// 텍스쳐 인터페이스
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mspTextureView;	// 셰이더 리소스 뷰에 대한 인터페이스

public:
	void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
	void Destroy() override;

private:
	void InitTriangle();
	void InitPipeline();

	HRESULT CreateTextureFromBMP();	// BMP파일 로딩 기능을 사용해 텍스쳐를 만드는 함수

protected:
	void Render() override;
};