#include "PreCompile.h"
#include "StageMainCamera.h"
#include "Player.h"


StageMainCamera::StageMainCamera() :
	m_Player(nullptr)
{
}

StageMainCamera::~StageMainCamera() 
{
}

void StageMainCamera::Start()
{
	


	GetLevel()->GetMainCameraActorTransform().SetLocalPosition({ 0.f,300.f,-300.f });

}

void StageMainCamera::Update(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	GetTransform().SetWorldPosition(m_Player->GetTransform().GetWorldPosition());
	float4 WorldPos = GetTransform().GetWorldPosition();
	float4 CameraWorldPos = WorldPos;
	CameraWorldPos.y += 700.f;
	CameraWorldPos.z -= 700.f;


	GetLevel()->GetMainCameraActorTransform().SetWorldPosition(CameraWorldPos);
	GetLevel()->GetMainCameraActorTransform().SetWorldRotation({45.f,0.f,0.f});

}