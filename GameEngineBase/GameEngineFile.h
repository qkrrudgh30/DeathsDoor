#pragma once
#include "GameEnginePath.h"
#include "GameEngineMath.h"

enum class OpenMode
{
	Read,
	Write,
};

enum class FileMode
{
	Binary,
	Text,
};

// ���� :
class GameEngineFile : public GameEnginePath
{
public:
	static uintmax_t GetFileSize(const std::filesystem::path& _Path);

	// constrcuter destructer
	GameEngineFile();
	GameEngineFile(const char* _Path);
	GameEngineFile(const std::filesystem::path& _Path);
	GameEngineFile(const GameEngineFile& _Other);
	~GameEngineFile();

	// delete Function
	void Open(OpenMode _Mode, FileMode _Open);

	void Close();

	void Create()
	{
		Open(OpenMode::Write, FileMode::Binary);
		Close();
	}

	void Write(const void* _WriteData, size_t _WriteSize);

	void Write(const std::string& _Text);

	void Write(const float4x4& _Data);

	void Write(const float4& _Data);

	void Write(const double& _Data);

	void Write(const float& _Data);

	template<typename Struct>
	void Write(const Struct& _Data)
	{
		Write(reinterpret_cast<const void*>(&_Data), sizeof(Struct));
	}



	// �޴¿� ����
	void Read(void* _ReadData, size_t _BufferSize, size_t _ReadSize);

	void Read(std::string& _Data);

	void Read(float4x4& _Data);

	void Read(float4& _Data);

	void Read(double& _Data);

	void Read(float& _Data);

	template<typename Struct>
	void Read(Struct& _Data)
	{
		Read(reinterpret_cast<void*>(&_Data), sizeof(Struct), sizeof(Struct));
	}



	std::string GetString();

	uintmax_t GetFileSize() const;


protected:

	


private:
	FILE* FilePtr;
};

