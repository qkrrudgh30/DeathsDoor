#pragma once
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineRenderingEvent.h"
#include <map>

class FbxExAniData;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class GameEngineFBXAnimationRenderer;
class FBXRendererAnimation : public std::enable_shared_from_this<FBXRendererAnimation>
{
public:
	GameEngineFBXAnimationRenderer* ParentRenderer;
	// SetFBX ���� ������ �ִ� �Ž�
	std::shared_ptr<GameEngineFBXMesh> Mesh;
	// �ִϸ��̼��� ������ �ִ� FBX
	std::shared_ptr<GameEngineFBXAnimation> Aniamtion;
	FbxExAniData* FBXAnimationData;

	UINT Start;
	UINT End;

	GameEngineRenderingEvent Info;

	bool bOnceStart;
	bool bOnceEnd;
	std::function<void(const GameEngineRenderingEvent&)> FrameEvent;
	std::function<void(const GameEngineRenderingEvent&)> EndEvent;
	std::function<void(const GameEngineRenderingEvent&)> StartEvent;
	std::function<void(const GameEngineRenderingEvent&, float)> TimeEvent;

	// Event

	void PauseSwtich();
	void Init(const std::string_view& _Name, int _Index);
	void Reset();
	void Update(float _DeltaTime);

public:
	FBXRendererAnimation() 
		: Start(0)
		, End(0)
		, bOnceStart(false)
		, bOnceEnd(false)
	{
		int a = 0;
	}

	~FBXRendererAnimation()
	{
		int a = 0;
	}
};

struct AnimationBoneData
{
public:
	int Index = -1;
	float4 Scale;
	float4 RotQuaternion;
	float4 Pos;
	float4 RotEuler;
};


// ���� :
class GameEngineFBXAnimationRenderer : public GameEngineFBXRenderer
{
	friend FBXRendererAnimation;

public:
	// constrcuter destructer
	GameEngineFBXAnimationRenderer();
	~GameEngineFBXAnimationRenderer();

	// delete Function
	GameEngineFBXAnimationRenderer(const GameEngineFBXAnimationRenderer& _Other) = delete;
	GameEngineFBXAnimationRenderer(GameEngineFBXAnimationRenderer&& _Other) noexcept = delete;
	GameEngineFBXAnimationRenderer& operator=(const GameEngineFBXAnimationRenderer& _Other) = delete;
	GameEngineFBXAnimationRenderer& operator=(GameEngineFBXAnimationRenderer&& _Other) noexcept = delete;

	void SetFBXMesh(const std::string& _Name, std::string _Material) override;
	GameEngineRenderUnit* SetFBXMesh(const std::string& _Name, std::string _Material, size_t _MeshIndex, size_t _SubSetIndex = 0) override;

	void CreateFBXAnimation(const std::string& _AnimationName, const GameEngineRenderingEvent& _Desc, int _Index = 0);


	void ChangeAnimation(const std::string& _AnimationName);

	void Update(float _DeltaTime) override;

	void PauseSwtich();

	// �ִϸ��̼� ���ε�
	// ���� �����ӿ� ���´�.
	void AnimationBindStart(const std::string& _AnimationName, std::function<void(const GameEngineRenderingEvent&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (Animations.end() == Animations.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		Animations[Name]->StartEvent = _Function;
	}
	// ������ �����ӿ� ���´�
	void AnimationBindEnd(const std::string& _AnimationName, std::function<void(const GameEngineRenderingEvent&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (Animations.end() == Animations.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		Animations[Name]->EndEvent = _Function;
	}
	// �������� �ٲ𶧸��� ���´�
	void AnimationBindFrame(const std::string& _AnimationName, std::function<void(const GameEngineRenderingEvent&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (Animations.end() == Animations.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		Animations[Name]->FrameEvent = _Function;
	}
	// Update
	void AnimationBindTime(const std::string& _AnimationName, std::function<void(const GameEngineRenderingEvent&, float)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (Animations.end() == Animations.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		Animations[Name]->TimeEvent = _Function;
	}

	std::shared_ptr<FBXRendererAnimation> GetCurAnimation() { return CurAnimation; }
	std::shared_ptr<FBXRendererAnimation> GetAnimationWithName(const std::string& _strName) 
	{ 
		std::string UpperName = GameEngineString::ToUpperReturn(_strName);

		std::map<std::string, std::shared_ptr<FBXRendererAnimation>>::iterator FindIter = Animations.find(UpperName);

		return FindIter->second;
	}

	bool CheckCurrentAnimation(std::string _strName)
	{
		std::string UpperName = GameEngineString::ToUpperReturn(_strName);

		std::map<std::string, std::shared_ptr<FBXRendererAnimation>>::iterator FindIter = Animations.find(UpperName);

		if (Animations.end() == FindIter)
		{
			MsgBox("�������� �ʴ� �ִϸ��̼��Դϴ�.");
			return false;
		}

		if (FindIter->second == CurAnimation)
		{
			return true;
		}
		return false;

	}

protected:

private:
	bool Pause;

	std::map<std::string, std::shared_ptr<FBXRendererAnimation>> Animations;
	std::shared_ptr<FBXRendererAnimation> CurAnimation;

	std::map<size_t, std::vector<float4x4>> AnimationBoneMatrixs;
	// std::map<size_t, std::vector<float4x4>> PrevAnimationBoneMatrixs;

	std::map<size_t, std::vector<AnimationBoneData>> AnimationBoneDatas;

};

