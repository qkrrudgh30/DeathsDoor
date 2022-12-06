struct LightData
{
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
};

cbuffer LightDatas : register(b8)
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
    // N
    _ViewNormal = normalize(_ViewNormal);
    float4 LightRevDir = normalize(_LightData.LightRevDir);
    
    float4 DiffuseLight = max(0.0f, dot(_ViewNormal.xyz, LightRevDir.xyz));
    DiffuseLight.w = 1.0f;
    return DiffuseLight;
}