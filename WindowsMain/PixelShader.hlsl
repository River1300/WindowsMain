Texture2D shaderTexture;
SamplerState Sampler;
//BlendState Blending;
// #. Alpha Blending
//	����� �����ϰ� �׷�����
//	���� ������ Output Merger ������������ �̷����� ���� �ȼ��鳢���� ������ ���� ������� ����� ����.
//	���� ����Ÿ�ٿ� �ִ� �ȼ��� ���� �ʼ� ���̴��� ��°��� ������ ���� ���� �ȼ��� ������ ���ϰ� �ȴ�.

struct PIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PIn input) : SV_TARGET
{
	float4 result = shaderTexture.Sample(Sampler, input.tex);

	float avg = (result[0] + result[1] + result[2]) / 3;
	result[0] = result[1] = result[2] = avg;

	//if (avg > 0.25f)
	//{
	//	result[0] = 1.0f;
	//	result[1] = 1.0f;
	//	result[2] = 1.0f;
	//}
	//else
	//{
	//	result[0] = 0.0f;
	//	result[1] = 0.0f;
	//	result[2] = 0.0f;
	//}

	return result;
}