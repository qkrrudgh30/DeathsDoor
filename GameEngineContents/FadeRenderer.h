#pragma once
#include <GameEngineCore/GameEngineDefaultRenderer.h>

struct FadeInfo
{
	float4  mf4FrameData;
	float4	mf4PivotPos;
	float	mfFrom;
	float	mfTo;
	int		miWrapping;
	int		miLoop;
	int		miIsUnityTexture;
};

// ���� :
class FadeRenderer : public GameEngineDefaultRenderer
{
public:
	// constrcuter destructer
	FadeRenderer();
	~FadeRenderer();

	// delete Function
	FadeRenderer(const FadeRenderer& _Other) = delete;
	FadeRenderer(FadeRenderer&& _Other) noexcept = delete;
	FadeRenderer& operator=(const FadeRenderer& _Other) = delete;
	FadeRenderer& operator=(FadeRenderer&& _Other) noexcept = delete;

	void SetPivot(PIVOTMODE _mode);

protected:
	void Start() override;

private:
	void InitializeFadeInfo(void);
	void SetFadeRendererSetting(void);

	FadeInfo mFadeInfo;

};
