Texture2D shaderTexture;
//	=> ���̴����� ����ϴ� �������� �ؽ��� �������� �ǹ��Ѵ�. 1D, 2D, 3D���� ������ ������ �� �ִ�.
SamplerState Sampler
{	//	=> ���÷� ���¸� �����ϴ� ����, ���÷��� �ؽ����� �ؼ��� ������ ��� �������� ���� ������ �� �ְ� ���ش�.
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct PIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PIn input) : SV_TARGET
{
	return shaderTexture.Sample(Sampler, input.tex);
//		=> �ؽ��� ������Ʈ�� ��� �Լ��� �ؽ��� ���ø��� �����ؼ� ����� ������ ���� �ش�.
}
//	�� �ȼ� ���̴������� �Ѱ� ���� ���ؽ��� �ؽ��� ��ǥ( u,v )�� ���÷��� ������ ���� ������ �����Ѵ�.