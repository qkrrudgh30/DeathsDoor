#include "PreCompile.h"
#include "Tower.h"
#include "Player.h"
#include <GameEngineCore/GameEngineFBXStaticRenderer.h>
#include "GameEngineCore/GameEngineFBXAnimationRenderer.h"

Tower::Tower() :
	StartPostion()
	, DirPower()
	, Pos()
{
}

Tower::~Tower()
{
}

void Tower::Start()
{


	m_Info.m_Hp = 10;
	m_Info.m_MaxHp = 10;
	m_Info.Dammage = 1;
	m_fSpeed = 150.f;
	



	FBXAnimationRenderer = CreateComponent<GameEngineFBXAnimationRenderer>();
	FBXAnimationRenderer->SetFBXMesh("OldCrow.fbx", "TextureAnimation");


	Event.ResourcesName = "OldCrow_Idle.FBX";
	Event.Loop = true;
	Event.Inter = 0.1f;
	FBXAnimationRenderer->CreateFBXAnimation("OldCrow_Idle", Event);


	FBXAnimationRenderer->GetTransform().SetLocalScale(float4{ 100.f, 100.f, 100.f });
	FBXAnimationRenderer->GetTransform().SetLocalRotation(float4{ 0.f, 0.f,0.f });

	FBXAnimationRenderer->ChangeAnimation("OldCrow_Idle");






	{
		Collision = CreateComponent<GameEngineCollision>();
		Collision->GetTransform().SetLocalScale({ 100.0f, 500.0f, 100.0f });
		Collision->ChangeOrder(OBJECTORDER::Monster);
	
	}



	StateManager.CreateStateMember("Idle"
		, std::bind(&Tower::IdleUpdate,this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Tower::IdleStart,this, std::placeholders::_1)
	);

	StateManager.CreateStateMember("Move"
		, std::bind(&Tower::MoveUpdate,this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Tower::MoveStart,this, std::placeholders::_1)
		, std::bind(&Tower::MoveEnd,this, std::placeholders::_1)
	);



	StateManager.CreateStateMember("Stun"
		, std::bind(&Tower::StunUpdate,this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Tower::StunStart,this, std::placeholders::_1)
		, std::bind(&Tower::StunEnd,this, std::placeholders::_1)
	);




	StateManager.CreateStateMember("Att"
		, std::bind(&Tower::AttUpdate,this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Tower::AttStart,this, std::placeholders::_1)
		, std::bind(&Tower::AttEnd,this, std::placeholders::_1)
	);


	StateManager.ChangeState("Idle");
}

void Tower::Update(float _DeltaTime)
{

	






	if (m_Info.m_Hp <= 0)
	{
		Death();
	}

	StateManager.Update(_DeltaTime);

}






void Tower::MoveStart(const StateInfo& _Info)
{

}
void Tower::MoveEnd(const StateInfo& _Info)
{

}
void Tower::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{


	if (m_bHitCheck)
	{
		StateManager.ChangeState("Stun");
		m_bHitCheck = false;
	}







	float4 TarGetDir = Player::GetMainPlayer()->GetTransform().GetWorldPosition() - GetTransform().GetWorldPosition();

	float4 TarGetDirAngle = Player::GetMainPlayer()->GetTransform().GetWorldPosition();

	float Len = TarGetDir.Length();
	TarGetDir = TarGetDir.Normalize3DReturn();


	//���� ����

	float4 MyPos = GetTransform().GetWorldPosition();

	TarGetDirAngle.y = -TarGetDirAngle.z;
	MyPos.y = -MyPos.z;


	TarGetDirAngle.z = 0.f;
	TarGetDirAngle.w = 0.f;

	MyPos.z = 0.f;
	MyPos.w = 0.f;

	float Angle = float4::VectorXYtoDegree(MyPos, TarGetDirAngle);

	Angle += 90.f;

	if (Angle >= 360.f)
	{
		Angle -= 360.f;
	}
	else if (Angle <= 0.f)
	{
		Angle -= 0.f;
	}
	GetTransform().SetLocalRotation({ 0.0f, Angle, 0.0f });





	GetTransform().SetWorldMove(TarGetDir * m_fSpeed * _DeltaTime);


	if (Len <= 150.f)
	{
		StateManager.ChangeState("Att");
	}
	else if (Len >= 600.f)
	{
		StateManager.ChangeState("Idle");
	}

	




}



void Tower::StunStart(const StateInfo& _Info)
{
	m_fHitDir = GetTransform().GetWorldPosition() - m_fHitPos;
	m_fHitDir = m_fHitDir.Normalize3DReturn();
}
void Tower::StunEnd(const StateInfo& _Info)
{

	

}
void Tower::StunUpdate(float _DeltaTime, const StateInfo& _Info)
{

	hitTime += _DeltaTime;

	if (hitTime <= 0.2f)
	{
	//	hitTime = 0.f;
		GetTransform().SetWorldMove(m_fHitDir * 500.f * _DeltaTime);
	//	StateManager.ChangeState("Idle");
	}
	else if (hitTime >= 0.5f)
	{
		hitTime = 0.f;
		StateManager.ChangeState("Idle");
	}
	

	



}
	 





void Tower::AttStart(const StateInfo& _Info)
{
	m_bhitCheck = true;
}
void Tower::AttEnd(const StateInfo& _Info)
{
	m_bhitCheck = false;
}
void Tower::AttUpdate(float _DeltaTime, const StateInfo& _Info)
{
	m_fHitTime += _DeltaTime;

	if (m_fHitTime >= 1.f)
	{
		m_fHitTime = 0.f;
		StateManager.ChangeState("Idle");

	}


	//���� ����
	if (m_bHitCheck)
	{
		StateManager.ChangeState("Stun");
		m_bHitCheck = false;
	}

}




void Tower::IdleStart(const StateInfo& _Info)
{

}
void Tower::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (m_bHitCheck)
	{
		StateManager.ChangeState("Stun");
		m_bHitCheck = false;
	}




	float4 TarGetDir = Player::GetMainPlayer()->GetTransform().GetWorldPosition() - GetTransform().GetWorldPosition();

	float Len = TarGetDir.Length();
	TarGetDir = TarGetDir.Normalize3DReturn();


	if (Len <= 600.f)
	{
		StateManager.ChangeState("Move");
	}



}