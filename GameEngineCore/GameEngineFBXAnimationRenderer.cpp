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


// _Index
// �ִϸ��̼��� ���� fbx�� AnimationDatas(vector)�� index�� �ǹ��Ѵ�.

void FBXRendererAnimation::Init(int _Index)
{
	// GameENgineFBXAnimation�� ��� ������ ���������°��� 
	// CalFbxExBoneFrameTransMatrix�� ȣ��ǰ� �� ���Դϴ�.
	// �ִϸ��̼��� ����� ���Ǵ°̴ϴ�.
	Aniamtion->AnimationMatrixLoad(Mesh, _Index);
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = 0;
	End = FBXAnimationData->TimeEndCount;
	FrameTime = 0.02f;

}

void FBXRendererAnimation::Update()
{

}

void FBXRendererAnimation::Reset()
{
	Start = 0;
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
	, const std::string& _AnimationFBX, int _Index)
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
	NewAnimation->ParentRenderer = this;

	NewAnimation->Init(_Index);

	// �̼��� �ִϸ��̼� ������ ��Ŀ� ���� ����� �̶� �̷����µ�.
	// �̰� �������̱� ������ �Ƹ� ���� �и���.
	// �ٸ� ���̳� �Լ��� �ѹ� �ε��ϰ� �츮 �������� �����ϴ� ���� �ؾ��Ұ̴ϴ�.

	RenderOptionInst.IsAnimation = 1;

	Animations.insert(std::make_pair(_AnimationName, NewAnimation));

	Animation;
}

void GameEngineFBXAnimationRenderer::ChangeAnimation(const std::string& _AnimationName)
{
	std::map<std::string, std::shared_ptr<FBXRendererAnimation>>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	CurAnimation = FindIter->second;
}