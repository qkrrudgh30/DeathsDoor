

#include "PreCompile.h"
#include "ROOM_MainHall.h"
#include <GameEngineCore/GameEngineFBXStaticRenderer.h>
//#include "GameEngineCore/GameEngineDefaultRenderer.h"

ROOM_MainHall::ROOM_MainHall()
	: ResultColor()
{
}

ROOM_MainHall::~ROOM_MainHall()
{
}

void ROOM_MainHall::Start()
{
	ResultColor = float4{ 1.f, 1.f, 0.f, 1.f };
	std::shared_ptr < GameEngineFBXStaticRenderer> Renderer = CreateComponent<GameEngineFBXStaticRenderer>();
	Renderer->GetTransform().SetLocalScale(float4{ 0.5f, 0.5f, 0.5f });
	Renderer->GetTransform().SetLocalRotation({ 0.f,45.f,0.f });
	Renderer->SetFBXMesh("ROOM_MainHall.fbx", "Texture", 0, 0);
	/*for (int i = 0; i < 3; ++i)
	{
		if (2 == i)
		{
			Renderer->SetFBXMesh("ROOM_MainHall.fbx", "Color", 0, i);
			Renderer->GetAllRenderUnit()[0][i].ShaderResources.SetConstantBufferLink("ResultColor", float4::BLACK);
			continue;
		}
		Renderer->SetFBXMesh("ROOM_MainHall.fbx", "Texture", 0, i);
	}*/

#pragma region ROOM_Enter (1).FBX


	//for (int i = 0; i < 409; ++i)
	//{
	//	if (62 == i)
	//	{
	//		Renderer->SetFBXMesh("ROOM_MainHall.fbx", "Color", i);

	//		Renderer->GetAllRenderUnit()[i][0].ShaderResources.SetConstantBufferLink("ResultColor", float4::BLACK);
	//		continue;
	//	}

	//	Renderer->SetFBXMesh("ROOM_MainHall.fbx", "Texture", i);

	//}



#pragma endregion

#pragma region crow_player (fbx)



#pragma endregion



}

void ROOM_MainHall::Update(float _DeltaTime)
{

}

