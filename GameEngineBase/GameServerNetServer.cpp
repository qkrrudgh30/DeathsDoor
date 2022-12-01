#include "PreCompile.h"
#include "GameServerNetServer.h"
#include "GameEngineString.h"
#include "GameEngineDebug.h"
#include "GameEngineThread.h"
#include "GameServerSerializer.h"

//enum PacketType
//{
//	Move,
//	Attack,
//};
//
//class Pakcet {
//	char String[100] 
//};


GameServerNetServer::GameServerNetServer()
{
}

GameServerNetServer::~GameServerNetServer()
{
	for (size_t i = 0; i < UserThreads.size(); i++)
	{
		closesocket(UserSockets[i]);
		UserSockets[i] = 0;
		UserThreads[i].Join();
	}

	if (0 != ServerAccpetSocket)
	{
		closesocket(ServerAccpetSocket);
		ServerAccpetSocket = 0;
		AcceptThread.Join();
	}
}

void GameServerNetServer::Accept(int Port)
{
	GameServerNet::WindowNetStartUp();

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

	// OSI 7�����̶�� ��� �Ծ࿡ ���ؼ�
	// �����ܰ�
	// ��ī��ܰ�

	// SOCK_STREAM ����������
	// SOCK_DRAM UDP
	// ��������

	// �ּҸ� ����� �̴ϴ�.
	ServerAccpetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == ServerAccpetSocket)
	{
		return;
	}

	if (SOCKET_ERROR == bind(ServerAccpetSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
	{
		return;
	}

	if (SOCKET_ERROR == listen(ServerAccpetSocket, 512))
	{
		return;
	}

	AcceptThread.Start("AcceptThread", std::bind(&GameServerNetServer::AcceptFunction, this, &AcceptThread));
}


void GameServerNetServer::AcceptFunction(GameEngineThread* Thread) 
{
	// �����忡�� ���� ���ư� �̴ϴ�.
	while (ServerAccpetSocket)
	{
		SOCKADDR_IN UserAddress;

		int AddressLen = sizeof(SOCKADDR_IN);

		// accept�� ȣ���ؾ� �����ڸ� �޴°ɷ� �����ϴµ� �ƴϾ�.
		SOCKET NewUser = accept(ServerAccpetSocket, (sockaddr*)&UserAddress, &AddressLen);

		// SOCKET NewUser
		if (-1 == NewUser)
		{
			return;
		}

		GameEngineThread& NewThread = UserThreads.emplace_back();
		std::stringstream ThreadName;
		ThreadName << NewUser;
		ThreadName << "UserThread";
		NewThread.Start(ThreadName.str(), std::bind(&GameServerNetServer::UserFunction, this, &NewThread, NewUser));
		UserSockets.push_back(NewUser);


		// ó���ؾ��Ҳ� ���ٰ� �صѲ���.

	}
}

void GameServerNetServer::UserFunction(GameEngineThread* Thread, SOCKET _Socket)
{
	char Packet[1024] = { 0 };

	while (true)
	{
		int Result = recv(_Socket, Packet, sizeof(Packet), 0);

		if (-1 == Result)
		{
			// MsgBoxAssert("��Ʈ��ũ ����");
			// ������ ������.
			// ��밡 ������.
			return;
		}

		GameServerSerializer Ser = GameServerSerializer(Packet, 1024);

		int PacketType;
		int PacketSize;

		memcpy_s(&PacketType, sizeof(int), Ser.GetDataPtr(), sizeof(int));
		memcpy_s(&PacketSize, sizeof(int), Ser.GetDataPtr() + 4, sizeof(int));

		std::shared_ptr<GameServerPacket> Packet = Dis.PacketReturnCallBack(PacketType, PacketSize, Ser);

		Dis.ProcessPacket(Packet);
	}
}