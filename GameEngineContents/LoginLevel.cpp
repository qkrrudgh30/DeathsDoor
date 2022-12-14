#include "PreCompile.h"
#include "LoginLevel.h"
#include "LoginUI.h"
#include "Enums.h"
#include "Player.h"
#include "TestActor.h"
#include "StaticMesh.h"
#include "Hall.h"
#include "LoadingUI.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

#include "UIMouse.h"
#include "ShopNPC.h"
#include "Potal.h"

#include <GameEngineCore/GameEngineFBXStaticRenderer.h>


#include "PlayerMainUI.h"
#include "PlayerUpgradeUI.h"

#include "ContentsBloom.h"
#include "ContentsBlur.h"
#include "EditGUIWindow.h"

#include "WorldLight.h"


#include "TsetBackGround.h"

#include "TestFog.h"

LoginLevel::LoginLevel()	:
	UI()
{
}

LoginLevel::~LoginLevel()
{
}


void LoginLevel::Start()
{
	



	GetMainCamera()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
	GetMainCameraActorTransform().SetWorldRotation({ 0.9f,2.9f,0.f });
	float4 CameraWorldPos = GetMainCameraActorTransform().GetWorldPosition();
	CameraWorldPos.x = -643.f;
	CameraWorldPos.y = 628.f;
	CameraWorldPos.z = -2672.f;;
	GetMainCameraActorTransform().SetWorldPosition(CameraWorldPos);



	if (false == GameEngineInput::GetInst()->IsKey("FreeCameaOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameaOnOff", 'O');
	}





	{
		std::shared_ptr < UIMouse> m_UIMouse = CreateActor<UIMouse>(OBJECTORDER::MouseUI);
		m_UIMouse->SetLevelOverOn();
	}





}

void LoginLevel::LevelStartEvent()
{
#pragma region LoadFBXMeshiesAndAnimation
	if (false == mbPrimitiveInitialized)
	{
		LoadCreaturesFromFile("00_LoginLevel");
		msptrContentsBlur->InitializeContentsBlur(msptrContentsBlur);
		msptrContentsBloom->InitializeBloom(msptrContentsBloom);
		EditGUIWindow::LoadPostEffectInfo();

		if (nullptr == WorldLight::GetWorldLight())
		{
			m_wptrWorldLight = CreateActor<WorldLight>(OBJECTORDER::LIGHT);
			m_wptrWorldLight.lock()->SetLevelOverOn();
		}

		EditGUIWindow::LoadLightData();

		mbPrimitiveInitialized = true;
	}
	EditGUIWindow::OffStartingLevel();

#pragma endregion
	

	if (nullptr == UI.lock())
	{


		CreateActor<Hall>(); // Test Code.


		{
			UI = CreateActor<LoginUI>(GameObjectGroup::UI);
			UI.lock()->CreateComponent<GameEngineCollision>();

			std::weak_ptr < ShopNPC> cShopNPC = CreateActor<ShopNPC>(OBJECTORDER::NPC);
			cShopNPC.lock()->GetTransform().SetWorldPosition({ -350.f,100.F,700.F });
			
			

		}
		//TsetBackGround
		{

			std::weak_ptr < TsetBackGround> BackGround = CreateActor<TsetBackGround>(OBJECTORDER::Eff);
			BackGround.lock()->GetTransform().SetWorldPosition({ -429.F,2309.F,4869.F });
			
		}



		{
			
			std::weak_ptr < TestFog> _TestFog = CreateActor<TestFog>(OBJECTORDER::UI);
			//_TestFog.lock()->GetTransform().SetWorldPosition({ 2122.f,216.f,-767.f });

		}


		
		


		
		
		{

			std::weak_ptr < Potal> Potal_ = CreateActor<Potal>(OBJECTORDER::NPC);
			Potal_.lock()->GetTransform().SetWorldPosition({ 250.F,100.F,-1300.F });
			Potal_.lock()->m_PotalType = PotalType::LoginToStage1;
			Potal_.lock()->FBXStaticRenderer->Off();
		}
	
		

	}





	{
		if (nullptr == Player::GetMainPlayer())
		{
			std::weak_ptr < Player> NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
			NewPlayer.lock()->GetTransform().SetWorldPosition({-600.F,120.F,-150.F});
			NewPlayer.lock()->SetLevelOverOn();
			NewPlayer.lock()->UIOff();
			NewPlayer.lock()->m_bLogoLevelCheck = true;
			NewPlayer.lock()->On();

		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetWorldPosition({ -600.F,120.F,-150.F });
		}
	}

#pragma region EngineCode

	

#pragma endregion

}

void LoginLevel::Update(float _DeltaTime)
{
	ContentsLevel::Update(_DeltaTime);
	//float4 MyPos = GetMainCameraActorTransform().GetWorldPosition();

	//std::string A2 = std::to_string(MyPos.x);
	//std::string B2 = std::to_string(MyPos.y);
	//std::string C2 = std::to_string(MyPos.z);

	//A2 += " : PX";
	//B2 += " : PY";
	//C2 += " : PZ";

	//GameEngineDebug::OutPutString(A2);
	//GameEngineDebug::OutPutString(B2);
	//GameEngineDebug::OutPutString(C2);


	//MyPos = GetMainCameraActorTransform().GetLocalRotation();

	//std::string A1 = std::to_string(MyPos.x);
	//std::string B1 = std::to_string(MyPos.y);
	//std::string C1 = std::to_string(MyPos.z);

	//A1 += " : LX";
	//B1 += " : LY";
	//C1 += " : LZ";

	//GameEngineDebug::OutPutString(A1);
	//GameEngineDebug::OutPutString(B1);
	//GameEngineDebug::OutPutString(C1);

#pragma region EngineCode

	msptrBlurCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());
	msptrBloomCameraActor->GetTransform().Copy(GEngine::GetCurrentLevel()->GetMainCameraActorTransform());

#pragma endregion
}

void LoginLevel::End()
{
}