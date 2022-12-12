

#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// �ڽ��� ��ó�� ���ִ� ��ġ�� ���
// ������Ʈ���� DeathUpdate���� death �ִϸ��̼��� ����Ǹ� �ڽ��� ��� �������� off
// LiveTime = 5.0f
// // dj

// ���� :
class Player;
class Headroller : public UnitBase
{
public:
	// constrcuter destructer
	Headroller();
	~Headroller();

	// delete Function
	Headroller(const Headroller& _Other) = delete;
	Headroller(Headroller&& _Other) noexcept = delete;
	Headroller& operator=(const Headroller& _Other) = delete;
	Headroller& operator=(Headroller&& _Other) noexcept = delete;

	void HittedSound() override { m_structSoundPlayer.Stop(); m_structSoundPlayer = GameEngineSound::SoundPlayControl("17_HeadRollerHitted.mp3"); }


protected:
	void Start() override;


	float4 StartPostion;
	float4 DirPower;


	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);
	void IdleStart(const StateInfo& _Info);


	void MoveStart(const StateInfo& _Info);
	void MoveEnd(const StateInfo& _Info);
	void MoveUpdate(float _DeltaTime, const StateInfo& _Info);


	void DeathStart(const StateInfo& _Info);
	void DeathEnd(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttStart(const StateInfo& _Info);
	void AttEnd(const StateInfo& _Info);
	void AttUpdate(float _DeltaTime, const StateInfo& _Info);


	void Ani_Att(const GameEngineRenderingEvent& _Data);
	void Ani_Att2(const GameEngineRenderingEvent& _Data);
	void Ani_Idle(const GameEngineRenderingEvent& _Data);
	void Ani_Dash(const GameEngineRenderingEvent& _Data);
	void Ani_Dash_E(const GameEngineRenderingEvent& _Data);
	void Ani_Dash_S(const GameEngineRenderingEvent& _Data);
	void Ani_Death(const GameEngineRenderingEvent& _Data);

	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;
	float m_Len = 0;
	float Angle = 0;

	bool m_bAtt = false;
	bool m_bMove =  false;
};

