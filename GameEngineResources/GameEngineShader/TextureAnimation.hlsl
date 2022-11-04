#include "TransformHeader.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct AniMat
{
    float4x4 Mat;
};

StructuredBuffer<AniMat> ArrAniMationMatrix : register(t11);

// �׷���ī�忡�� �̷����°�.
Output TextureAnimation_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    // �ڽ��� ���ð������� �ִϸ��̼��� ��Ű��
    NewOutPut.POSITION = mul(_Input.POSITION, ArrAniMationMatrix[_Input.BLENDINDICES[0]].Mat);
    NewOutPut.POSITION = mul(NewOutPut.POSITION, WorldViewProjection);
    NewOutPut.TEXCOORD = NewOutPut.TEXCOORD;
    
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState LINEARWRAP : register(s0);

float4 TextureAnimation_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(LINEARWRAP, _Input.TEXCOORD.xy);
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    return Color;
}
