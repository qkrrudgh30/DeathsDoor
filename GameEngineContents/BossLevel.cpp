#include "PreCompile.h"
#include "BossLevel.h"
#include "Player.h"
#include "UIMouse.h"
#include "LoadingUI.h"


#include "Floor.h"
#include "Rock.h"
#include "OldCrow.h"
#include "ContentsBloom.h"
#include "ContentsBlur.h"


#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

BossLevel::BossLevel() 
{
}

BossLevel::~BossLevel() 
{
}

void BossLevel::Start()
{
}

void BossLevel::Update(float _DeltaTime)
{



#pragma region EngineCode
	msptrBlurCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
	msptrBloomCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
#pragma endregion
}

void BossLevel::End()
{
}

void BossLevel::LevelStartEvent()
{
#pragma region LoadFBXMeshiesAndAnimation
	if (false == mbPrimitiveInitialized)
	{
		LoadCreaturesFromFile("06_BossLevel");
		msptrContentsBlur->InitializeContentsBlur(msptrContentsBlur);
		msptrContentsBloom->InitializeBloom(msptrContentsBloom);


		std::weak_ptr < Rock> CRock = CreateActor<Rock>(OBJECTORDER::StaticMesh);


		std::weak_ptr < Floor> CFloor = CreateActor<Floor>(OBJECTORDER::BackGround);

		std::weak_ptr < OldCrow> COldCrow = CreateActor<OldCrow>(OBJECTORDER::BackGround);

		mbPrimitiveInitialized = true;
	}
#pragma endregion
	{
		if (nullptr == Player::GetMainPlayer())
		{
		/*	std::weak_ptr< Player> NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
			NewPlayer->SetLevelOverOn();*/


		}
		else
		{
			Player* NewPlayer = Player::GetMainPlayer();
			NewPlayer->GetTransform().SetWorldPosition({ 1.F,0.F,1.F });
			NewPlayer->UIOn();
			//NewPlayer->m_bLogoLevelCheck = false;

			GetMainCamera()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
			GetMainCameraActorTransform().SetWorldRotation({ 45.f,0.f,0.f });
			float4 CameraWorldPos = GetMainCameraActorTransform().GetWorldPosition();
			CameraWorldPos.y += 1700.f;
			CameraWorldPos.z -= 1700.f;
			GetMainCameraActorTransform().SetWorldPosition(CameraWorldPos);



		}
	}



}
