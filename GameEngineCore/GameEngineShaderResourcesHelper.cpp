#include "PreCompile.h"
#include "GameEngineShaderResourcesHelper.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineStructuredBuffer.h"

GameEngineShaderResourcesHelper::GameEngineShaderResourcesHelper()
{
}

GameEngineShaderResourcesHelper::~GameEngineShaderResourcesHelper()
{
}

void GameEngineShaderResourcesHelper::AllResourcesSetting()
{
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& Setter : ConstantBufferSettingMap)
	{
		Setter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& Setter : TextureSettingMap)
	{
		Setter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineSamplerSetter>& Setter : SamplerSettingMap)
	{
		Setter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineStructuredBufferSetter>& Setter : StructuredBufferSettingMap)
	{
		Setter.second.Setting();
	}

}


void GameEngineShaderResourcesHelper::AllResourcesReset()
{
	for (const std::pair<std::string, GameEngineTextureSetter>& Setter : TextureSettingMap)
	{
		Setter.second.Reset();
	}
}

void GameEngineShaderResourcesHelper::ResourcesCheck(std::shared_ptr<GameEngineMaterial> _Line)
{
	if (nullptr == _Line)
	{
		MsgBoxAssert("������������ nullptr �Դϴ� �������� ������������ �־��ּ���");
	}

	ShaderCheck(_Line->GetVertexShader());
	ShaderCheck(_Line->GetPixelShader());

}

GameEngineConstantBufferSetter* GameEngineShaderResourcesHelper::GetConstantsBuffer(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator Find = ConstantBufferSettingMap.find(Key);

	if (ConstantBufferSettingMap.end() != Find)
	{
		return &Find->second;
	}

	return &Find->second;
}

void GameEngineShaderResourcesHelper::ShaderCheck(std::shared_ptr < GameEngineShader> _Shader)
{
	// �ȼ����̴��� ���ؽ� ���̴����� transform������ ���� �߿� ��������� �̸��� �Ȱ��� �ؼ� ����ϰ� �ʹٸ�??????
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& Data : _Shader->ConstantBufferSettingMap)
	{
		std::multimap<std::string, GameEngineConstantBufferSetter>::iterator InsertIter =
			ConstantBufferSettingMap.insert(std::make_pair(Data.first, Data.second));

		InsertIter->second.Bind();
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& Data : _Shader->TextureSettingMap)
	{
		std::multimap<std::string, GameEngineTextureSetter>::iterator InsertIter =
			TextureSettingMap.insert(std::make_pair(Data.first, Data.second));

		InsertIter->second.Bind();

	}


	for (const std::pair<std::string, GameEngineSamplerSetter>& Data : _Shader->SamplerSettingMap)
	{
		std::multimap<std::string, GameEngineSamplerSetter>::iterator InsertIter =
			SamplerSettingMap.insert(std::make_pair(Data.first, Data.second));

		InsertIter->second.Bind();
	}


	for (const std::pair<std::string, GameEngineStructuredBufferSetter>& Data : _Shader->StructuredBufferSettingMap)
	{
		std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator InsertIter =
			StructuredBufferSettingMap.insert(std::make_pair(Data.first, Data.second));

		InsertIter->second.Bind();
	}

}


GameEngineStructuredBufferSetter* GameEngineShaderResourcesHelper::GetStructuredBuffer(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator Find = StructuredBufferSettingMap.find(Key);

	if (StructuredBufferSettingMap.end() != Find)
	{
		return &Find->second;
	}

	return &Find->second;
}

void GameEngineShaderResourcesHelper::SetConstantBufferNew(const std::string& _Name, const void* _Data, UINT _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssertString(_Name + "���̴����� �̷��� �̸��� ������۸� ����� ���� �����ϴ�.");
		return;
	}

	if (16 > _Size)
	{

		MsgBox("�ּ��� 16����Ʈ �̻��� ������ ����� �մϴ�.");
		return;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStartIter
		= ConstantBufferSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEndIter
		= ConstantBufferSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		// Ʈ�������� �ٲ��

		if (0 == NameStartIter->second.OriginalData.size()
			|| NameStartIter->second.OriginalData.size() != _Size)
		{
			NameStartIter->second.OriginalData.resize(_Size);
		}

		NameStartIter->second.SetData = &NameStartIter->second.OriginalData[0];
		memcpy_s(&NameStartIter->second.OriginalData[0], _Size, _Data, _Size);

		NameStartIter->second.Size = _Size;
	}
}

void GameEngineShaderResourcesHelper::SetConstantBufferLink(
	const std::string& _Name,
	const void* _Data,
	UINT _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssertString(_Name + "���̴����� �̷��� �̸��� ������۸� ����� ���� �����ϴ�.");
		return;
	}

	if (16 > _Size)
	{

		MsgBox("�ּ��� 16����Ʈ �̻��� ������ ����� �մϴ�.");
		return;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStartIter
		= ConstantBufferSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEndIter
		= ConstantBufferSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		// Ʈ�������� �ٲ��
		NameStartIter->second.SetData = _Data;
		NameStartIter->second.Size = _Size;
	}

}



std::shared_ptr<GameEngineTexture> GameEngineShaderResourcesHelper::SetTexture(const std::string& _Name, const std::string& _TextureName)
{
	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	return SetTexture(_Name, GameEngineTexture::Find(_TextureName));
}

std::shared_ptr < GameEngineTexture> GameEngineShaderResourcesHelper::SetTexture(const std::string& _Name, const std::string& _FolderTextureName, int _Index)
{
	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	std::string TextureName = GameEngineString::ToUpperReturn(_FolderTextureName);

	std::shared_ptr < GameEngineFolderTexture> Tex = GameEngineFolderTexture::Find(TextureName);

	if (nullptr == Tex)
	{
		MsgBoxAssert("�������� �ʴ� ���� �ؽ�ó�� �����Ϸ��� �߽��ϴ�.");
	}

	return SetTexture(_Name, Tex->GetTexture(_Index));
}

std::shared_ptr < GameEngineTexture> GameEngineShaderResourcesHelper::SetTexture(const std::string& _Name, std::shared_ptr < GameEngineTexture> _Texture)
{
	std::string Name = GameEngineString::ToUpperReturn(_Name);

	if (false == IsTexture(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStartIter
		= TextureSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEndIter
		= TextureSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		NameStartIter->second.Res = _Texture;
		NameStartIter->second.Bind();
	}

	return _Texture;
}


std::shared_ptr < GameEngineSampler> GameEngineShaderResourcesHelper::SetSampler(const std::string& _Name, const std::string& _TextureName)
{
	if (false == IsSampler(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::string Name = GameEngineString::ToUpperReturn(_Name);

	return SetSampler(_Name, GameEngineSampler::Find(_TextureName));

}

std::shared_ptr < GameEngineSampler> GameEngineShaderResourcesHelper::SetSampler(const std::string& _Name, std::shared_ptr < GameEngineSampler> _Res)
{
	std::string Name = GameEngineString::ToUpperReturn(_Name);

	if (false == IsSampler(_Name))
	{
		MsgBox("���̴����� �̷��� �̸��� �ؽ�ó���ø� ����� ���� �����ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameStartIter
		= SamplerSettingMap.lower_bound(Name);

	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameEndIter
		= SamplerSettingMap.upper_bound(Name);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		NameStartIter->second.Res = _Res;
		NameStartIter->second.Bind();
		// BindSampler(NameStartIter->second, _Res);
	}

	return _Res;
}





void GameEngineShaderResourcesHelper::AllConstantBufferNew()
{
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator Start = ConstantBufferSettingMap.begin();
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator End = ConstantBufferSettingMap.end();

	for (; Start != End; ++Start)
	{
		int Buffersize = Start->second.Res->GetBufferDesc().ByteWidth;
		Start->second.OriginalData.resize(Buffersize);
		Start->second.SetData = &Start->second.OriginalData[0];
		Start->second.Size = Buffersize;
	}
}

bool GameEngineShaderResourcesHelper::IsTexture(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (TextureSettingMap.end() != TextureSettingMap.find(Key))
	{
		return true;
	}

	return false;
}



bool GameEngineShaderResourcesHelper::IsSampler(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (SamplerSettingMap.end() != SamplerSettingMap.find(Key))
	{
		return true;
	}

	return false;
}



bool GameEngineShaderResourcesHelper::IsConstantBuffer(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (ConstantBufferSettingMap.end() != ConstantBufferSettingMap.find(Key))
	{
		return true;
	}

	return false;
}

bool GameEngineShaderResourcesHelper::IsStructuredBuffer(const std::string& _Name)
{
	std::string Key = GameEngineString::ToUpperReturn(_Name);

	if (StructuredBufferSettingMap.end() != StructuredBufferSettingMap.find(Key))
	{
		return true;
	}

	return false;
}