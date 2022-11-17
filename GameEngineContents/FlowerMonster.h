
#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// �ڽ��� ��ó�� ���ִ� ��ġ�� ���
// ������Ʈ���� DeathUpdate���� death �ִϸ��̼��� ����Ǹ� �ڽ��� ��� �������� off
// LiveTime = 5.0f
// // dj

// ���� :
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

	void AniFlower_Att(const GameEngineRenderingEvent& _Data);
	void AniFlower_Idle(const GameEngineRenderingEvent& _Data);
	void AniFlower_Death(const GameEngineRenderingEvent& _Data);


	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;

};

