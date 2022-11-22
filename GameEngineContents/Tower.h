#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// �ڽ��� ��ó�� ���ִ� ��ġ�� ���
// ������Ʈ���� DeathUpdate���� death �ִϸ��̼��� ����Ǹ� �ڽ��� ��� �������� off
// LiveTime = 5.0f
// // dj

// ���� :
class Player;
class Tower : public UnitBase
{
public:
	// constrcuter destructer
	Tower();
	~Tower();

	// delete Function
	Tower(const Tower& _Other) = delete;
	Tower(Tower&& _Other) noexcept = delete;
	Tower& operator=(const Tower& _Other) = delete;
	Tower& operator=(Tower&& _Other) noexcept = delete;



	
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
	std::shared_ptr < GameEngineCollision> StartCollision;
public : 
	bool m_bstart = false;

};

