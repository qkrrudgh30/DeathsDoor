#include "PreCompile.h"
#include "TestStaticActor.h"
#include "BillboardRenderer.h"
#include "WaveRenderer.h"
#include "FadeRenderer.h"

#include <GameEngineCore/GameEngineTextureRenderer.h>

TestStaticActor::TestStaticActor() 
{
}

TestStaticActor::~TestStaticActor() 
{
}

void TestStaticActor::Start()
{
	/*std::shared_ptr<BillboardRenderer> sptrTestPicture = CreateComponent<BillboardRenderer>();
	sptrTestPicture->SetTexture("TestPicture.png");
	sptrTestPicture->GetTransform().SetLocalScale({ 500, 500, 1 });
	sptrTestPicture->GetTransform().SetLocalRotation({ 45.f,0.f,0.f });
	sptrTestPicture->GetTransform().SetLocalPosition({ -1500.f,1500.f, -250.f });
	sptrTestPicture->ChangeCamera(CAMERAORDER::MAINCAMERA);
	sptrTestPicture->SetBillboardInfo(1u);*/

	std::shared_ptr<WaveRenderer> sptrTestPicture = CreateComponent<WaveRenderer>();
	sptrTestPicture->SetTexture("TestPicture.png");
	sptrTestPicture->GetTransform().SetLocalScale({ 500, 500, 1 });
	sptrTestPicture->GetTransform().SetLocalRotation({ 45.f,0.f,0.f });
	sptrTestPicture->GetTransform().SetLocalPosition({ -1500.f,1500.f, -250.f });
	sptrTestPicture->ChangeCamera(CAMERAORDER::MAINCAMERA);

	/*std::shared_ptr<FadeRenderer> sptrTestPicture = CreateComponent<FadeRenderer>();
	sptrTestPicture->SetTexture("TestPicture.png");
	sptrTestPicture->GetTransform().SetLocalScale({ 500, 500, 1 });
	sptrTestPicture->GetTransform().SetLocalRotation({ 45.f,0.f,0.f });
	sptrTestPicture->GetTransform().SetLocalPosition({ -1500.f,1500.f, -250.f });
	sptrTestPicture->ChangeCamera(CAMERAORDER::MAINCAMERA);
	sptrTestPicture->SetFadeInfo(-1, 1.f, True);*/

	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetWorldScale(float4{200.f, 100.f, 100.f});
	Collision->SetParent(sptrTestPicture);
	Collision->GetTransform().SetLocalPosition(float4{0.f, 0.f, 0.f});

}

void TestStaticActor::Update(float _DeltaTime)
{
}

