#pragma once

// #. Shader( High Level Shader Language ) -> Compile
//		=> ��� ���̴� �� C ���ó�� ���� �� ���̴��� ������ �� �ְ� ���ִ� ����, ������ ������ �ʿ��ϴ�.
//		=> �����ϵ� HLSL�� ������� ������ �����Ǹ� �ϵ����� ������ ó���ȴ�.
#include <d3dcompiler.h>
#include "D3DFramework.h"

#pragma comment ( lib, "d3dcompiler.lib" )

// #. �ﰢ�� �׸��� Ŭ����
class DrawTriangle : public D3DFramework
{
	struct VERTEX	// �ﰢ���� ��ġ�� ����
	{
		FLOAT X, Y, Z;
		FLOAT Color[4];
	};

	// ���ؽ� ���̴��� ����� �� ���ؽ� �����͵��� ��� �޸𸮿� ��Ƽ� Input-Assember ���������� �����ϴ� ���� ��Ÿ����.
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	mspInputLayout;
	// ���� ���ؽ� ������ ��� �ִ� ����
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mspVertexBuffer;
	// ���ؽ� ���̴��� ���� �������̽�
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	mspVertexShader;
	// �ʼ� ���̴� �������̽�
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