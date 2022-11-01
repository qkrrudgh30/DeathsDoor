#include "PreCompile.h"
#include "GameEngineBase/GameEngineDirectory.h"

#include "ContentsLevel.h"


std::atomic<int> ContentsLevel::muFBXFolderCount = 0;

ContentsLevel::ContentsLevel() 
{
}

ContentsLevel::~ContentsLevel() 
{
}

void ContentsLevel::DirectPathAt(const std::string& _strFolderName)
{
	GameEngineDirectory tempDir;
	tempDir.MoveParentToExitsChildDirectory("ContentsResources");
	tempDir.Move("ContentsResources");
	tempDir.Move("Texture");
	tempDir.Move(_strFolderName);

	std::vector<GameEngineDirectory> vOuterDirectories = tempDir.GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	mvecDirectories.reserve(iOuterDirectoriesCount);
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		mvecDirectories.push_back(vOuterDirectories[i]);
	}
}

void ContentsLevel::LoadFBXFiles()
{
	LoadFBXMesiesOfAnimator();
	LoadFBXMesiesOfStatic();
	// LoadAnimationsOfAnimator();
}

void ContentsLevel::LoadTextureInAnimator()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eAnimatorDir].GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	GameEngineDirectory actorDir;
	
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		actorDir.Move(vOuterDirectories[i].GetFullPath());
		// std::lock_guard<std::mutex> LockInst(m);
		GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
		actorDir.Move("..\\");
		++muFBXFolderCount;
	}
}

void ContentsLevel::LoadTextureInStatic()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eStaticDir].GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	GameEngineDirectory mapDir;
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mapDir.Move(vOuterDirectories[i].GetFullPath());
		// std::lock_guard<std::mutex> LockInst(m);
		GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
		mapDir.Move("..\\");
		++muFBXFolderCount;
	}
}

void ContentsLevel::LoadFBXMesiesOfAnimator()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eAnimatorDir].GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	// int iThreadCount = GameEngineCore::EngineThreadPool.GetThreadCount();
	int iThreadCount = 6;
	int iLines = iOuterDirectoriesCount / iThreadCount;
	int iRemains = iOuterDirectoriesCount % iThreadCount;

	// Create FileNames From Path.
	// mapDir.Move(vOuterDirectories[i].GetFullPath());
	// �� ��ȸ���� strTemp�� ����Ǵ� ����.. -> ��Ƽ �������� ��ΰ� ��� �����.
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecAnimatorMeshFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

	// 1�ٵ� �ȵ� ��
	// ���� �� �ε�, ������ ��������
	// ���� �� �ε�, ������ �������� ������

	GameEngineDirectory mapDir;
	for (int i = 0; i < iLines; ++i) // ���� ���� ���, �� iLines * iThreadCount ������ ��ȸ. 1�ٵ� �ȵǴ� ��쿣 �ڵ����� �Ѿ�Բ�. 
	{
		for (int j = 0; j < iThreadCount; ++j)
		{
			//GameEngineCore::EngineThreadPool.Work(
			//	[&]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i * iThreadCount + j]);
			//		++muFBXFolderCount;
			//	});

			GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i * iThreadCount + j]);
		}
	}

	if (0 != iRemains) // 1�ٵ� �ȵǴ� ���� ���� �������� ������ FBX ������ �ִ� ���.
	{
		for (int i = 0; i < iRemains; ++i)
		{

			//GameEngineCore::EngineThreadPool.Work(
			//	[=]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i]);
			//		++muFBXFolderCount;
			//	});

			GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i]);
		}
	}
}

void ContentsLevel::LoadFBXMesiesOfStatic()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eStaticDir].GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	// int iThreadCount = GameEngineCore::EngineThreadPool.GetThreadCount();
	int iThreadCount = 6;
	int iLines = iOuterDirectoriesCount / iThreadCount;
	int iRemains = iOuterDirectoriesCount % iThreadCount;

	// Create FileNames From Path.
	// mapDir.Move(vOuterDirectories[i].GetFullPath());
	// �� ��ȸ���� strTemp�� ����Ǵ� ����.. -> ��Ƽ �������� ��ΰ� ��� �����.
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecStaticMeshFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

	// 1�ٵ� �ȵ� ��
	// ���� �� �ε�, ������ ��������
	// ���� �� �ε�, ������ �������� ������

	GameEngineDirectory mapDir;
	for (int i = 0; i < iLines; ++i) // ���� ���� ���, �� iLines * iThreadCount ������ ��ȸ. 1�ٵ� �ȵǴ� ��쿣 �ڵ����� �Ѿ�Բ�. 
	{
		for (int j = 0; j < iThreadCount; ++j)
		{
			//GameEngineCore::EngineThreadPool.Work(
			//	[&]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i * iThreadCount + j]);
			//		++muFBXFolderCount;
			//	});

			GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecStaticMeshFileNames[i * iThreadCount + j]);
		}
	}

	if (0 != iRemains) // 1�ٵ� �ȵǴ� ���� ���� �������� ������ FBX ������ �ִ� ���.
	{
		for (int i = 0; i < iRemains; ++i)
		{

			//GameEngineCore::EngineThreadPool.Work(
			//	[=]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i]);
			//		++muFBXFolderCount;
			//	});

			GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecStaticMeshFileNames[i]);
		}
	}
}

void ContentsLevel::LoadAnimationsOfAnimator()
{
	std::vector<GameEngineDirectory> vOuterDirectories = mvecDirectories[eAnimatorDir].GetAllDirectory();
	int iOuterDirectoriesCount = static_cast<int>(vOuterDirectories.size());
	// int iThreadCount = GameEngineCore::EngineThreadPool.GetThreadCount();
	int iThreadCount = 6;
	int iLines = iOuterDirectoriesCount / iThreadCount;
	int iRemains = iOuterDirectoriesCount % iThreadCount;

	// Create FileNames From Path.
	// mapDir.Move(vOuterDirectories[i].GetFullPath());
	// �� ��ȸ���� strTemp�� ����Ǵ� ����.. -> ��Ƽ �������� ��ΰ� ��� �����.
	for (int i = 0; i < iOuterDirectoriesCount; ++i)
	{
		std::string strTemp = vOuterDirectories[i].GetFileName();
		mstrvecAnimationFileNames.push_back(vOuterDirectories[i].PlusFilePath(strTemp + ".FBX"));
	}

	// 1�ٵ� �ȵ� ��
	// ���� �� �ε�, ������ ��������
	// ���� �� �ε�, ������ �������� ������

	GameEngineDirectory mapDir;
	for (int i = 0; i < iLines; ++i) // ���� ���� ���, �� iLines * iThreadCount ������ ��ȸ. 1�ٵ� �ȵǴ� ��쿣 �ڵ����� �Ѿ�Բ�. 
	{
		for (int j = 0; j < iThreadCount; ++j)
		{
			//GameEngineCore::EngineThreadPool.Work(
			//	[&]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh* Mesh = GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i * iThreadCount + j]);
			//		++muFBXFolderCount;
			//	});
			GameEngineFBXAnimation* Animation = GameEngineFBXAnimation::Load(mstrvecAnimationFileNames[i * iThreadCount + j]);
		}
	}

	if (0 != iRemains) // 1�ٵ� �ȵǴ� ���� ���� �������� ������ FBX ������ �ִ� ���.
	{
		for (int i = 0; i < iRemains; ++i)
		{

			//GameEngineCore::EngineThreadPool.Work(
			//	[=]
			//	{
			//		// std::lock_guard<std::mutex> LockInst(m);
			//		GameEngineFBXMesh::Load(mstrvecAnimatorMeshFileNames[i]);
			//		++muFBXFolderCount;
			//	});

			GameEngineFBXAnimation* Animation = GameEngineFBXAnimation::Load(mstrvecAnimationFileNames[i]);
		}
	}
}
