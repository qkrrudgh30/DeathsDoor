#include "LightHeader.fx"
#include "TransformHeader.fx"
// SV_POSITION �ø�ƽ
// �׷���ī�忡�� �̳༮�� �̷� �η��ϱ� �ϰ� �ڵ����� ó���ϴ� �༮�� ������ �ϰ�.


struct Input
{
    float4 POSITION : POSITION;
    float4 WEIGHT : BLENDWEIGHT0; // ���� ���������� ������ �󸶳� ������ �޴��Ŀ� ���� ����.
    int4 INDICES : BLENDINDICES0; // ���� ���������� ���� �ε��� ����
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 VPPOSITION : TEXCOORD;
};


// �׷���ī�忡�� �̷����°�.
Output Shadow_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    NewOutPut.POSITION = mul(_Input.POSITION, WorldViewProjection);
    NewOutPut.VPPOSITION = mul(_Input.POSITION, WorldViewProjection);
    
    return NewOutPut;
}


float4 Shadow_PS(Output _Input) : SV_Target0
{
    return float4(_Input.VPPOSITION.z / _Input.VPPOSITION.w, 0.0f, 0.0f, 1.0f);
}