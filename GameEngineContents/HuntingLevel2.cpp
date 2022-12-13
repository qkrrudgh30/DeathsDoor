#include "PreCompile.h"
#include "HuntingLevel2.h"
#include "Player.h"
#include "Potal.h"
#include "ROOM_Left.h"
#include "LoadingUI.h"
#include "FlowerMonster.h"
#include "ContentsBloom.h"
#include "ContentsBlur.h"
#include "EditGUIWindow.h"
#include "WorldLight.h"

#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

HuntingLevel2::HuntingLevel2() 
{
}

HuntingLevel2::~HuntingLevel2() 
{
}

void HuntingLevel2::Start()
{
}

void HuntingLevel2::Update(float _DeltaTime)
{
	ContentsLevel::Update(_DeltaTime);



#pragma region EngineCode
	msptrBlurCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
	msptrBloomCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
#pragma endregion
}

void HuntingLevel2::End()
{
}

void HuntingLevel2::LevelStartEvent()
{
#pragma region LoadFBXMeshiesAndAnimation
	if (false == mbPrimitiveInitialized)
	{
		LoadCreaturesFromFile("02_HuntingLevel2");
		msptrContentsBlur->InitializeContentsBlur(msptrContentsBlur);
		msptrContentsBloom->InitializeBloom(msptrContentsBloom);

		{
			std::weak_ptr < ROOM_Left> pMap = CreateActor<ROOM_Left>(OBJECTORDER::StaticMesh);
		}

		{

			std::weak_ptr < Potal> Potal_ = CreateActor<Potal>(OBJECTORDER::NPC);
			Potal_.lock()->GetTransform().SetWorldPosition({ -1205.F,0.F,-3169.F });
			Potal_.lock()->m_PotalType = PotalType::Stage2ToStage3;

		}



		{

			std::weak_ptr < Potal> Potal_ = CreateActor<Potal>(OBJECTORDER::NPC);
			Potal_.lock()->GetTransform().SetWorldPosition({ 204.F,0.F,-638.f });
			Potal_.lock()->m_PotalType = PotalType::Stage2ToStage1;



		}


		{
			std::weak_ptr < FlowerMonster> cMonster = CreateActor<FlowerMonster>(OBJECTORDER::Monster);
			cMonster.lock()->GetTransform().SetWorldPosition({ 1251.f,0.F,-787.f });


		}


		{
			std::weak_ptr < FlowerMonster> cMonster = CreateActor<FlowerMonster>(OBJECTORDER::Monster);
			cMonster.lock()->GetTransform().SetWorldPosition({ 78.f,0.F,-1672.f });


		}

		{
			std::weak_ptr < FlowerMonster> cMonster = CreateActor<FlowerMonster>(OBJECTORDER::Monster);
			cMonster.lock()->GetTransform().SetWorldPosition({ -550.f,0.F,-2660.f });


		}



		mbPrimitiveInitialized = true;
	}
	EditGUIWindow::OffStartingLevel();
#pragma endregion
	

	{
		if (nullptr == Player::GetMainPlayer())
		{
			//std::shared_ptr < Player> NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		//	NewPlayer->SetLevelOverOn();


		}
		else
		{
			Player* NewPlayer = Player::GetMainPlayer();
			NewPlayer->GetTransform().SetWorldPosition({ 432.F,0.F,-862.F });
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
