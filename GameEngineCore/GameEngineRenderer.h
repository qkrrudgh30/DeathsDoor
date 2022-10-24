#pragma once
#include "GameEngineCamera.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"
#include "GameEngineMaterial.h"
#include "GameEngineShaderResourcesHelper.h"
#include "GameEngineMesh.h"

struct RenderOption 
{
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
	int Temp0;
	int Temp1;
};

// ����Ÿ���� ���õǾ� �ִٸ� �̳༮���� �׸����� �ִ�.

class GameEngineRenderUnit 
{
public:
	GameEngineRenderUnit();

	void SetMesh(const std::string& _Name);

	void SetPipeLine(const std::string& _Name);

	// �츮 ���������� �̷� �̸��� ����x�� ������ �̷��� �����ϱ�� �߾�.
	void EngineShaderResourcesSetting(GameEngineRenderer* _Renderer);

	void Render(float _DeltaTime);

	void SetRenderer(GameEngineRenderer* _Renderer);

	GameEngineMaterial* GetPipeLine();

	GameEngineMaterial* GetClonePipeLine();

	GameEngineMaterial* ClonePipeLine(GameEngineMaterial* _Rendering);

	GameEngineShaderResourcesHelper ShaderResources;

private:
	GameEngineRenderer* ParentRenderer;
	GameEngineMesh* Mesh; // �� �޽���
	GameEngineMaterial* PipeLine; // �� ��������
	GameEngineInputLayOut* InputLayOut; // ��ǲ�����1 ����
	// 	GameEngineShaderResourcesHelper ShaderResources; // �� �����͸� ������
	D3D11_PRIMITIVE_TOPOLOGY Topology;// �̷��� �׸���.
};


// �߻�Ŭ����
// ���� �� �ڽĵ��� �ϳ��� �������̽��� ���� �����Դϴ�.
// ���� :
class GameEngineMaterial;
class GameEngineShaderResourcesHelper;
class GameEngineRenderer : public GameEngineTransformComponent
{
	friend class GameEngineLevel;
	friend class GameEngineCamera;


public:
	RenderOption renderOption;

	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	// float4x4 ViewPort;
	void ChangeCamera(CAMERAORDER _Order);

    GameEngineMaterial* ClonePipeLine(GameEngineMaterial* _Rendering);

    inline int GetRenderingOrder() 
    {
        return RenderingOrder;
    }

    void SetRenderingOrder(int _Order);

	virtual void InstancingOn() 
	{
		IsInstancing_ = true;
	};

	bool IsInstancing(GameEngineMaterial* _Rendering);

	void InstancingDataSetting(GameEngineMaterial* _Line);

	void EngineShaderResourcesSetting(GameEngineShaderResourcesHelper* _ShaderResources);

	void PushRendererToMainCamera();

	void PushRendererToUICamera();
	
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}

    class GameEngineCamera* Camera;



private:
	CAMERAORDER CameraOrder;
    int RenderingOrder;
	bool IsInstancing_;

	virtual void Render(float _DeltaTime) = 0;

};

