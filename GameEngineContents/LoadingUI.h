#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum
{
	eAnimatorMesh,
	eStaticMesh,
	eAnimation,
	eEndStage,
};

// ���� :
class LoadingUI : public GameEngineActor
{
public:
	// constrcuter destructer
	LoadingUI();
	~LoadingUI();

	// delete Function
	LoadingUI(const LoadingUI& _Other) = delete;
	LoadingUI(LoadingUI&& _Other) noexcept = delete;
	LoadingUI& operator=(const LoadingUI& _Other) = delete;
	LoadingUI& operator=(LoadingUI&& _Other) noexcept = delete;

	void SetProgressAmount(size_t _uTotalCountOfResources, size_t _uCurrentCountOfLoadedResources) 
	{
		if (-1 != _uTotalCountOfResources) // �ش� �ε� ���������� �ε��� �Ϸ� ���� �ʾ��� ��
		{ 
			if (false == mbIsFirstLoadingStage && 1 == _uCurrentCountOfLoadedResources) 
			{ // ���� �ܰ��� ù ��° �ε��� �Ϸ�Ǿ��� ��. 0%���� �������Բ� �ϱ� ����.
				++muCurrLoadingStage;
				mfSrcProgressAmount = 0.f;
			}
			muTotalCountOfResources = _uTotalCountOfResources;
			muCurrentCountOfLoadedResources = _uCurrentCountOfLoadedResources;
			mfDstProgressAmount = (static_cast<float>(muCurrentCountOfLoadedResources)) / muTotalCountOfResources;
		}
		else 
		{ // �ش� �ε� ���������� �ε��� �Ϸ� �Ǿ��� ��. 100%������ Progress bar�� �������Բ� �ϱ� ����.
			return;
		}
	}

	static bool mbIsFirstLoadingStage;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineUIRenderer* mpUIRendererForBackground;
	GameEngineUIRenderer* mpUIRendererForProgressBar;
	
	size_t muTotalCountOfResources;
	size_t muCurrentCountOfLoadedResources;

	float mfCrrProgressState;
	float mfDstProgressAmount;
	float mfSrcProgressAmount;
	size_t muCurrLoadingStage;
	size_t muPrevLoadingStage;

};

