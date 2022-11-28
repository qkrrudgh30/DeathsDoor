#include "PreCompile.h"
#include "GameEngineStatusWindow.h"
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GEngine.h>
#include "ContentsLevel.h"

std::map<std::string, GameEngineRenderTarget*> GameEngineStatusWindow::DebugRenderTarget;
bool GameEngineStatusWindow::IsHost = true;


void GameEngineImageShotWindow::RenderTextureSetting(ImTextureID _RenderTexture, ImVec2 _Size)
{
	RenderTexture = _RenderTexture;
	Size = _Size;
}

void GameEngineImageShotWindow::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	if (true == ImGui::ImageButton(RenderTexture, { Size.x, Size.y }))
	{
		Off();
	}

}


GameEngineStatusWindow::GameEngineStatusWindow()
{
}

GameEngineStatusWindow::~GameEngineStatusWindow()
{
}


void GameEngineStatusWindow::Initialize(class GameEngineLevel* _Level)
{

}

void GameEngineStatusWindow::AddDebugRenderTarget(const std::string& _DebugName, GameEngineRenderTarget* _RenderTarget)
{
	if (DebugRenderTarget.end() != DebugRenderTarget.find(_DebugName))
	{
		MsgBoxAssert("�̹� �����ϴ� ����� ����Ÿ���Դϴ�.");
	}

	DebugRenderTarget.insert(std::make_pair(_DebugName, _RenderTarget));
}

void GameEngineStatusWindow::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	int FPS = static_cast<int>(1.0f / _DeltaTime);
	// printf �����ε� �Ⱦ�.

	std::string Name = "FPS : " + std::to_string(GameEngineTime::GetFPS());
	ImGui::Text(Name.c_str());

	if (true == ImGui::Button("On/Off collider visibleness"))
	{
		GEngine::CollisionDebugSwitch();
	}

	ImGui::SameLine();

	if (true == ImGui::Button("On/Off free-camera mode"))
	{
		// ;
		GEngine::GetCurrentLevel()->GetMainCameraActor()->FreeCameraModeOnOff();
	}

	ImGui::Text("Select level");
	for (std::pair<std::string, GameEngineLevel*> Pair : GameEngineCore::AllLevels)
	{
		if (true == ImGui::Button(Pair.first.c_str()))
		{
			ContentsLevel::mstrNextLevelName = Pair.first;
			GEngine::ChangeLevel("07_EditLevel");
		}

		ImGui::SameLine();
	}

	ImGui::NewLine();

}