float4 main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
// 입력 받은 색상을 그대로 반환한다. 즉 버텍스 정보에 지정한 색상으로 해당 픽셀을 채우는 단순한 방식
}