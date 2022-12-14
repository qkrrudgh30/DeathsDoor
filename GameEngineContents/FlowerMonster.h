
#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// 자신이 맨처음 서있던 위치를 기억
// 업데이트에서 DeathUpdate에서 death 애니메이션이 종료되면 자신의 모든 랜더러를 off
// LiveTime = 5.0f
// // dj

// 설명 :
class Player;
class FlowerMonster : public UnitBase
{
public:
	// constrcuter destructer
	FlowerMonster();
	~FlowerMonster();

	// delete Function
	FlowerMonster(const FlowerMonster& _Other) = delete;
	FlowerMonster(FlowerMonster&& _Other) noexcept = delete;
	FlowerMonster& operator=(const FlowerMonster& _Other) = delete;
	FlowerMonster& operator=(FlowerMonster&& _Other) noexcept = delete;

	void HittedSound() override { m_structSoundPlayer.Stop(); m_structSoundPlayer = GameEngineSound::SoundPlayControl("12_FlowerHitted.mp3"); }


protected:
	void Start() override;


	float4 StartPostion;
	float4 DirPower;


	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);
	void IdleStart(const StateInfo& _Info);

	void StunStart(const StateInfo& _Info);
	void StunEnd(const StateInfo& _Info);
	void StunUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttStart(const StateInfo& _Info);
	void AttEnd(const StateInfo& _Info);
	void AttUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathEnd(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);



	void AniFlower_Att(const GameEngineRenderingEvent& _Data);
	void AniFlower_AttFrame(const GameEngineRenderingEvent& _Data);
	void AniFlower_Idle(const GameEngineRenderingEvent& _Data);


	void AniFlower_Death(const GameEngineRenderingEvent& _Data);


	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;

};

