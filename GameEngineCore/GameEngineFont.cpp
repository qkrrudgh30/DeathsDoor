#include "PreCompile.h"
#include "GameEngineFont.h"

IFW1Factory* GameEngineFont::FontFactiory = nullptr;


class GameEngineFontInit
{
public:
	GameEngineFontInit()
	{
		FW1CreateFactory(FW1_VERSION, &GameEngineFont::FontFactiory);
	}

	~GameEngineFontInit()
	{
		if (nullptr != GameEngineFont::FontFactiory)
		{
			GameEngineFont::FontFactiory->Release();
		}
	}
};

GameEngineFontInit Inst;

GameEngineFont::GameEngineFont() 
	: FontWrapper(nullptr)
{
}

GameEngineFont::~GameEngineFont() 
{
	if (nullptr != FontWrapper)
	{
		FontWrapper->Release();
		FontWrapper = nullptr;
	}
	
}


std::shared_ptr<GameEngineFont> GameEngineFont::Load(const std::string& _Path, const std::string& _Name)
{
	std::shared_ptr < GameEngineFont> NewRes = CreateResName(_Name);
	NewRes->LoadFont(_Path);
	return NewRes;
}

void GameEngineFont::LoadFont(const std::string& _Path)
{
	std::wstring WPath = GameEngineString::AnsiToUniCodeReturn(_Path);

	if (S_OK != FontFactiory->CreateFontWrapper(GameEngineDevice::GetDevice(), WPath.c_str(), &FontWrapper))
	{
		MsgBoxAssert("��Ʈ ������ �����߽��ϴ�");
		return;
	}
}

void GameEngineFont::FontDraw(const std::string& _Text, float _FontSize, float4 _ScreenPos, float4 Color, UINT Flag)
{
	std::wstring Text = GameEngineString::AnsiToUniCodeReturn(_Text);

	//for (size_t i = 0; i < Text.size(); i++)
	//{
	//	std::wstring Character;
	//	Character.resize(2);
	//	Character[0] = Text[i];
	//	FontWrapper->DrawString(GameEngineDevice::GetContext(), Character.c_str(), _FontSize, _ScreenPos.x, _ScreenPos.y, Color.GetUIntColor(), Flag);
	//}

	FontWrapper->DrawString(GameEngineDevice::GetContext(), Text.c_str(), _FontSize, _ScreenPos.x, _ScreenPos.y, Color.GetUIntColor(), Flag);
}