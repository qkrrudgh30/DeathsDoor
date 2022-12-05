#pragma once
#include "GameServerSerializer.h"
#include "GameEngineDebug.h"

#define	GameServerPacketError -1

// MonsterMovePacket : public GameServerPacket
// PlayerReady : public GameServerPacket

// ���� :
class GameServerPacket : public std::enable_shared_from_this<GameServerPacket>
{
public:

	// delete Function
	GameServerPacket(const GameServerPacket& _Other) = delete;
	GameServerPacket(GameServerPacket&& _Other) noexcept = delete;
	GameServerPacket& operator=(const GameServerPacket& _Other) = delete;
	GameServerPacket& operator=(GameServerPacket&& _Other) noexcept = delete;


	template<typename ConvertType>
	std::shared_ptr<ConvertType> DynamicConvert()
	{
		return std::dynamic_pointer_cast<ConvertType>(shared_from_this());
	}


	unsigned int GetPacketID()
	{
		return PacketID;
	}

	template<typename EnumType>
	EnumType GetPacketIDToEnum()
	{
		return static_cast<EnumType>(PacketID);
	}

	template<typename EnumType>
	void SetPacketID(EnumType _Enum)
	{
		PacketID = static_cast<int>(_Enum);
	}

	SOCKET GetMaster()
	{
		return Master;
	}

	void SetMaster(SOCKET _Master)
	{
		if (Master != -1)
		{
			return;
		}

		Master = _Master;
	}


	void SerializePacket(GameServerSerializer& _Ser)
	{
		Serialize(_Ser);
		SerializeEnd(_Ser);
	}

	void DeSerializePacket(GameServerSerializer& _Ser)
	{
		DeSerialize(_Ser);
	}


	GameServerPacket() 
		: PacketID(GameServerPacketError)
		, Master(-1)
	{

	}

	GameServerPacket(int _PacketType)
		: PacketID(_PacketType)
		, Master(-1)
	{

	}

	~GameServerPacket() 
	{

	}

protected:

	virtual void Serialize(GameServerSerializer& _Ser)  = 0
	{
		_Ser << PacketID;
		_Ser << Size;
		_Ser << Master;
	}
	virtual void DeSerialize(GameServerSerializer& _Ser) = 0
	{
		_Ser >> PacketID;
		_Ser >> Size;
		_Ser >> Master;
	}



private:
	unsigned int PacketID; // ��Ŷ�� ����
	unsigned int Size; // ��Ŷ�� ũ��
	SOCKET Master; // ��Ŷ�� ũ��


	void SerializeEnd(GameServerSerializer& _Ser) 
	{
		Size = _Ser.GetOffSet();

		if (Size == 0)
		{
			MsgBoxAssert("0����Ʈ ��Ŷ�� �����Ǿ����ϴ�.");
		}

		unsigned char* Ptr = _Ser.GetDataPtr();

		memcpy_s(&Ptr[4], sizeof(int), &Size, sizeof(int));

		// _Ser.GetDataPtr[]
	}
};

