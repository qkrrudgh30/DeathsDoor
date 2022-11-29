#include "PreCompile.h"
#include "GameEngineNet.h"
#include "GameEngineDebug.h"

GameEngineNet::GameEngineNet() 
{
}

GameEngineNet::~GameEngineNet() 
{
}

void GameEngineNet::WindowNetStartUp()
{
	static bool IsStart = false;

	if (true == IsStart)
	{
		return;
	}

	WSAData wsaData;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// winapi������ �̰� �������� ������ ������� ��ü�� ���ϰ� ���Ƴ���.
	if (SOCKET_ERROR == errorCode)
	{
		MsgBoxAssert("winapi server socket startup fail");
		return;
	}

	IsStart = true;
}

int GameEngineNet::Send(SOCKET _Socket, const char* Data, size_t _Size)
{
	return send(_Socket, Data, static_cast<int>(_Size), 0);
}

int GameEngineNet::Send(const char* Data, size_t _Size)
{
	return 0;
}