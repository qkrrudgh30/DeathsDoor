#include "PreCompile.h"
#include "GameEngineFBXAnimationRenderer.h"
#include "GameEngineMaterial.h"
#include "GameEngineVertexShader.h"

GameEngineFBXAnimationRenderer::GameEngineFBXAnimationRenderer() 
{
	
}

GameEngineFBXAnimationRenderer::~GameEngineFBXAnimationRenderer() 
{
	Animations.clear();
}


void GameEngineFBXAnimationRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	GameEngineMaterial* Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return;
	}

	if (false == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("FBX �ִϸ��̼� �������� �ִϸ��̼��� �Ұ����� ��Ƽ������ �����߽��ϴ�.");
		return;
	}

	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
}

void GameEngineFBXAnimationRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t _MeshIndex, size_t _SubSetIndex /*= 0*/)
{
	GameEngineMaterial* Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return;
	}

	if (false == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("FBX �ִϸ��̼� �������� �ִϸ��̼��� �Ұ����� ��Ƽ������ �����߽��ϴ�.");
		return;
	}

	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, _MeshIndex, _SubSetIndex);
}

void GameEngineFBXAnimationRenderer::CreateFBXAnimation(const std::string& _AnimationName
	, const std::string& _AnimationFBX) 
{
	// ���� ���� fbx�� ���õǾ� �ִ��� �˻��Ѵ�.
	if (nullptr == GetFBXMesh())
	{
		MsgBoxAssert("��� FBX�� ���õǾ� ���� �ʽ��ϴ�.");
		return;
	}

	// �����ִ� �Լ��� �ܺο��� ȣ���ϸ� �˴ϴ�.

	if (Animations.end() != Animations.find(_AnimationName))
	{
		MsgBoxAssertString("�̹� �����ϴ� �̸��� �ִϸ��̼��Դϴ�." + _AnimationName);
		return;
	}

	GameEngineFBXAnimation* Animation = GameEngineFBXAnimation::Find(_AnimationFBX);

	if (nullptr == Animation)
	{
		MsgBoxAssertString("GameEngineFBXAnimation�� �������� �ʽ��ϴ�. " + _AnimationFBX);
		return;
	}

	std::shared_ptr<FBXRendererAnimation> NewAnimation = std::make_shared<FBXRendererAnimation>();

	NewAnimation->Mesh = GetFBXMesh();
	NewAnimation->Aniamtion = Animation;

	RenderOptionInst.IsAnimation = 1;

	Animations.insert(std::make_pair(_AnimationName, NewAnimation));

}