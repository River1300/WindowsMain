struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
// �Ϲ� ���α׷��� ���� �ܼ��� ������ �ش� ������ � �뵵�� ���Ǵ� �� �� �� ����.
//		=> ���� Semantic(�ǹ�)�� �ο��ϴµ� �ش� ������ � �뵵�� ���Ǵ� �� ������ �ش�.
VOut main(float4 pos : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = pos;
	output.color = color;

	return output;
	// �Է� ���� ��ġ, ���� ���� �״�� ��ȯ
	//		=> �Ѱ��� ���� ������ 2���� ��鿡 ǥ�õ� ��ǥ(Transform�� ��ġ�� �ʴ� ��ǥ)��� �ǹ�
}