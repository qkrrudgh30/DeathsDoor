#include "PreCompile.h"
#include "GameEngineTransformComponent.h"

GameEngineTransformComponent::GameEngineTransformComponent() 
{
}

GameEngineTransformComponent::~GameEngineTransformComponent() 
{
}

void GameEngineTransformComponent::DetachObject() 
{
	// ������Ʈ���� ������
	GameEngineUpdateObject::DetachObject();

	// Ʈ���������� ������.
	GetTransform().DetachTransform();
}

void GameEngineTransformComponent::SetParent(std::shared_ptr<GameEngineUpdateObject> _Object)
{
	GameEngineUpdateObject::SetParent(_Object);

	{
		std::shared_ptr<GameEngineTransformBase> Actor = nullptr;
		if (Actor = std::dynamic_pointer_cast<GameEngineTransformBase>(_Object))
		{
			GetTransform().SetParentTransform(Actor->GetTransform());
			return;
		}
	}

	MsgBoxAssert("Ʈ�������� ���� ������Ʈ�� Ʈ�������� �ִ� �θ� ���̷��� �߽��ϴ�.");
}