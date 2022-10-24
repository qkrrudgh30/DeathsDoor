#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// ���� :

enum
{
	ACTORPANNEL,
	TILEPANNEL,
	END,
};

struct EditItem
{
	
};

class EditLevel;
class GameEngineActor;
class EditGUIWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	EditGUIWindow();
	~EditGUIWindow();

	// delete Function
	EditGUIWindow(const EditGUIWindow& _Other) = delete;
	EditGUIWindow(EditGUIWindow&& _Other) noexcept = delete;
	EditGUIWindow& operator=(const EditGUIWindow& _Other) = delete;
	EditGUIWindow& operator=(EditGUIWindow&& _Other) noexcept = delete;

	void Save();
	void SaveTileData(const std::string& _strTitle);
	// void SaveTileData(const std::wstring& _strFilePath, const std::string& _strTitle);


	void Load();
	void LoadTileData(const std::string& _strFilePath);

protected:

private:
	void Initialize(class GameEngineLevel* _Level) override;
	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;

	void CreateMultiple(int _XCount, int Y);

	GameEngineDirectory m_CurrentDirectory;
	GameEngineDirectory m_ProjectDirectory;
	std::vector<std::string> m_vLoadedFromActor;
	std::vector<std::string> m_vLoadedFromTile;

	EditLevel* m_ptrEditLevel;
	std::vector<std::pair<std::string, GameEngineActor*>> m_vCreatedActors;
};

