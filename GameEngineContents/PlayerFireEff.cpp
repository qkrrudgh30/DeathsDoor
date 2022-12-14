
#include "PreCompile.h"
#include "PlayerFireEff.h"
#include "BillboardRenderer.h"
#include"GameEngineBase/GameEngineRandom.h"

PlayerFireEff::PlayerFireEff()
{
}

PlayerFireEff::~PlayerFireEff()
{
}

void PlayerFireEff::Start()
{

	m_fSpeed = 800.f;



	float G = GameEngineRandom::MainRandom.RandomFloat(94.f, 228.f) / 255.f;
	float A = GameEngineRandom::MainRandom.RandomFloat(700.f, 1000.f) / 1000.f;


	float4 color = { 255.f ,G,0.f,A };
	TexRenderer = CreateComponent<GameEngineTextureRenderer>();
	TexRenderer->SetTexture("fireparticle.png");
	TexRenderer->GetTransform().SetLocalScale({ 10.f, 10.f, 1 });

	TexRenderer->GetTransform().SetLocalRotation({ 0.f,0.f,0.f });
	TexRenderer->ChangeCamera(CAMERAORDER::MAINCAMERA);
	TexRenderer->GetPixelData().MulColor = color;
	TexRenderer->RenderOptionInst.Temp1 = 15;
	TexRenderer->ChangeCamera(CAMERAORDER::USER1);



	//Death(1.f);
}

void PlayerFireEff::Update(float _DeltaTime)
{


	m_fSpeed = GameEngineRandom::MainRandom.RandomFloat(200.f, 500.f);

	float fScaleSpeed = GameEngineRandom::MainRandom.RandomFloat(300.f, 500.f);


	float4 MyScale = TexRenderer->GetTransform().GetLocalScale();

	if (m_bScalecheck)
	{
		MyScale.x -= fScaleSpeed * _DeltaTime;
		MyScale.y -= fScaleSpeed * _DeltaTime;
		//	MyScale.z -= 100.f * _DeltaTime;

		if (0.f >= MyScale.y)
		{

			Death();
			m_bScalecheck = !m_bScalecheck;
		}

	}
	else
	{
		MyScale.x += fScaleSpeed * _DeltaTime;
		MyScale.y += fScaleSpeed * _DeltaTime;
		//	MyScale.z += 100.f * _DeltaTime;

		if (m_fScaleMax <= MyScale.y)
		{
			m_bScalecheck = !m_bScalecheck;
		}
	}



	float4 MyRot = TexRenderer->GetTransform().GetLocalRotation();
	MyRot.z += fScaleSpeed * _DeltaTime;
	TexRenderer->GetTransform().SetLocalRotation(MyRot);


	TexRenderer->GetTransform().SetWorldScale(MyScale);

	float4 UpVec = { 0.f,1.f,0.f };


	TexRenderer->GetTransform().SetWorldMove(UpVec * _DeltaTime * m_fSpeed);





}


void PlayerFireEff::ChangeColorBlue()
{
	float G = GameEngineRandom::MainRandom.RandomFloat(0.f, 216.f) / 255.f;
	float A = GameEngineRandom::MainRandom.RandomFloat(700.f, 1000.f) / 1000.f;


	float4 color = { 0.01f ,G,255.f,A };


	TexRenderer->GetPixelData().MulColor = color;

}


//-436  218  43