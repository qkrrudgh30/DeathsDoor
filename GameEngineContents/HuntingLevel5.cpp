#include "PreCompile.h"
#include "HuntingLevel5.h"
#include "Player.h"
#include "Potal.h"
#include "ROOM_Far.h"
#include "LoadingUI.h"
#include "Tower.h"
#include "Spike.h"
#include "ContentsBloom.h"
#include "ContentsBlur.h"
#include "EditGUIWindow.h"
#include "WorldLight.h"
#include "Elevator.h"

#include "Firemgr.h"

#include <GameEngineCore/GameEngineFBXStaticRenderer.h>


#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

HuntingLevel5::HuntingLevel5() 
{
}

HuntingLevel5::~HuntingLevel5() 
{
}

void HuntingLevel5::Start()
{
}

void HuntingLevel5::Update(float _DeltaTime)
{
	ContentsLevel::Update(_DeltaTime);



#pragma region EngineCode
	msptrBlurCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
	msptrBloomCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
#pragma endregion
}

void HuntingLevel5::End()
{
}

void HuntingLevel5::LevelStartEvent()
{
#pragma region LoadFBXMeshiesAndAnimation
	if (false == mbPrimitiveInitialized)
	{
		LoadCreaturesFromFile("05_HuntingLevel5");
		msptrContentsBlur->InitializeContentsBlur(msptrContentsBlur);
		msptrContentsBloom->InitializeBloom(msptrContentsBloom);

		{
			std::weak_ptr < ROOM_Far> pMap = CreateActor<ROOM_Far>(OBJECTORDER::StaticMesh);
		}

		{

			std::weak_ptr < Elevator> _Elevator = CreateActor<Elevator>(OBJECTORDER::MoveStatic);
			_Elevator.lock()->GetTransform().SetWorldPosition({ -1997.F,0.F,-2317.F });
			
		}


		{

			std::weak_ptr < Potal> Potal_ = CreateActor<Potal>(OBJECTORDER::NPC);
			Potal_.lock()->GetTransform().SetWorldPosition({ -1997.F,-1300.F,-2317.F });
			Potal_.lock()->m_PotalType = PotalType::Stage5ToStageBoss;
			Potal_.lock()->FBXStaticRenderer->Off();

		}



		//Fire
		{

			std::weak_ptr < Firemgr> Fire = CreateActor<Firemgr>(OBJECTORDER::Eff);
			Fire.lock()->GetTransform().SetWorldPosition({ -548.f,280.f,347.f });


		}


		{

			std::weak_ptr < Firemgr> Fire = CreateActor<Firemgr>(OBJECTORDER::Eff);
			Fire.lock()->GetTransform().SetWorldPosition({ -24.f,280.f,491.f });


		}

		{

			std::weak_ptr < Firemgr> Fire = CreateActor<Firemgr>(OBJECTORDER::Eff);
			Fire.lock()->GetTransform().SetWorldPosition({ 750.f,280.f,-287.f });


		}

		{

			std::weak_ptr < Potal> Potal_ = CreateActor<Potal>(OBJECTORDER::NPC);
			Potal_.lock()->GetTransform().SetWorldPosition({ 542.F,0.F,237.f });
			Potal_.lock()->m_PotalType = PotalType::Stage5ToStage4;

		}

		{
			cMonster = CreateActor<Tower>(OBJECTORDER::Monster);
			std::weak_ptr < Spike> CSpike = CreateActor<Spike>(OBJECTORDER::StaticMesh);
			cMonster.lock()->m_cSpike = CSpike;

		}

		mbPrimitiveInitialized = true;
	}
	EditGUIWindow::OffStartingLevel();
#pragma endregion
	

	{
		if (nullptr == Player::GetMainPlayer())
		{
			//std::shared_ptr < Player> NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
			//NewPlayer->SetLevelOverOn();
		

		}
		else
		{
			Player* NewPlayer = Player::GetMainPlayer();
			NewPlayer->On();
			NewPlayer->GetTransform().SetWorldPosition({ 369.F,0.F,66.F });
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
