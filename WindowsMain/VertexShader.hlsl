// #. Transform
//		=> 하드웨어 가속의 핵심은 Transformation & Lighting (T&L)이다.
// Shader Constant Buffer
//		=> 지금까지 셰이더에 좌표, 텍스쳐 좌표, 텍스쳐 리소스등을 넘겨줬는데, 하드웨어 가속을 적용하기 위해서는 행렬을 넘겨줘서
//		=> 각 버텍스 별로 버텍스 셰이더에서 연산을 수행해야 한다.
//		=> 행렬은 버퍼 형태로 메모리에 저장되는데 CPU와 GPU가 직접 꺼내다 사용해야 한다. CPU는 매번 업데이트할 때마다 행렬의
//		=> 최종값을 생산해 낼 것이고 GPU는 버텍스 셰이더 단계에서 이 행렬을 가져와 버텍스의 최종 위치를 구하게 된다.
//		=> 즉 CPU는 매우 자주 버퍼(행렬)를 업데이트 할 것이고, CPU는 빠른 접근이 가능해야 한다.
//		=> HLSL Model 4 부터는 상수 버퍼라는 개념이 생겨서 이러한 접근 빈도에 최적화된 데이터를 관리할 수 있다.
// DirectXMath
//		=> 3D게임의 생명은 빠른 수학연산, 오브젝트의 이동, 회전, 스케일은 물론이고 화면에 표시할 픽셀의 색상을 결정하기까지 수
//		=> 많은 연산들이 사용된다.
//		=> DirectXMath는 이런 수학 연산을 보다 빠르게 처리하기 위해 SIMD( Single Instruction Multiple Data )와 유사한 데이터 타입들과
//		=> 함수들을 가지고 있다.
// 행렬 순서
//		=> 행렬은 4 * 4 형태의 2차원 배열의 모습을 가지고 있다. DirectXMath 및 DirectX는 열 기준 행렬을 사용한다.
//		=> 벡터 역시 열이 하나인 행렬이다.
//		=> 하지만 HLSL에서는 행 기준을 사용한다.
//		=> 벡터는 행이 하나일 행렬이다.
//		=> 이런 특성은 좌표계에서 기인하는데, 왼존 좌표계 시스템은 이런 열 기준 행렬을 사용해 왔으면 DirectX 역시 이런 표기를
//		=> 사용한다. 하지만 HLSL은 하드웨어에서 작동하도록 설계되어 있고 많은 하드웨어는 마이크로 소프트만 주로 사용하는
//		=> 좌표계가 아닌 오른손 좌표계를 사용한다. 따라서 DirectXMath를 사용할 때 주의할 점은 셰이더에 행렬을 넘겨줄 때는
//		=> 행과 열이 바뀐 전치행렬을 넘겨줘야 한다는 점이다.

cbuffer MatrixBuffer	// 상수 버퍼를 지정
{
	matrix worldMatrix;	// HLSL 에서 사용하는 matrix 타입의 변수를 선언
};

struct POut
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

POut main(float4 pos : POSITION, float4 tex : TEXCOORD0)
{
	POut output;

	output.pos = mul(pos, worldMatrix);
	output.tex = tex;

	return output;
}