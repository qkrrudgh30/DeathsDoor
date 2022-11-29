#include "PreCompile.h"
#include "SnapCircle.h"

SnapCircle::SnapCircle() 
{
}

SnapCircle::~SnapCircle() 
{
}

void SnapCircle::Start()
{
	float4 color = { 1.f,0.f,0.f,1.f };

	TexRenderer = CreateComponent<GameEngineTextureRenderer>();
	TexRenderer->SetTexture("Circle.png");
	TexRenderer->GetTransform().SetLocalScale({ 100.f, 100.f, 10.f });
	TexRenderer->GetTransform().SetLocalRotation({ -90.f,0.f,0.f });
	TexRenderer->GetPixelData().MulColor = color;
}

void SnapCircle::Update(float _DeltaTime)
{
	TexRenderer->GetTransform().SetLocalScale(TexRenderer->GetTransform().GetLocalScale() * _DeltaTime * 100.f);

	if (3.f < GetAccTime())
	{
		Death();
	}
}

