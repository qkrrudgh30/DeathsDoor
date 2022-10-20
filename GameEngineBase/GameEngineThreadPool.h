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

// ���� :
class GameEngineThreadPool
{
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
	void Work(std::function<void()> _Job);

	void Initialize(const std::string& _ThreadName, int _ThreadCount = 0);

protected:

private:
	HANDLE IocpHandle;
	int ThreadCount;
	std::atomic<bool> IsRun;

	static void ThreadPoolFunction(GameEngineThread* _Thread, HANDLE _IocpHandle, std::atomic<bool>* _Run);

	std::vector<GameEngineThread*> Threads;

};

// extern GameEngineThreadPool EngineThreadPool;