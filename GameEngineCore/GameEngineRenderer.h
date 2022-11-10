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
	int IsAnimation = 0;
	int Temp1;
};

// ����Ÿ���� ���õǾ� �ִٸ� �̳༮���� �׸����� �ִ�.

class GameEngineRenderUnit 
	: public std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	GameEngineRenderUnit();

	GameEngineRenderUnit(const GameEngineRenderUnit& _Render);

	void SetMesh(const std::string& _Name);

	void SetMesh(std::shared_ptr<GameEngineMesh> _Mesh);

	void SetPipeLine(const std::string& _Name);

	// �츮 ���������� �̷� �̸��� ����x�� ������ �̷��� �����ϱ�� �߾�.
	void EngineShaderResourcesSetting(std::shared_ptr<GameEngineRenderer> _Renderer);

	void Render(float _DeltaTime);

	void SetRenderer(std::shared_ptr < GameEngineRenderer> _Renderer);

	std::shared_ptr < GameEngineMaterial> GetPipeLine();

	std::shared_ptr < GameEngineMaterial> GetClonePipeLine();

	std::shared_ptr < GameEngineMaterial> ClonePipeLine(std::shared_ptr<GameEngineMaterial> _Rendering);

	GameEngineShaderResourcesHelper ShaderResources;

private:
	std::weak_ptr<GameEngineRenderer> ParentRenderer;
	std::shared_ptr < GameEngineMesh> Mesh; // �� �޽���
	std::shared_ptr < GameEngineMaterial> PipeLine; // �� ��������
	std::shared_ptr < GameEngineInputLayOut> InputLayOut; // ��ǲ�����1 ����
	// 	GameEngineShaderResourcesHelper ShaderResources; // �� �����͸� ������
	D3D11_PRIMITIVE_TOPOLOGY Topology;// �̷��� �׸���.
};


// �߻�Ŭ����
// ���� �� �ڽĵ��� �ϳ��� �������̽��� ���� �����Դϴ�.
// ���� :
class GameEngineMaterial;
class GameEngineShaderResourcesHelper;
class GameEngineRenderer 
	: public GameEngineTransformComponent
{
	friend class GameEngineLevel;
	friend class GameEngineCamera;


public:
	RenderOption RenderOptionInst;

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

    std::shared_ptr<GameEngineMaterial> ClonePipeLine(std::shared_ptr<GameEngineMaterial> _Rendering);

    inline int GetRenderingOrder() 
    {
        return RenderingOrder;
    }

    void SetRenderingOrder(int _Order);

	virtual void InstancingOn() 
	{
		IsInstancing_ = true;
	};

	bool IsInstancing(std::shared_ptr<GameEngineMaterial> _Rendering);

	void InstancingDataSetting(std::shared_ptr<GameEngineMaterial> _Line);

	void EngineShaderResourcesSetting(GameEngineShaderResourcesHelper* _ShaderResources);

	void PushRendererToMainCamera();

	void PushRendererToUICamera();
	
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}

    std::weak_ptr<class GameEngineCamera> Camera;



private:
	CAMERAORDER CameraOrder;
    int RenderingOrder;
	bool IsInstancing_;

	virtual void Render(float _DeltaTime) = 0;

};

