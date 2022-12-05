
#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// �ڽ��� ��ó�� ���ִ� ��ġ�� ���
// ������Ʈ���� DeathUpdate���� death �ִϸ��̼��� ����Ǹ� �ڽ��� ��� �������� off
// LiveTime = 5.0f
// // dj

// ���� :
class Player;
class OldCrow : public UnitBase
{
public:
	// constrcuter destructer
	OldCrow();
	~OldCrow();

	// delete Function
	OldCrow(const OldCrow& _Other) = delete;
	OldCrow(OldCrow&& _Other) noexcept = delete;
	OldCrow& operator=(const OldCrow& _Other) = delete;
	OldCrow& operator=(OldCrow&& _Other) noexcept = delete;




protected:
	void Start() override;


	float4 StartPostion;
	float4 DirPower;


	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);
	void IdleStart(const StateInfo& _Info);


	void StartActStart(const StateInfo& _Info);
	void StartActEnd(const StateInfo& _Info);
	void StartActUpdate(float _DeltaTime, const StateInfo& _Info);



	void MoveReadyStart(const StateInfo& _Info);
	void MoveReadyEnd(const StateInfo& _Info);
	void MoveReadyUpdate(float _DeltaTime, const StateInfo& _Info);

	void MoveStart(const StateInfo& _Info);
	void MoveEnd(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);


	void TurnLStart(const StateInfo& _Info);
	void TurnLEnd(const StateInfo& _Info);
	void TurnLUpdate(float _DeltaTime, const StateInfo& _Info);

	void TurnRStart(const StateInfo& _Info);
	void TurnREnd(const StateInfo& _Info);
	void TurnRUpdate(float _DeltaTime, const StateInfo& _Info);

	void DashReadyStart(const StateInfo& _Info);
	void DashReadyEnd(const StateInfo& _Info);
	void DashReadyUpdate(float _DeltaTime, const StateInfo& _Info);

	void DashStart(const StateInfo& _Info);
	void DashEnd(const StateInfo& _Info);
	void DashUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpStart(const StateInfo& _Info);
	void JumpEnd(const StateInfo& _Info);
	void JumpUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpReadyStart(const StateInfo& _Info);
	void JumpReadyEnd(const StateInfo& _Info);
	void JumpReadyUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpEndStart(const StateInfo& _Info);
	void JumpEndEnd(const StateInfo& _Info);
	void JumpEndUpdate(float _DeltaTime, const StateInfo& _Info);


	void ScreamStart(const StateInfo& _Info);
	void ScreamEnd(const StateInfo& _Info);
	void ScreamUpdate(float _DeltaTime, const StateInfo& _Info);

	void DashDeathStart(const StateInfo& _Info);
	void DashDeathEnd(const StateInfo& _Info);
	void DashDeathUpdate(float _DeltaTime, const StateInfo& _Info);
	
	void StandDeathStart(const StateInfo& _Info);
	void StandDeathEnd(const StateInfo& _Info);
	void StandDeathUpdate(float _DeltaTime, const StateInfo& _Info);




	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;
private:
	void AniDashEnd(const GameEngineRenderingEvent& _Data);
	void AniDashStartEnd(const GameEngineRenderingEvent& _Data);
	void AniDeathRunEnd(const GameEngineRenderingEvent& _Data);
	void AniDeathStandEnd(const GameEngineRenderingEvent& _Data);
	void AniIdleEnd(const GameEngineRenderingEvent& _Data);
	void AniJumpStartEnd(const GameEngineRenderingEvent& _Data);
	void AniRunEnd(const GameEngineRenderingEvent& _Data);
	void AniRunStartEnd(const GameEngineRenderingEvent& _Data);
	void AniScreamEnd(const GameEngineRenderingEvent& _Data);
	void AniSlamEnd(const GameEngineRenderingEvent& _Data);
	void AniSlamEndEnd(const GameEngineRenderingEvent& _Data);
	void AniTurnLeftEnd(const GameEngineRenderingEvent& _Data);
	void AniTurnRightEnd(const GameEngineRenderingEvent& _Data);


};

