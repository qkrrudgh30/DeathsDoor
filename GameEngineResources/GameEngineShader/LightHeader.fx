struct LightData
{
    float4 LightColor;
    
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
    
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
};

cbuffer LightDatas : register(b13)
{
    int LightCount;
    LightData Lights[128];
};

// ������ 3���� ������ �ֽ��ϴ�.
// ���ݻ籤 Diffuse ���� ���⿡ ���� ǥ�������� ��� �����ɰų�?
// ���ݻ籤 Specular ���� �ݻ�Ȱ� ������ �󸶳� ��������.


// ȯ�汤 ���� ������ ���� ǥ���Ϸ��� ȯ�汤�� ����� ����ؾ� �ϴµ�.
// ����� �Ұ��ɿ� ������ ������ ��迡 ����� �������� ó���ϰų�.
// �׳� ����� �־�����µ�. �������� �����

// StructuredBuffer<InstTransformData> AllInstancingTransformData : register(t12);

float4 CalDiffuseLight(float4 _ViewNormal, LightData _LightData)
{
    _ViewNormal = normalize(_ViewNormal);
    float4 LightRevDir = normalize(_LightData.ViewLightRevDir);
    
    float4 DiffuseLight = max(0.0f, dot(_ViewNormal.xyz, LightRevDir.xyz));
    DiffuseLight *= _LightData.LightColor;
    DiffuseLight.w = 1.0f;
    
    return DiffuseLight;
}

float4 CalDiffuseLights(float4 _ViewNormal)
{
    float4 DiffuseLights = (float4) 0.0f;
    
    for (int i = 0; i < LightCount; ++i)
    {
        DiffuseLights += CalDiffuseLight(_ViewNormal, Lights[i]);
    }
    
    //if (DiffuseLights.r > 1.0f)
    //{
    //    DiffuseLights.r = 1.0f;
    //}
    
    //if (DiffuseLights.g > 1.0f)
    //{
    //    DiffuseLights.g = 1.0f;
    //}
    
    //if (DiffuseLights.b > 1.0f)
    //{
    //    DiffuseLights.b = 1.0f;
    //}
    
    DiffuseLights.w = 1.0f;
    return DiffuseLights;

}