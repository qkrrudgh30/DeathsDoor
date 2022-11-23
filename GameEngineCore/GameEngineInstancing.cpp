#include "PreCompile.h"
#include "GameEngineInstancing.h"
#include "GameEngineRenderer.h"
#include "GameEngineVertexShader.h"

void GameEngineInstancing::InstancingData::Link(const std::string_view& _Name, const void* _Data)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name.data());

	if (Data.end() == Data.find(UpperName))
	{
		MsgBoxAssertString("�ν��Ͻ̿� �����Ͱ� �������� �ʽ��ϴ� => " + UpperName);
	}

	std::multimap<std::string, const void*>::iterator Start = Data.lower_bound(UpperName);
	std::multimap<std::string, const void*>::iterator End = Data.upper_bound(UpperName);
	for (; Start != End; ++Start)
	{
		Start->second = _Data;
	}
}

void GameEngineInstancing::InstancingData::Init(std::multiset<std::string>& _Set)
{
	//std::multimap<std::string, GameEngineStructuredBufferSetter>& StructuredBuffers
	//	= _Helper.GetStructuredBufferSettingMap();

	//std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator Start = StructuredBuffers.begin();
	//std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator End = StructuredBuffers.end();

	for (const std::string& _Name : _Set)
	{
		Data.insert(std::make_pair(_Name, nullptr));
	}

	//for (; Start != End; ++Start)
	//{
	//	if (std::string::npos != Start->first.find("INST_"))
	//	{
	//		Data.insert(std::make_pair(Start->first, nullptr));
	//	}
	//}

}

size_t GameEngineInstancing::MaxInstancingCount = 100;

GameEngineInstancing::GameEngineInstancing() 
	: Buffer(nullptr)
	, Count(0)

{
	// CreateInstancingUnit();
}

std::list<GameEngineInstancing::InstancingData>& GameEngineInstancing::CreateInstancingUnit()
{
	// ���ο� 100�� ¥�� �׷��� ���������.
	std::list<InstancingData>& NewGroup = Units.emplace_back();
	// �ű⿡ �� �ν��Ͻ̿� ���۵� �������
	std::vector<char>& Data = DataBuffer.emplace_back();
	// ����
	GameEngineShaderResourcesHelper& Helper = ShaderResources.emplace_back();

	Helper.ShaderCheck(InitUnit->GetMaterial()->GetVertexShader()->GetInstancingShader());


	unsigned int size = static_cast<unsigned int>(InitUnit->GetMesh()->GetLayOutDesc().InstancingSize);
	Buffer = GameEngineInstancingBuffer::Create(MaxInstancingCount, size);
	Data.resize(MaxInstancingCount * size);

	// ��Ʈ��ó�� ���� ����
	std::multimap<std::string, GameEngineStructuredBufferSetter>& StructuredBuffers
		= Helper.GetStructuredBufferSettingMap();

	std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator Start = StructuredBuffers.begin();
	std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator End = StructuredBuffers.end();

	for (; Start != End; ++Start)
	{
		if (std::string::npos != Start->first.find("INST_"))
		{
			Start->second.Resize(MaxInstancingCount);
		}
	}

	if (StructuredBufferSet.empty())
	{
		std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator Start = StructuredBuffers.begin();
		std::multimap<std::string, GameEngineStructuredBufferSetter>::iterator End = StructuredBuffers.end();

		for (; Start != End; ++Start)
		{
			if (std::string::npos != Start->first.find("INST_"))
			{
				StructuredBufferSet.insert(Start->first);
			}
		}
	}

	return NewGroup;
}

GameEngineInstancing::~GameEngineInstancing() 
{
}

void GameEngineInstancing::RenderInstancing() 
{

}

void GameEngineInstancing::InstancingBufferChangeData()
{
	if (nullptr == Buffer)
	{
		MsgBoxAssert("�ν��Ͻ̿� ���۸� ������ �ʾҽ��ϴ�. �ʾҽ��ϴ�.")
	}

	Buffer->ChangeData(&DataBuffer[0], DataBuffer.size());
}

void GameEngineInstancing::PushUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, std::function<void(InstancingData&)> _Function)
{
	if (nullptr == InitUnit)
	{
		InitUnit = _Unit;

		if (false == InitUnit->GetMaterial()->GetVertexShader()->IsInstancing())
		{
			MsgBoxAssert("�ν��Ͻ̿� ���׸����� �ƴմϴ�.");
		}

		CreateInstancingUnit();
	}

	_Unit->Off();

	if (false == _Unit->GetMaterial()->GetVertexShader()->IsInstancing())
	{
		MsgBoxAssert("�ν��Ͻ̿� ���׸����� ������ ���� ����Ʈ �Դϴ�");
	}

	// 100��
	// 1 ��

	bool Push = false;

	for (size_t i = 0; i < Units.size(); i++)
	{
		if (MaxInstancingCount <= Units[i].size())
		{
			continue;
		}

		Push = true;
		InstancingData NewData = { _Unit };

		NewData.Init(StructuredBufferSet);

		_Function(NewData);

		Units[i].push_back(NewData);
	}

	if (false == Push)
	{
		std::list<InstancingData>& Insert = CreateInstancingUnit();

		InstancingData NewData = { _Unit };

		NewData.Init(StructuredBufferSet);

		_Function(NewData);

		Insert.push_back(NewData);
	}
	

}

