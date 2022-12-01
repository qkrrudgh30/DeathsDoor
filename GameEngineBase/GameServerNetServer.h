#pragma once
#include "GameServerNet.h"
#include "GameEngineThread.h"
#include "GameServerDispatcher.h"
// ���� :
class GameServerNetServer : public GameServerNet
{
public:

	// constrcuter destructer
	GameServerNetServer();
	~GameServerNetServer();

	// delete Function
	//GameServerNetServer(const GameServerNetServer& _Other) = delete;
	//GameServerNetServer(GameServerNetServer&& _Other) noexcept = delete;
	//GameServerNetServer& operator=(const GameServerNetServer& _Other) = delete;
	//GameServerNetServer& operator=(GameServerNetServer&& _Other) noexcept = delete;

	// �������� ���� �����ڸ� �ްڴ�.
	void Accept(int Port);

	int Send(const char* Data, size_t _Size) {
		return 0;
	}

	int SendPacket(std::shared_ptr<GameServerPacket> _Packet) {
		return 0;
	}

protected:


private:
	// �����ڸ� �ް� ������ �⵿��Ű�ڴ�.
	// 3�� ���
	// SOCKET
	// accpet�� �ְ�
	// connect�� �ִ�.

	// SOCKET�� ������ ũ�� 2������ �����ϴµ�.
	// 1. ���� � ��ǻ�Ϳ��� ������ �Ǿ��ִ����� ���� �ڵ�
	// 2. ���� ���� ������ ��ǥ�ϴ� 

	std::atomic<bool> IsRun;

	SOCKET ServerAccpetSocket;

	GameEngineThread AcceptThread;

	std::vector<SOCKET> UserSockets;
	std::vector<GameEngineThread> UserThreads;

	void AcceptFunction(GameEngineThread* Thread);

	void UserFunction(GameEngineThread* Thread, SOCKET _Socket);

};

