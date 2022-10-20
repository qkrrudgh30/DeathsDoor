#include "PreCompile.h"
#include "GameEngineThreadPool.h"
#include "GameEngineDebug.h"


GameEngineThreadPool::GameEngineThreadPool(const std::string& _ThreadName, int _ThreadCount /*= 0*/)
{
	Initialize(_ThreadName, _ThreadCount);
}

GameEngineThreadPool::GameEngineThreadPool()
{

}

void GameEngineThreadPool::Initialize(const std::string& _ThreadName, int _ThreadCount /*= 0*/)
{
	// �� �ھ�� *2�� �������� ���� ���̾��ִ��� �����
// ...

	ThreadCount = _ThreadCount;

	if (0 == ThreadCount)
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		ThreadCount = Info.dwNumberOfProcessors;
	}

	// ���ʿ� ����� iocp�ڵ��� 
	// CreateIoCompletionPort�� �������� ������ �ִµ�
	IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);

	if (nullptr == IocpHandle)
	{
		MsgBoxAssert("IOCP �ڵ� ������ �����߽��ϴ�.");
		return;
	}

	IsRun = true;

	for (size_t i = 0; i < ThreadCount; i++)
	{
		GameEngineThread* NewThread = new GameEngineThread();

		Threads.push_back(NewThread);

		NewThread->Start(_ThreadName + std::to_string(i)
			, std::bind(ThreadPoolFunction, NewThread, IocpHandle, &IsRun));
	}

}

void GameEngineThreadPool::ThreadPoolFunction(GameEngineThread* _Thread, HANDLE _IocpHandle, std::atomic<bool>* _Run)
{
	//_In_ HANDLE CompletionPort,
	//_Out_ LPDWORD lpNumberOfBytesTransferred, 
	//_Out_ PULONG_PTR lpCompletionKey,
	//_Out_ LPOVERLAPPED* lpOverlapped,
	//_In_ DWORD dwMilliseconds

	while (*_Run)
	{
	//	GetQueuedCompletionStatus(_IocpHandle,);

		//Sleep(1);

		// ���� ���������� �����带 ������.

		//if (���� ���ٸ�)
		//{
		//	continue;
		//}

		//���ϴ� �ڵ�
	}
}

GameEngineThreadPool::~GameEngineThreadPool() 
{
	IsRun = false;

	for (size_t i = 0; i < Threads.size(); i++)
	{
		Threads[i]->Join();
		delete Threads[i];
	}
}


void GameEngineThreadPool::Work(std::function<void()> _Job) 
{

}

