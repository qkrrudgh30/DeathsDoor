#pragma once
#include <GameEngineCore/GameEngineLevel.h>

enum
{
	eAnimatorDir,
	eStaticDir,
	eEndDir
};

// ���� :
class GameEngineDirectory;
class ContentsLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	ContentsLevel();
	~ContentsLevel();

	// delete Function
	ContentsLevel(const ContentsLevel& _Other) = delete;
	ContentsLevel(ContentsLevel&& _Other) noexcept = delete;
	ContentsLevel& operator=(const ContentsLevel& _Other) = delete;
	ContentsLevel& operator=(ContentsLevel&& _Other) noexcept = delete;
	

protected:
	void DirectPathAt(const std::string& _strFolderName);
	void LoadFBXFiles();			// Ŭ�� �е��� �� �Լ��� ȣ���ϸ� �˾Ƽ� �ش� ������ ��� FBXFile���� �ε� �ǰԲ���.
									// �� �Լ� ������ �� ���� �ε尡 ����ǰ�, �� �ܰ踶�� ���� ���� üũ �� ���� �� / ������ ������ ����.
									// 
	void LoadTextureInAnimator();
	void LoadTextureInStatic();

	std::vector<GameEngineDirectory> mvecDirectories;
	static std::atomic<int> muFBXFolderCount;
	// bool mb

private:
	void LoadFBXMesiesOfAnimator();
	void LoadFBXMesiesOfStatic();
	void LoadAnimationsOfAnimator();
	std::vector<std::string> mstrvecAnimatorMeshFileNames;
	std::vector<std::string> mstrvecStaticMeshFileNames;
	std::vector<std::string> mstrvecAnimationFileNames;

};

