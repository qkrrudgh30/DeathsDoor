#include "PreCompile.h"
#include "LoginLevel.h"
#include "LoginUI.h"
#include "Enums.h"
#include "Player.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>


LoginLevel::LoginLevel()
{
}

LoginLevel::~LoginLevel()
{
}


void LoginLevel::Start()
{


	// ���� �ܾƿ��� �̳༮���� �Ҽ� �ֽ��ϴ�.
	// GetMainCamera()->SetProjectionSize(float4{1920, 1080});

	// ī�޶� ���� ���� ���踦 �� �غ� �ϰ�
	//GetMainCameraActor()->GetTransform().SetLocalPosition({0.0f, 0.0f, -100.0f});
	//GetMainCameraActor()->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	// [1][0][0][0]
	// [0][1][0][0]
	// [0][0][1][0] ���� ���� �ִ�.
	// [0][0][-100][0] �ڷ� ��������

	// ���� ���� ������Ʈ���� ����
	// [800][0][0][0]
	// [0][400][0][0]
	// [0][0][1][0] ���� ���� �ִ�.
	// [0][200][0][0] �ڷ� ��������

	// Ptr->Death(0.0f);

}

void LoginLevel::LevelStartEvent()
{

	if (nullptr == GameEngineTexture::Find("LoginBackground.png"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("Login");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}
	}



	UI = CreateActor<LoginUI>(GameObjectGroup::UI);
	UI->CreateComponent<GameEngineCollision>();


}

void LoginLevel::Update(float _DeltaTime)
{



}

void LoginLevel::End()
{
}