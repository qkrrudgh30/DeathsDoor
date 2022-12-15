#include "PreCompile.h"
#include "ContentsBlur.h"

ContentsBlur::ContentsBlur()
	: mBlurInfo{}
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

	msptrAppliedRenderUnit = std::make_shared<GameEngineRenderUnit>();

	msptrAppliedRenderUnit->SetMesh("FullRect");
	msptrAppliedRenderUnit->SetMaterial("ContentsBlur");
	msptrAppliedRenderUnit->ShaderResources.SetConstantBufferLink("BlurInfo", mBlurInfo);
}

void ContentsBlur::Effect(std::shared_ptr<GameEngineRenderTarget> _sptrRenderTarget)
{
	msptrRenderTargetForBlur->Copy(_sptrRenderTarget);
	// msptrRenderTargetForBlur�� ��� �����, _sptrRenderTarget�� �׸��� �׷���.

	msptrAppliedRenderUnit->ShaderResources.SetTexture("Tex", msptrRenderTargetForBlur->GetRenderTargetTexture(0u));
	// msptrRenderTargetForBlur�� �׷��� �׸��� mAppliedRenderUnit�� ���̴� ���ҽ��� ������. (���̴��� ����ǰ� �ϱ� ����.)

	_sptrRenderTarget->Clear();
	_sptrRenderTarget->Setting();
	_sptrRenderTarget->Effect(msptrAppliedRenderUnit);
	// _sptrRenderTarget���ٰ� �ϼ��� mAppliedRenderUnit�� ȿ���� ����ǰԲ� ��.

}

void ContentsBlur::SetBlurInfo(unsigned int _uOnOff, unsigned int _uAppliedArea, float _fIntence)
{
	mBlurInfo.muOnOff = _uOnOff;
	mBlurInfo.muAppliedArea = _uAppliedArea;
	mBlurInfo.mfIntence = _fIntence;
}

void ContentsBlur::InitializeBlurInfo(void)
{
	mBlurInfo.muOnOff = 0u;
	mBlurInfo.muAppliedArea = 3u;
	mBlurInfo.mfIntence = 0.f;

	mBlurInfo.mf4WindowSize.x = GameEngineWindow::GetScale().x;
	mBlurInfo.mf4WindowSize.y = GameEngineWindow::GetScale().y;
}

