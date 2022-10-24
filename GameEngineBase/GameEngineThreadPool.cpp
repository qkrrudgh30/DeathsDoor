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
	DestroyThreadCount = 0;

	for (size_t i = 0; i < ThreadCount; i++)
	{
		GameEngineThread* NewThread = new GameEngineThread();

		Threads.push_back(NewThread);

		NewThread->SetName(_ThreadName + std::to_string(i));
		NewThread->Start(_ThreadName + std::to_string(i)
			, std::bind(ThreadPoolFunction, this, NewThread, IocpHandle));
	}

}

void GameEngineThreadPool::ThreadPoolFunction(GameEngineThreadPool* _ThreadPool, GameEngineThread* _Thread, HANDLE _IocpHandle)
{
	//_In_ HANDLE CompletionPort,
	// // io�۾��� �Ҷ� �󸶳� ���޹޾Ҵ����� �����ش�.
	// // ���� io�۾��� ���� �ʴ´ٸ� ���� ���� �־��ټ��� �ִ�.
	//_Out_ LPDWORD lpNumberOfBytesTransferred, 
	// 
	// 8����Ʈ�� �� �Ͽ� ���õ� ����� �������� 8����Ʈ ������ �־��ټ� �ִ�.
	// ���� ���Ƿ� �־��ټ� �ִ� Ű.
	//_Out_ PULONG_PTR lpCompletionKey,
	// 
	// �񵿱� io�۾��� �����쿡�� ��û�ϴ� ����ü ���� �����Ҵ��ؼ� �־���� �Ѵ�.
	//_Out_ LPOVERLAPPED* lpOverlapped,
	//_In_ DWORD dwMilliseconds


	DWORD Byte;
	ULONG_PTR CompletionKey;
	LPOVERLAPPED lpOverlapped;

	while (_ThreadPool->IsRun)
	{
		GetQueuedCompletionStatus(_IocpHandle, &Byte, &CompletionKey, &lpOverlapped, INFINITE);

		ThreadWorkType WorkType = static_cast<ThreadWorkType>(Byte);
		switch (WorkType)
		{
		case UserWork:
		{
			GameEngineThreadCallBackJob* Job = reinterpret_cast<GameEngineThreadCallBackJob*>(CompletionKey);
			Job->Process();
			delete Job;
			break;
		}
		case Destroy:
			++_ThreadPool->DestroyThreadCount;
			return;
		default:
			break;
		}
		// Sleep(1);
		// ���ϴ� ��.
	}
}

GameEngineThreadPool::~GameEngineThreadPool() 
{
	IsRun = false;

	while (true)
	{
		if (FALSE == PostQueuedCompletionStatus(
			IocpHandle,
			static_cast<DWORD>(ThreadWorkType::Destroy),
			0,
			nullptr)
			)
		{
			MsgBoxAssert("�����忡�� �ݹ����� ��û�ϴµ� �����߽��ϴ�.");
		}

		Sleep(1);

		if (DestroyThreadCount == ThreadCount)
		{
			break;
		}
	}

	for (size_t i = 0; i < Threads.size(); i++)
	{
		Threads[i]->Join();
		delete Threads[i];
	}
}


void GameEngineThreadPool::Work(std::function<void()> _CallBack) 
{
	if (nullptr == _CallBack)
	{
		MsgBoxAssert("��ȿ���� ���� �Լ� ������ �Դϴ�. �� �Լ������ʹ� �����忡�� �ñ�� �����ϴ�.");
	}

	GameEngineThreadCallBackJob* NewJob = new GameEngineThreadCallBackJob();

	NewJob->Work = _CallBack;

	// iocp���� �ϳ��� ���ø��� Ű�� �����ϸ鼭 1���� �����带 �����..
	if (FALSE == PostQueuedCompletionStatus(
		IocpHandle, 
		static_cast<DWORD>(ThreadWorkType::UserWork), 
		reinterpret_cast<ULONG_PTR>(NewJob), 
		nullptr)
		)
	{
		MsgBoxAssert("�����忡�� �ݹ����� ��û�ϴµ� �����߽��ϴ�.");
	}
}

