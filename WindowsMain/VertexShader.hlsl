struct VOut
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};
// #. 화면의 사각형에 그림을 입혀 보기
//		=> 3D 개체에 입히는 이미지는 텍스쳐라 부르며 입히는 방식을 Mapping이라 한다. 매핑을 위해서는 텍스쳐 좌표 U,V가 필요하며 이런 정보들을 셰이더로 넘겨줘야 한다.
//	1. 버텍스 정보 변경( u ,v 정보 )
//	2. 텍스쳐 생성( ID3D11Texture2D )
//	3. 픽셀 데이터 복사
//	4. 텍스쳐에서 셰이더 리소스 뷰 생성( ID3D11ShaderResourceView )
//	5. 입력 레이아웃 변경( D3D11_INPUT_ELEMENT_DESC )
//	6. 렌더링
//		=> 셰이더 리소스로 텍스쳐 뷰 지정

// 버텍스 셰이더와 픽셀 셰이더를 변경
VOut main(float4 pos : POSITION, float4 tex : TEXCOORD0)
{	// Texture Coordinate의 약자, 한 텍스쳐에 다양한 정보를 담기 위해 여러 채널을 등록해 정보를 넘길 수 있으며 가장 첫번째 채널이 0이다.
	VOut output;

	output.position = pos;
	output.tex = tex;

	return output;
}