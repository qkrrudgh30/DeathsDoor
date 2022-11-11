#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// �ڽ��� ��ó�� ���ִ� ��ġ�� ���
// ������Ʈ���� DeathUpdate���� death �ִϸ��̼��� ����Ǹ� �ڽ��� ��� �������� off
// LiveTime = 5.0f
// // dj

// ���� :
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



	void End() {}

	virtual void Update(float _DeltaTime) override;

private:
	float4 Pos;

};
