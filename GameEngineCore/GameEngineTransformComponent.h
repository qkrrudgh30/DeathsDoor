#pragma once
#include "GameEngineComponent.h"
#include <GameEngineBase/GameEngineTransform.h>
#include <GameEngineCore/GameEngineActor.h>
#include "GameEngineTransformBase.h"

// ���� : ����ε� ������������ �������ϴ� ������Ʈ���� �̰� ��� �޴´�.
class GameEngineTransformComponent 
	: public GameEngineComponent
	, public GameEngineTransformBase
{
public:
	// constrcuter destructer
	GameEngineTransformComponent();
	~GameEngineTransformComponent();

	// delete Function
	GameEngineTransformComponent(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _Other) noexcept = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _Other) noexcept = delete;

	std::shared_ptr<GameEngineActor> GetActor()
	{
		return GetRoot<GameEngineActor>();
	}

	template<typename ParentType>
	std::shared_ptr<ParentType> GetActor()
	{
		return GetParent<ParentType>();
	}

	void DetachObject() override;

	void SetParent(std::shared_ptr<GameEngineUpdateObject> _Parent) override;

protected:


private:
};

