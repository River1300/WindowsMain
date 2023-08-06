Texture2D shaderTexture;
SamplerState Sampler;
//		=> 텍스쳐 매핑 방식이나 필터링은 상황에 따라 수정해서 사용해야 한다.
//		=> 메시에 따라 매핑 방식이 다르기도 하고, 바닥용 텍스쳐는 조금 더 필터링에 신경써줘야 위신호(aliasing)가 발생하지 않는다.
//		=> 하지만, 이런 정의가 셰이더 코드에 들어 있으면 제어하기 힘들다.
//		=> 그러므로 우선 픽셀셰이더에서 샘플러는 선언만 한다.

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

	if (avg > 0.25f)
	{
		result[0] = 1.0f;
		result[1] = 1.0f;
		result[2] = 1.0f;
	}
	else
	{
		result[0] = 0.0f;
		result[1] = 0.0f;
		result[2] = 0.0f;
	}

	return result;
}