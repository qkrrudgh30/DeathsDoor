#pragma once
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"

class FbxExAniData;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class FBXRendererAnimation : public std::enable_shared_from_this<FBXRendererAnimation>
{
public:
	// SetFBX ���� ������ �ִ� �Ž�
	GameEngineFBXMesh* Mesh;
	// �ִϸ��̼��� ������ �ִ� FBX
	GameEngineFBXAnimation* Aniamtion;
	FbxExAniData* FBXAnimationData;

	// �� �ִϸ��̼��� ������� �󸶳� ������.
	float CurFrameTime;
	float FrameTime; // 0.1
	UINT CurFrame;
	UINT Start;
	UINT End;

public:
	FBXRendererAnimation() 
	{
		int a = 0;
	}

	~FBXRendererAnimation()
	{
		int a = 0;
	}
};

// ���� :
class GameEngineFBXAnimationRenderer : public GameEngineFBXRenderer
{
public:
	// constrcuter destructer
	GameEngineFBXAnimationRenderer();
	~GameEngineFBXAnimationRenderer();

	// delete Function
	GameEngineFBXAnimationRenderer(const GameEngineFBXAnimationRenderer& _Other) = delete;
	GameEngineFBXAnimationRenderer(GameEngineFBXAnimationRenderer&& _Other) noexcept = delete;
	GameEngineFBXAnimationRenderer& operator=(const GameEngineFBXAnimationRenderer& _Other) = delete;
	GameEngineFBXAnimationRenderer& operator=(GameEngineFBXAnimationRenderer&& _Other) noexcept = delete;

	void SetFBXMesh(const std::string& _Name, std::string _Material);
	void SetFBXMesh(const std::string& _Name, std::string _Material, size_t _MeshIndex, size_t _SubSetIndex = 0);

	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBX);

protected:

private:
	std::map<std::string, std::shared_ptr<FBXRendererAnimation>> Animations;
	std::shared_ptr<FBXRendererAnimation> CurAnimation;

};

