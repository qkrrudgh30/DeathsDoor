#include "PreCompile.h"
#include "GameEngineNetServer.h"


GameEngineNetServer::GameEngineNetServer()
{
}

GameEngineNetServer::~GameEngineNetServer()
{
}

void GameEngineNetServer::Accept(int Port)
{
	GameEngineNet::WindowNetStartUp();

	// windowapi���� ��������� �Ϸ��� ������ ������ �켱������ �������� �Ѵ�.

	// 00000001 00000000 00000000 00000000
	// int a = 1;

	// 
	SOCKADDR_IN Add;
	Add.sin_family = AF_INET; // ip4 �ּ�

	// 1   1   1   1     2
	// 255.255.255.255 + 30001
	// 127.0.0.1 + 30001
	// 1.0.0.127 + 30001

	Add.sin_port = htons(Port);

	// sin_addr
	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
	{
		return;
	}

	// ������ ���� �ּҷ� ������ �� ����
	// ServerAccpetSocket = socket(AF_INET, SOCKET_);

}