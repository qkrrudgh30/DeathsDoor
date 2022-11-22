#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineShaderResourcesHelper.h"

// ���� :
class GameEngineMaterial;
class GameEngineDefaultRenderer 
	: public GameEngineRenderer
{
	// �ϳ��� ������ ������������
	// �ϳ��� �Ž�
	// �ϳ��� ���׸���

	// �ϳ��� ����Ÿ��


public:

	// GameEngineShaderResourcesHelper ShaderResources;

	GameEngineDefaultRenderer();
	~GameEngineDefaultRenderer();

	// delete Function
	GameEngineDefaultRenderer(const GameEngineDefaultRenderer& _Other) = delete;
	GameEngineDefaultRenderer(GameEngineDefaultRenderer&& _Other) noexcept = delete;
	GameEngineDefaultRenderer& operator=(const GameEngineDefaultRenderer& _Other) = delete;
	GameEngineDefaultRenderer& operator=(GameEngineDefaultRenderer&& _Other) noexcept = delete;

	virtual void Render(float _DeltaTime);


	void SetMesh(const std::string& _Name);
	void SetPipeLine(const std::string& _Name);

	inline GameEngineShaderResourcesHelper& GetShaderResources()
	{
		return Unit->ShaderResources;
	}

	inline std::shared_ptr<GameEngineRenderUnit> GetRenderUnit()
	{
		return Unit;
	}

protected:

	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}

private:
	std::shared_ptr<GameEngineRenderUnit> Unit;

	// �������� �޸� �����ϴ� �޷θ����� ��� �ְ�
	// GameEngineMaterial* PipeLine;


};

