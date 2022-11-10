

#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// ���� :
class FlowerBullet : public UnitBase
{
public:
	// constrcuter destructer
	FlowerBullet();
	~FlowerBullet();

	// delete Function
	FlowerBullet(const FlowerBullet& _Other) = delete;
	FlowerBullet(FlowerBullet&& _Other) noexcept = delete;
	FlowerBullet& operator=(const FlowerBullet& _Other) = delete;
	FlowerBullet& operator=(FlowerBullet&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:


	CollisionReturn PlayerCollision(std::shared_ptr < GameEngineCollision> _This, std::shared_ptr < GameEngineCollision> _Other);



};

