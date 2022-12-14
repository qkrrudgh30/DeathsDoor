#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// 자신이 맨처음 서있던 위치를 기억
// 업데이트에서 DeathUpdate에서 death 애니메이션이 종료되면 자신의 모든 랜더러를 off
// LiveTime = 5.0f
// // dj

// 설명 :
class Player;
class Slime : public UnitBase
{
public:
	// constrcuter destructer
	Slime();
	~Slime();

	// delete Function
	Slime(const Slime& _Other) = delete;
	Slime(Slime&& _Other) noexcept = delete;
	Slime& operator=(const Slime& _Other) = delete;
	Slime& operator=(Slime&& _Other) noexcept = delete;

	void HittedSound() override { m_structSoundPlayer.Stop(); m_structSoundPlayer = GameEngineSound::SoundPlayControl("09_SlimeHitted.mp3"); }

	
protected:
	void Start() override;


	float4 StartPostion;
	float4 DirPower;


	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);
	void IdleStart(const StateInfo& _Info);


	void MoveStart(const StateInfo& _Info);
	void MoveEnd(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);


	void StunStart(const StateInfo& _Info);
	void StunEnd(const StateInfo& _Info);
	void StunUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttStart(const StateInfo& _Info);
	void AttEnd(const StateInfo& _Info);
	void AttUpdate(float _DeltaTime, const StateInfo& _Info);


	void DeathStart(const StateInfo& _Info);
	void DeathEnd(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);


	void AniSlime_Att(const GameEngineRenderingEvent& _Data);
	void AniSlime_Idle(const GameEngineRenderingEvent& _Data);
	void AniSlime_Move(const GameEngineRenderingEvent& _Data);


	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;

};

