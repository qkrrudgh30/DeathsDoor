#include "PreCompile.h"
#include "GameEngineMesh.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

GameEngineMesh::GameEngineMesh() 
	: VertexBuffer(nullptr)
	, IndexBuffer(nullptr)
{
	SetInputAssembler1VertexBuffer("rect");
	SetInputAssembler2IndexBuffer("rect");
}

GameEngineMesh::~GameEngineMesh() 
{
}

const GameEngineLayOutDesc& GameEngineMesh::GetLayOutDesc()
{
	if (nullptr == VertexBuffer)
	{
		MsgBoxAssert("���ؽ� ���۰� ������ �Ǿ����� �ʾƼ� ���̾ƿ� ������ ������ �����ϴ�.");
	}

	return VertexBuffer->GetLayOutDesc();
}

GameEngineMesh* GameEngineMesh::Create(const std::string& _Name)
{
	return Create(_Name, _Name, _Name);
}

GameEngineMesh* GameEngineMesh::Create(const std::string& _Name, const std::string& _Vtx, const std::string& _Idx)
{
	GameEngineMesh* NewRes = CreateResName(_Name);
	NewRes->SetInputAssembler1VertexBuffer(_Vtx);
	NewRes->SetInputAssembler2IndexBuffer(_Idx);
	return NewRes;
}

void GameEngineMesh::InputAssembler1VertexBufferSetting()
{
	VertexBuffer->Setting();
}


void GameEngineMesh::InputAssembler2IndexBufferSetting()
{
	IndexBuffer->Setting();
}

void GameEngineMesh::Setting()
{
	InputAssembler1VertexBufferSetting();
	InputAssembler2IndexBufferSetting();
}

void GameEngineMesh::SetInputAssembler1VertexBuffer(const std::string& _Name)
{
	VertexBuffer = GameEngineVertexBuffer::Find(_Name);

	if (nullptr == VertexBuffer)
	{
		MsgBoxAssert("�������� �ʴ� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}


	//if (nullptr == InputLayOut && nullptr != VertexShader)
	//{
	//	InputLayOut = GameEngineInputLayOut::Create(*VertexBuffer->GetLayOutDesc(), VertexShader);
	//}
}


void GameEngineMesh::SetInputAssembler2IndexBuffer(const std::string& _Name)
{
	IndexBuffer = GameEngineIndexBuffer::Find(_Name);

	if (nullptr == IndexBuffer)
	{
		MsgBoxAssert("�������� �ʴ� �ε������۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}


void GameEngineMesh::Render()
{
	GameEngineDevice::GetContext()->DrawIndexed(IndexBuffer->GetIndexCount(), 0, 0);
}