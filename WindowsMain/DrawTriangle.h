#pragma once

// #. Shader( High Level Shader Language ) -> Compile
//		=> 고급 셰이더 언어를 C 언어처럼 조금 더 셰이더를 구출할 수 있게 해주는 언어로, 컴파일 과정이 필요하다.
//		=> 컴파일된 HLSL은 어셈블리에 가깝게 변형되며 하드웨어에서 빠르게 처리된다.
#include <d3dcompiler.h>
#include "D3DFramework.h"

#pragma comment ( lib, "d3dcompiler.lib" )

// #. 삼각형 그리기 클래스
class DrawTriangle : public D3DFramework
{
	struct VERTEX	// 삼각형의 위치와 색상
	{
		FLOAT X, Y, Z;
		FLOAT Color[4];
	};

	// 버텍스 셰이더를 사용할 때 버텍스 데이터들을 어떻게 메모리에 담아서 Input-Assember 스테이지에 전달하는 지를 나타낸다.
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	mspInputLayout;
	// 실제 버텍스 정보를 담고 있는 버퍼
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mspVertexBuffer;
	// 버텍스 셰이더에 대한 인터페이스
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	mspVertexShader;
	// 필셀 셰이더 인터페이스
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	mspPixelShader;

public:
	void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
	void Destroy() override;

private:
	void InitTriangle();
	void InitPipeline();

protected:
	void Render() override;
};