Texture2D shaderTexture;
//	=> 세이더에서 사용하는 형식으로 텍스쳐 변수형을 의미한다. 1D, 2D, 3D등의 형식을 지정할 수 있다.
SamplerState Sampler
{	//	=> 샘플러 상태를 지정하는 형식, 샘플러는 텍스쳐의 텍셀이 도형에 어떻게 씌여지는 지를 지정할 수 있게 해준다.
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
//		=> 텍스쳐 오브젝트의 멤버 함수로 텍스쳐 샘플링을 수행해서 결과로 색상값을 돌려 준다.
}
//	이 픽셀 셰이더에서는 넘겨 받은 버텍스의 텍스쳐 좌표( u,v )를 샘플러를 적용해 최종 색상을 결정한다.