#pragma once
#include "GameEngineNet.h"
// ���� :
class GameEngineNetServer : public GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNetServer();
	~GameEngineNetServer();

	// delete Function
	GameEngineNetServer(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer(GameEngineNetServer&& _Other) noexcept = delete;
	GameEngineNetServer& operator=(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer& operator=(GameEngineNetServer&& _Other) noexcept = delete;

	void Accept(int Port);

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
	SOCKET ServerAccpetSocket;

};

