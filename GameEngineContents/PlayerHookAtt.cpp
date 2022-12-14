
#include "PreCompile.h"
#include "PlayerHookAtt.h"
#include "PlayerHookTrail.h"

#include <GameEngineCore/GameEngineFBXStaticRenderer.h>
PlayerHookAtt::PlayerHookAtt()
{
}

PlayerHookAtt::~PlayerHookAtt()
{
}

void PlayerHookAtt::Start()
{

	m_fSpeed = 2000.f;

	{
		FBXStaticRenderer = CreateComponent<GameEngineFBXStaticRenderer>();
		FBXStaticRenderer->GetTransform().SetLocalPosition(float4{ 0.f, 0.f, 0.f });
		FBXStaticRenderer->GetTransform().SetLocalScale(float4{ 0.5f, 1.f, 1.f });
		FBXStaticRenderer->SetFBXMesh("Arrow.FBX", "Texture");


	}



	AttCollision = CreateComponent<GameEngineCollision>();
	AttCollision->GetTransform().SetLocalScale({ 20.f, 500.0f, 50.0f });
	AttCollision->ChangeOrder(OBJECTORDER::PlayerHook);



}

void PlayerHookAtt::Update(float _DeltaTime)
{


	if (m_bHitCheck)
	{

		AttCollision->IsCollision(CollisionType::CT_OBB, OBJECTORDER::Player, CollisionType::CT_OBB,
			std::bind(&PlayerHookAtt::PlayerCollision, this, std::placeholders::_1, std::placeholders::_2)
		);


		return;

	}



	float4 MoveDir = FBXStaticRenderer->GetTransform().GetForwardVector();


	m_fLifeTime += _DeltaTime;
	m_ftrailTime += _DeltaTime;


	if (m_fLifeTime >= 0.5f)
	{
		MoveDir = MoveDir * -1.f;

		AttCollision->IsCollision(CollisionType::CT_OBB, OBJECTORDER::Player, CollisionType::CT_OBB,
			std::bind(&PlayerHookAtt::PlayerCollision, this, std::placeholders::_1, std::placeholders::_2)
		);


		m_bHookBack = true;




		if (m_fLifeTime >= 1.3f)
		{
			Death();
		}
	}
	else
	{
		AttCollision->IsCollision(CollisionType::CT_OBB, OBJECTORDER::Monster, CollisionType::CT_OBB,
			std::bind(&PlayerHookAtt::MonsterCollision, this, std::placeholders::_1, std::placeholders::_2)
		);

		if (m_ftrailTime >= 0.01f)
		{
			m_ftrailTime -= 0.01f;
			std::weak_ptr< PlayerHookTrail> HookTrail = GetLevel()->CreateActor<PlayerHookTrail>(OBJECTORDER::PlayerHookTrail);
			HookTrail.lock()->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition());
			HookTrail.lock()->GetTransform().SetLocalRotation(GetTransform().GetLocalRotation());
			HookTrail.lock()->m_cHook =  std::dynamic_pointer_cast<PlayerHookAtt>(shared_from_this());
		}


	}
	

	
	GetTransform().SetWorldMove(MoveDir * m_fSpeed * _DeltaTime);




}

CollisionReturn PlayerHookAtt::MonsterCollision(std::shared_ptr < GameEngineCollision> _This, std::shared_ptr < GameEngineCollision> _Other)
{


	Player::GetMainPlayer()->m_bHookHitcheck = true;
	m_bHitCheck = true;
	Player::GetMainPlayer()->m_fHookPoint = GetTransform().GetWorldPosition();


	return CollisionReturn::Break;
}

CollisionReturn PlayerHookAtt::PlayerCollision(std::shared_ptr < GameEngineCollision> _This, std::shared_ptr < GameEngineCollision> _Other)
{


	Death();
	Player::GetMainPlayer()->m_bHookEndcheck = true;

	return CollisionReturn::Break;
}

CollisionReturn PlayerHookAtt::TrailCollision(std::shared_ptr < GameEngineCollision> _This, std::shared_ptr < GameEngineCollision> _Other)
{


	_Other->GetParent()->Death();


	return CollisionReturn::Break;
}

