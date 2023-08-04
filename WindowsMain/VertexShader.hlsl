struct VOut
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};
// #. ȭ���� �簢���� �׸��� ���� ����
//		=> 3D ��ü�� ������ �̹����� �ؽ��Ķ� �θ��� ������ ����� Mapping�̶� �Ѵ�. ������ ���ؼ��� �ؽ��� ��ǥ U,V�� �ʿ��ϸ� �̷� �������� ���̴��� �Ѱ���� �Ѵ�.
//	1. ���ؽ� ���� ����( u ,v ���� )
//	2. �ؽ��� ����( ID3D11Texture2D )
//	3. �ȼ� ������ ����
//	4. �ؽ��Ŀ��� ���̴� ���ҽ� �� ����( ID3D11ShaderResourceView )
//	5. �Է� ���̾ƿ� ����( D3D11_INPUT_ELEMENT_DESC )
//	6. ������
//		=> ���̴� ���ҽ��� �ؽ��� �� ����

// ���ؽ� ���̴��� �ȼ� ���̴��� ����
VOut main(float4 pos : POSITION, float4 tex : TEXCOORD0)
{	// Texture Coordinate�� ����, �� �ؽ��Ŀ� �پ��� ������ ��� ���� ���� ä���� ����� ������ �ѱ� �� ������ ���� ù��° ä���� 0�̴�.
	VOut output;

	output.position = pos;
	output.tex = tex;

	return output;
}