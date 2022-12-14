#include "PreCompile.h"
#include "PlayerSWAtt2.h"
#include "SWEffMgr.h"

#include <GameEngineCore/GameEngineFBXStaticRenderer.h>
PlayerSWAtt2::PlayerSWAtt2()
{
}

PlayerSWAtt2::~PlayerSWAtt2()
{
}

void PlayerSWAtt2::Start()
{

	m_Info.Dammage = 1;
	AttCollision = CreateComponent<GameEngineCollision>();
	AttCollision->GetTransform().SetLocalScale({ 150.0f, 500.0f, 150.0f });
	AttCollision->GetTransform().SetLocalPosition(AttCollision->GetTransform().GetForwardVector() * 100.f);

	AttCollision->ChangeOrder(OBJECTORDER::PlayerAtt);
	AttCollision->SetCollisionMode(CollisionMode::Ex);
	{
		FBXStaticRenderer = CreateComponent<GameEngineFBXStaticRenderer>();
		FBXStaticRenderer->GetTransform().SetLocalScale(float4{ 0.4f, 0.4f, 0.4f });
		FBXStaticRenderer->SetFBXMesh("Sworld_Trail_1.fbx", "StaticPaperBurn");
		FBXStaticRenderer->ChangeCamera(CAMERAORDER::MAINCAMERA);
		// FBXStaticRenderer->GetAllRenderUnit()[0][0].GetCloneMaterial()->SetOutputMergerBlend("Lighten");

#pragma region StaticPaperBurn

		InitializePaperBurn(FBXStaticRenderer);
		m_fAccTimeForPaperburn = 0.f;
		mfPaperburnDeathTime = 5.f;

#pragma endregion

		

	}





}

void PlayerSWAtt2::Update(float _DeltaTime)
{

	AttCollision->IsCollisionEnterBase(CollisionType::CT_OBB, static_cast<int>(OBJECTORDER::Monster), CollisionType::CT_OBB,
		std::bind(&PlayerSWAtt2::MonsterCollision, this, std::placeholders::_1, std::placeholders::_2)
	);

#pragma region StaticPaperBurn
	m_fAccTimeForPaperburn += _DeltaTime * mfPaperburnDeathTime;
	SetPaperBurnInfo(1u, m_fAccTimeForPaperburn);
#pragma endregion	
}

CollisionReturn PlayerSWAtt2::MonsterCollision(std::shared_ptr <GameEngineCollision> _This, std::shared_ptr <GameEngineCollision> _Other)
{
	std::dynamic_pointer_cast<UnitBase>(_Other->GetParent())->m_Info.m_Hp -= m_Info.Dammage;
	std::dynamic_pointer_cast<UnitBase>(_Other->GetParent())->m_bHitCheck = true;
	std::dynamic_pointer_cast<UnitBase>(_Other->GetParent())->m_bHitActionCheck = true;
	std::weak_ptr < SWEffMgr> Bullet = GEngine::GetCurrentLevel()->CreateActor<SWEffMgr>(OBJECTORDER::Eff);

	Bullet.lock()->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition());


	return CollisionReturn::Break;
}

