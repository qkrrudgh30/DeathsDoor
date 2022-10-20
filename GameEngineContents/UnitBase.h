#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "GlobalContentsValue.h"







// ���� :
class UnitBase : public GameEngineActor
{
public:
	// constrcuter destructer
	UnitBase();
	~UnitBase();

	// delete Function
	UnitBase(const UnitBase& _Other) = delete;
	UnitBase(UnitBase&& _Other) noexcept = delete;
	UnitBase& operator=(const UnitBase& _Other) = delete;
	UnitBase& operator=(UnitBase&& _Other) noexcept = delete;

protected:
	GameEngineDefaultRenderer* Renderer;
	GameEngineCollision* Collision;
	GameEngineCollision* AttCollision;

	GameEngineStateManager StateManager;

public:
	UINFO	m_Info;
private:

};

