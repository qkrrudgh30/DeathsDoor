#include "PreCompile.h"
#include "GameEngineBase/GameEngineDirectory.h"
#include "GameEngineCore/GameEngineFBXStaticRenderer.h"

#include "ContentsLevel.h"
#include "EditGUIWindow.h"
#include "StaticMesh.h"
#include "LoadingUI.h"

#include <filesystem>
#include <fstream>

std::atomic<unsigned int> ContentsLevel::muFBXLoadedCount = 0;
std::string ContentsLevel::mstrNextLevelName;
std::string ContentsLevel::mstrPrevLevelName;
std::map<std::string, bool> ContentsLevel::mmapPrimitiveInitialized;
std::vector<std::string> ContentsLevel::mstrvecAllResourceNames;

enum { eWaiting = -1 };

// #define MT
#define NMT

ContentsLevel::ContentsLevel()	
	: mbPrimitiveInitialized(false)
	, mpLoadingUI(nullptr)
{
	mvecDirectories.reserve(64u);
	mstrvecAnimatorMeshFileNames.reserve(64u);
	mstrvecAnimatorMeshFileNamesForEdit.reserve(64u);
	mstrvecStaticMeshFileNames.reserve(64u);
	mstrvecStaticMeshFileNamesForEdit.reserve(64u);
	mstrvecAnimationFileNames.reserve(64u);

	// GameEngineFBX::CreateManager();
}

ContentsLevel::~ContentsLevel() 
{
}

void ContentsLevel::PlacePathOn(const std::string& _strFolderName)
{
	mvecDirectories.clear();

	GameEngineDirectory tempDir;
	tempDir.MoveParentToExitsChildDirectory("ContentsResources");
	tempDir.Move("ContentsResources");
	tempDir.Move("Asset");
	tempDir.Move(_strFolderName);

	std::vector<GameEngineDirectory> vOuterDirectories = tempDir.GetAllDirectory();
	int uOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	mvecDirectories.reserve(uOuterDirectoriesCount);
	for (int i = 0; i < uOuterDirectoriesCount; ++i)
	{
		mvecDirectories.push_back(vOuterDirectories[i]);
	}
}

void ContentsLevel::LoadFBXFiles()
{
	mstrvecAllResourceNames.clear();
	mstrvecAllResourcePaths.clear();
	muAllResourcesCount = 0u;
	muMyThreadCount = GameEngineCore::EngineThreadPool.GetMyThreadCount();

	LoadFBXMesiesOfAnimator();
	LoadFBXMesiesOfStatic();
	LoadAnimationsOfAnimator();

	LoadResources();
}

void ContentsLevel::LoadCreaturesFromFile(const std::string& _strFolderName)
{
	GameEngineDirectory tempDir;
	tempDir.MoveParentToExitsChildDirectory("ContentsResources");
	tempDir.Move("ContentsResources");
	tempDir.Move("Asset");
	tempDir.Move(_strFolderName);

	std::filesystem::path p(tempDir.PlusFilePath(_strFolderName + ".txt"));

	std::ifstream fin;
	fin.open(p);

	int uCount = 0;

	fin >> uCount;

	std::string strName;
	float4 f4Scale, f4Rotation, f4Position;
	float4 f4ColliderScale, f4ColliderRotation, f4ColliderPosition;
	for (int i = 0; i < uCount; ++i)
	{
		fin >> strName
			>> f4Scale.x >> f4Scale.y >> f4Scale.z
			>> f4Rotation.x >> f4Rotation.y >> f4Rotation.z
			>> f4Position.x >> f4Position.y >> f4Position.z
			>> f4ColliderScale.x >> f4ColliderScale.y >> f4ColliderScale.z
			>> f4ColliderRotation.x >> f4ColliderRotation.y >> f4ColliderRotation.z
			>> f4ColliderPosition.x >> f4ColliderPosition.y >> f4ColliderPosition.z;

		StaticMesh* temp = GEngine::GetCurrentLevel()->CreateActor<StaticMesh>();
		temp->SetPriorityInitialize();
		temp->GetFBXRenderer()->SetFBXMesh(strName + ".fbx", "Texture");
		temp->GetTransform().SetLocalScale(f4Scale);
		temp->GetTransform().SetLocalRotate(f4Rotation);
		temp->GetTransform().SetLocalPosition(f4Position);

		temp->GetCollider()->GetTransform().SetLocalScale(f4ColliderScale);
		temp->GetCollider()->GetTransform().SetLocalRotation(f4ColliderRotation);
		temp->GetCollider()->GetTransform().SetLocalPosition(f4ColliderPosition);

		std::string strCurrLevelName = GEngine::GetCurrentLevel()->GetNameCopy();
		EditGUIWindow::GetCreatureMap()[strCurrLevelName].push_back(make_pair(strName, temp));
		int a = 100;
	}
}

void ContentsLevel::LoadFBXMesiesOfAnimator()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eAnimatorDir].GetAllDirectory();
	size_t uOuterDirectoriesCount = vOuterDirectories.size();
	muAllAnimatorCount = uOuterDirectoriesCount;
	muAllResourcesCount += uOuterDirectoriesCount;
	size_t uThreadCount = GameEngineCore::EngineThreadPool.GetMyThreadCount();
	// size_t uThreadCount = 6u;
	size_t uLines = static_cast<size_t>(uOuterDirectoriesCount / uThreadCount);
	size_t uRemains = uOuterDirectoriesCount % uThreadCount;
	muFBXLoadedCount = 0u;

	mstrvecAnimatorMeshFileNames.clear();
	mstrvecAnimatorMeshFileNames.reserve(uOuterDirectoriesCount);
	mstrvecAnimatorMeshFileNamesForEdit.clear();
	mstrvecAnimatorMeshFileNamesForEdit.reserve(uOuterDirectoriesCount);
	for (size_t i = 0; i < uOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecAnimatorMeshFileNamesForEdit.push_back(strTemp);
		mstrvecAllResourceNames.push_back(strTemp);
		mstrvecAnimatorMeshFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
		mstrvecAllResourcePaths.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

	LoadingUI::mbIsFirstLoadingStage = true;

}

void ContentsLevel::LoadFBXMesiesOfStatic()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eStaticDir].GetAllDirectory();
	size_t uOuterDirectoriesCount = vOuterDirectories.size();
	muAllStaticCount = uOuterDirectoriesCount;
	muAllResourcesCount += uOuterDirectoriesCount;
	size_t uThreadCount = GameEngineCore::EngineThreadPool.GetMyThreadCount();
	// size_t uThreadCount = 6u;
	size_t uLines = static_cast<size_t>(uOuterDirectoriesCount / uThreadCount);
	size_t uRemains = uOuterDirectoriesCount % uThreadCount;
	muFBXLoadedCount = 0u;

	mstrvecStaticMeshFileNames.clear();
	mstrvecStaticMeshFileNames.reserve(uOuterDirectoriesCount);
	mstrvecStaticMeshFileNamesForEdit.clear();
	mstrvecStaticMeshFileNamesForEdit.reserve(uOuterDirectoriesCount);
	for (size_t i = 0; i < uOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecStaticMeshFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
		mstrvecStaticMeshFileNamesForEdit.push_back(strTemp);
		mstrvecAllResourceNames.push_back(strTemp);
		mstrvecAllResourcePaths.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

	LoadingUI::mbIsFirstLoadingStage = false;

}

void ContentsLevel::LoadAnimationsOfAnimator()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eAnimatorDir].GetAllDirectory();
	size_t uOuterDirectoriesCount = vOuterDirectories.size();
	muAllAnimationCount = uOuterDirectoriesCount;
	muAnimationStartIndex = muAllResourcesCount;
	muAllResourcesCount += uOuterDirectoriesCount;
	// int uThreadCount = GameEngineCore::EngineThreadPool.GetThreadCount();
	size_t uThreadCount = 6;
	size_t uLines = static_cast<size_t>(uOuterDirectoriesCount / uThreadCount);
	size_t uRemains = uOuterDirectoriesCount % uThreadCount;
	muFBXLoadedCount = 0u;

	mstrvecAnimationFileNames.clear();
	mstrvecAnimationFileNames.reserve(uOuterDirectoriesCount);
	for (size_t i = 0; i < uOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecAnimationFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
		mstrvecAllResourcePaths.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

}

void ContentsLevel::LoadResources()
{
	// size_t uThreadCount = GameEngineCore::EngineThreadPool.GetThreadCount();
	muMyThreadCount = 6u;
	muLines = static_cast<size_t>(muAllResourcesCount / muMyThreadCount);
	muRemains = muAllResourcesCount % muMyThreadCount;
	muFBXLoadedCount = 0u;

	size_t i = 0, j = 0, k = 0, l = 0;
	for (i = 0; i < muLines; ++i) // ���� ���� ���, �� uLines * uThreadCount ������ ��ȸ. 1�ٵ� �ȵǴ� ��쿣 �ڵ����� �Ѿ�Բ�. 
	{
		for (j = 0; j < muMyThreadCount; ++j)
		{
			l = i * muMyThreadCount + j;
			if (l < muAnimationStartIndex)
			{
				if (l < muAllAnimatorCount)
				{
					EditGUIWindow::GetLoadedFromAnimatorSet().insert(mstrvecAllResourceNames[l]);
				}
				else
				{
					EditGUIWindow::GetLoadedFromStaticSet().insert(mstrvecAllResourceNames[l]);
				}

				GameEngineCore::EngineThreadPool.Work(
					[=]
					{
						GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAllResourcePaths[l]);
						mpLoadingUI->SetProgressAmount(muAllResourcesCount, ++muFBXLoadedCount);
					});
			}
			else
			{
				GameEngineCore::EngineThreadPool.Work(
					[=]
					{
						GameEngineFBXAnimation* Mesh = GameEngineFBXAnimation::Load(mstrvecAllResourcePaths[l]);
						mpLoadingUI->SetProgressAmount(muAllResourcesCount, ++muFBXLoadedCount);
					});
			}
		}
	}

	if (0 != muRemains) // 1�ٵ� �ȵǴ� ���� ���� �������� ������ FBX ������ �ִ� ���.
	{
		for (k = 0; k < muRemains; ++k)
		{
			l = i * muMyThreadCount + k;
			if (i * muMyThreadCount + k < muAnimationStartIndex)
			{
				if (l < muAllAnimatorCount)
				{
					EditGUIWindow::GetLoadedFromAnimatorSet().insert(mstrvecAllResourceNames[l]);
				}
				else
				{
					EditGUIWindow::GetLoadedFromStaticSet().insert(mstrvecAllResourceNames[l]);
				}

				GameEngineCore::EngineThreadPool.Work(
					[=]
					{
						GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAllResourcePaths[l]);
						mpLoadingUI->SetProgressAmount(muAllResourcesCount, ++muFBXLoadedCount);
					});
			}
			else
			{
				GameEngineCore::EngineThreadPool.Work(
					[=]
					{
						GameEngineFBXAnimation* Mesh = GameEngineFBXAnimation::Load(mstrvecAllResourcePaths[l]);
						mpLoadingUI->SetProgressAmount(muAllResourcesCount, ++muFBXLoadedCount);
					});
			}
		}
	}

}
