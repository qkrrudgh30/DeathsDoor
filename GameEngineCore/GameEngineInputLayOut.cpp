#include "PreCompile.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineVertexs.h"
#include "GameEngineVertexShader.h"

std::shared_ptr < GameEngineInputLayOut> GameEngineInputLayOut::Create(const GameEngineLayOutDesc& _Desc, std::shared_ptr<GameEngineVertexShader> _Shader)
{
	std::shared_ptr < GameEngineInputLayOut> Input = CreateResUnName();
	Input->CreateRes(_Desc, _Shader);
	return Input;
}

GameEngineInputLayOut::GameEngineInputLayOut() 
	: InputLayOut(nullptr)
{
}

GameEngineInputLayOut::~GameEngineInputLayOut() 
{
	if (nullptr != InputLayOut)
	{
		InputLayOut->Release();
	}
}


void GameEngineInputLayOut::Setting()
{
	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("��ǲ���̾ƿ��� ������ �ֽ��ϴ�.");
	}

	GameEngineDevice::GetContext()->IASetInputLayout(InputLayOut);

	// GameEngineDevice::GetContext()->VSSetConstantBuffers(0, 1, nullptr);

	// GameEngineDevice::GetContext()->VSSetConstantBuffers(5, 1, nullptr);
}

void GameEngineInputLayOut::CreateRes(const GameEngineLayOutDesc& _Desc, std::shared_ptr<GameEngineVertexShader> _Shader)
{
	if (
		S_OK != GameEngineDevice::GetDevice()->CreateInputLayout(
		&_Desc.InputLayOutDesc[0],
		static_cast<UINT>(_Desc.InputLayOutDesc.size()),
		_Shader->BinaryPtr->GetBufferPointer(),
		_Shader->BinaryPtr->GetBufferSize(),
			&InputLayOut)
		)
	{
		MsgBoxAssert("��ǲ���̾ƿ� ������ �����߽��ϴ�");
	}


}
