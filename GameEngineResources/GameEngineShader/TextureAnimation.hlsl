#include "TransformHeader.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
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

// inout c++�� ġ�� ���۷���
// float4&
void Skinning(inout float4 _Pos, inout float4 _Weight, inout int4 _Index, StructuredBuffer<AniMat> _ArrMatrix)
{
    float4 CalPos = (float4)0.0f;
    _Pos.w = 1.0f;
    // 1�� ���߷��� �ൿ.
    // _Weight[3] = 1.0f - _Weight[0] - _Weight[1] - _Weight[2];
    
    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Pos, Mat.Mat);
    }
    
    _Pos = CalPos;
}

// �׷���ī�忡�� �̷����°�.
Output TextureAnimation_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    NewOutPut.POSITION = _Input.POSITION;
    Skinning(NewOutPut.POSITION, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
    NewOutPut.POSITION.w = 1.0f;
    // �ڽ��� ���ð������� �ִϸ��̼��� ��Ű��
    // NewOutPut.POSITION = mul(_Input.POSITION, ArrAniMationMatrix[_Input.BLENDINDICES[0]].Mat);
    NewOutPut.POSITION = mul(NewOutPut.POSITION, WorldViewProjection);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
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
