struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
// 일반 프로그래밍 언어에서 단순한 변수는 해당 변수가 어떤 용도로 사용되는 지 알 수 없다.
//		=> 따라서 Semantic(의미)을 부여하는데 해당 변수가 어떤 용도로 사용되는 지 지정해 준다.
VOut main(float4 pos : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = pos;
	output.color = color;

	return output;
	// 입력 받은 위치, 색상 값을 그대로 반환
	//		=> 넘겨줄 점은 실제로 2차원 평면에 표시될 좌표(Transform을 거치지 않는 좌표)라는 의미
}