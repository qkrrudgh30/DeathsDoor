// #include "myshader.hlsli"

// SV_POSITION �ø�ƽ
// �׷���ī�忡�� �̳༮�� �̷� �η��ϱ� �ϰ� �ڵ����� ó���ϴ� �༮�� ������ �ϰ�.

// t1

// Create("TransformData");
#include "TransformHeader.fx"
#include "LightHeader.fx"

// cbuffer Color : 
// TestColor;
struct Input
{
    float4 Pos : POSITION;
    float4 Normal : NORMAL0;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL0;
};

//cbuffer ResultColor : register(b2)
//{
//    float4 PlusColor;
//    float4 MultyplyColor;
//}



// 1000
// 0100
// 2010
// 0301

// 1020
// 0103
// 0010
// 0001

Output Color_VS(Input _Input)
{
    // ���̴��� ��쿡�� ��κ��� ��Ȳ���� ����ȯ�� �����ϴ�.
    Output NewOutPut = (Output)0;
    
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    
    NewOutPut.ViewPos = mul(_Input.Pos, WorldView);
    
    _Input.Normal.w = 0.0f;
    NewOutPut.Normal = mul(_Input.Normal, WorldView);
    NewOutPut.Normal.w = 0.0f;
    
    // NewOutPut.Normal = _Input.Normal;
    
    return NewOutPut;
}

cbuffer ResultColor : register(b8)
{ 
    float4 Color;
}

float4 Color_PS(Output _Input) : SV_Target0
{
    //LightData Data;
    //Data.LightDir = float4(1.0f, 0.0f, 0.0f, 0.0f);
    //Data.LightRevDir = float4(-1.0f, 0.0f, 0.0f, 0.0f);
    //Data.LightRevDir = mul(Data.LightRevDir, WorldView);
    
    float4 DiffuseLight = CalDiffuseLights(_Input.Normal);
    float4 SpacularLight = CalSpacularLight(_Input.ViewPos, _Input.Normal);
    float4 AmbientLight = CalAmbientLight();
    
    
    // Color
    float4 LightEffectResult = Color * (DiffuseLight + SpacularLight + AmbientLight);
    LightEffectResult.w = 1.0f;
    
    return LightEffectResult;
}
