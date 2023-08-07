Texture2D shaderTexture;
SamplerState Sampler;
//BlendState Blending;
// #. Alpha Blending
//	배경을 제외하고 그려보기
//	알파 블렌딩은 Output Merger 스테이지에서 이뤄지며 최종 픽셀들끼리의 연산을 통해 결과물을 만들어 낸다.
//	기존 렌더타겟에 있는 픽셀과 현재 필셀 셰이더의 출력값을 연산을 통해 최종 픽셀의 색상을 구하게 된다.

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