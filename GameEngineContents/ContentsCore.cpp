#include "PreCompile.h"
#include "ContentsCore.h"
#include "GameEngineContents/LoginLevel.h"
#include "GameEngineContents/PlayLevel.h"
#include "GameEngineContents/TestLevel.h"
#include "GameEngineContents/EditLevel.h"
#include "GameEngineContents/EditGUIWindow.h"



#include <GameEngineBase/magic_enum.hpp>


#pragma comment(lib, "GameEngineBase.lib")

#define ENGINE
// #define CLIENT

ContentsCore::ContentsCore() 
	: GameEngineCore()
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{

	GameEngineTime::SetLimitFrame(60);
	std::string_view Value = magic_enum::enum_name(OBJECTORDER::Player);
	std::string Name = Value.data();

	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");

		// std::vector<GameEngineDirectory> DirList = Dir.GetRecursiveAllDirectory();

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineTexture::Load(Shaders[i].GetFullPath());
		}

	}


	{
		GameEngineDirectory Dir;

		Dir.MoveParentToExitsChildDirectory("GameEngineResources");
		Dir.Move("ContentsResources");
		Dir.Move("Shader");

		std::vector<GameEngineFile> Shaders = Dir.GetAllFile("hlsl");

		for (size_t i = 0; i < Shaders.size(); i++)
		{
			GameEngineShader::AutoCompile(Shaders[i].GetFullPath());
		}
	}

	//{
	//	GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("YEffect");
	//	NewPipe->SetInputAssembler1VertexBuffer("FullRect");
	//	NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	//	NewPipe->SetVertexShader("YEffect.hlsl");
	//	NewPipe->SetPixelShader("YEffect.hlsl");
	//}

	if (false == GameEngineInput::GetInst()->IsKey("LevelChangeKey"))
	{
		GameEngineInput::GetInst()->CreateKey("LevelChangeKey", 'P');
	}

	// RTTI �� Ÿ�� Ÿ�� �������̼�
	CreateLevel<LoginLevel>("Login");
	CreateLevel<PlayLevel>("Play");
	CreateLevel<TestLevel>("Test");
	CreateLevel<EditLevel>("Edit");

#ifdef ENGINE
	ChangeLevel("Edit");
#else
	ChangeLevel("Login");
#endif
	
	GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);
	GameEngineGUI::CreateGUIWindow<EditGUIWindow>("EditGUIWindow", nullptr);

}

void ContentsCore::Update(float _DeltaTime)
{
}

void ContentsCore::End()
{
}