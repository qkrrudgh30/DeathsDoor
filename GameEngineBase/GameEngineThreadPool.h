#pragma once
#include <functional>
#include "GameEngineThread.h"
#include <atomic>
// Window�� ���� ������ 

// obejct pool memory pool
// obejct pool�� Ư���� ����� ������.
// �츮 �������� ���������� �����ϸ�
// ���� 100���� ����� off ���ѳ������� �ʿ��Ҷ����� on�ϸ� �̰� pool ����̶�� �Ѵ�.

// �׷��� �̸� ��ü�� ���� ����� ���� ����ų� ���̻� ������ �ʰ�
// �ʿ��Ҷ� ���� ����ϴ� ��� => pool ���

enum ThreadWorkType
{
	UserWork = -1,
	Destroy = -2,
};


// ���� :
class GameEngineThreadPool
{
private:
	class GameEngineThreadJob
	{
	public:
		virtual void Process() = 0;
	};

	class GameEngineThreadCallBackJob : public GameEngineThreadJob
	{
	public:
		std::function<void()> Work;

		void Process()
		{
			if (nullptr == Work)
			{
				return;
			}
			Work();
		}
	};


public:
	// constrcuter destructer
	GameEngineThreadPool(const std::string& _ThreadName, int _ThreadCount = 0);

	GameEngineThreadPool();

	~GameEngineThreadPool();

	// delete Function
	GameEngineThreadPool(const GameEngineThreadPool& _Other) = delete;
	GameEngineThreadPool(GameEngineThreadPool&& _Other) noexcept = delete;
	GameEngineThreadPool& operator=(const GameEngineThreadPool& _Other) = delete;
	GameEngineThreadPool& operator=(GameEngineThreadPool&& _Other) noexcept = delete;

	// �Լ��� �ѱ�� �����尡 �˾Ƽ� ó���Ѵ�.
	void Work(std::function<void()> _CallBack);

	void Initialize(const std::string& _ThreadName, int _ThreadCount = 0);

protected:

private:
	HANDLE IocpHandle;
	int ThreadCount;
	std::atomic<bool> IsRun;
	std::atomic<int> DestroyThreadCount;

	static void ThreadPoolFunction(GameEngineThreadPool* _ThreadPool, GameEngineThread* _Thread, HANDLE _IocpHandle);

	std::vector<GameEngineThread*> Threads;

};

// extern GameEngineThreadPool EngineThreadPool;