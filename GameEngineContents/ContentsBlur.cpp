#include "PreCompile.h"
#include "ContentsBlur.h"

std::weak_ptr<ContentsBlur> ContentsBlur::msptrContentsBlur;
BlurInfo ContentsBlur::mBlurInfo = { 0, };

ContentsBlur::ContentsBlur()
{
}

ContentsBlur::~ContentsBlur()
{
}

void ContentsBlur::EffectInit()
{
	msptrRenderTargetForBlur = std::make_shared<GameEngineRenderTarget>();
	msptrRenderTargetForBlur->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	InitializeBlurInfo();


	mAppliedRenderUnit.SetMesh("FullRect");
	// mAppliedRenderUnit.SetPipeLine("Blur");
	mAppliedRenderUnit.SetPipeLine("ContentsBlur");
	mAppliedRenderUnit.ShaderResources.SetConstantBufferLink("BlurInfo", mBlurInfo);
}

void ContentsBlur::Effect(std::shared_ptr<GameEngineRenderTarget> _sptrRenderTarget)
{
	msptrRenderTargetForBlur->Copy(_sptrRenderTarget);
	// msptrRenderTargetForBlur�� ��� �����, _sptrRenderTarget�� �׸��� �׷���.

	mAppliedRenderUnit.ShaderResources.SetTexture("Tex", msptrRenderTargetForBlur->GetRenderTargetTexture(0));
	// msptrRenderTargetForBlur�� �׷��� �׸��� mAppliedRenderUnit�� ���̴� ���ҽ��� ������. (���̴��� ����ǰ� �ϱ� ����.)

	_sptrRenderTarget->Clear();
	_sptrRenderTarget->Setting();
	_sptrRenderTarget->Effect(mAppliedRenderUnit);
	// _sptrRenderTarget���ٰ� �ϼ��� mAppliedRenderUnit�� ȿ���� ����ǰԲ� ��.

}

void ContentsBlur::SetBlurInfo(unsigned int _uBlurType, unsigned int _uAppliedCount, float _fRadius, int _iIsUnityTexture)
{
	mBlurInfo.muAppliedType = _uBlurType;
	mBlurInfo.muAppliedCount = _uAppliedCount;
	mBlurInfo.mfRadius = _fRadius;
}

void ContentsBlur::InitializeBlurInfo(void)
{
	mBlurInfo.muAppliedType = BlurType::eNoBlur;
	mBlurInfo.muAppliedCount = 0u;

	mBlurInfo.mfRadius = 0.f;

	mBlurInfo.mf4WindowSize.x = GameEngineWindow::GetInst()->GetScale().x;
	mBlurInfo.mf4WindowSize.y = GameEngineWindow::GetInst()->GetScale().y;
}

