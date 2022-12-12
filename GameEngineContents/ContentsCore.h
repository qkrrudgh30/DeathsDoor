#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCore.h>

#include <GameEngineBase/GameEngineSound.h>

// ���� : ���α׷� ��ü�� �����ϰ� �Ѵ�.
class ContentsCore : public GameEngineCore
{
public:
	// constrcuter destructer
	ContentsCore(); 
	~ContentsCore(); 
	
	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _Other) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;

	// float4 StartWindowSize() override { return float4{ 1920.0f, 1080.0f } * 0.6f; } ;

	float4 StartWindowSize() override { return float4{ 1280, 720.0f }; };
	float4 StartWindowPosition() override { return { 0,0 }; };

	std::string GetWindowTitle() override
	{
		return "DeathDoor";
	}

	static float ReturnFloatLerp(float _fSrc, float _fDst, float _fRatio)
	{
		return _fSrc * (1 - _fRatio) + _fDst * _fRatio;
	}

protected:
	void Start() override;

	// �� ���α׷��� ��� ��Ȳ������ ��� Update�ϸ鼭 üũ������ �ִٸ� ���⼭ �Ѵ�.
	// ������ ���� ������ �׷����� ����. ������ ���� �����̰� ���� ����Ű�� ������ �����Ѵ�.
	// Ÿ��Ʋ�� �ѱ�ų� �̷�
	void Update(float _DeltaTime) override;

	void End() override;

private:
	static void InitializeContentsResource();
	static void DestroyContentsResource();

	void ChangeLevelNumber();

public:

protected:

private:
	static int m_nCurrentLevel;
	static int m_nNextLevel;
	std::weak_ptr<class ContentsLevel> m_wptrCurrentLevel;
	class GameEngineSoundPlayer mstructSoundPlayer;

};

