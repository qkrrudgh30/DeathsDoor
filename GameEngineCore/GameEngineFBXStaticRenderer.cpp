#include "PreCompile.h"
#include "GameEngineFBXStaticRenderer.h"
#include "GameEngineVertexShader.h"

GameEngineFBXStaticRenderer::GameEngineFBXStaticRenderer() 
{
}

GameEngineFBXStaticRenderer::~GameEngineFBXStaticRenderer() 
{
}

void GameEngineFBXStaticRenderer::SetFBXMesh(const std::string& _Name, std::string _Material) 
{
	GameEngineMaterial* Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return;
	}

	if (true == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("����ƽ �Ž� �������� �ִϸ��̼��� �Ҽ� �����ϴ�. ��Ƽ���� ���ο� ArrAniMationMatrix�� ����ϰ� �ֽ��ϴ�.");
		return;
	}

	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
}

void GameEngineFBXStaticRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t _MeshIndex, size_t _SubSetIndex /*= 0*/)
{
	GameEngineMaterial* Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return;
	}

	if (true == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("����ƽ �Ž� �������� �ִϸ��̼��� �Ҽ� �����ϴ�. ��Ƽ���� ���ο� ArrAniMationMatrix�� ����ϰ� �ֽ��ϴ�.");
		return;
	}

	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, _MeshIndex, _SubSetIndex);
}

void GameEngineFBXStaticRenderer::Render(float _DeltaTime) 
{
	GameEngineFBXRenderer::Render(_DeltaTime);
}