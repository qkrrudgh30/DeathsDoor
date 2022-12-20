

#pragma once
#include <GameEngineCore/CoreMinimal.h>

#include "UnitBase.h"

// ���� :
class RealLaser;
class RealLaserTarget;
class RealLaserMgr : public UnitBase
{
public:
	// constrcuter destructer
	RealLaserMgr();
	~RealLaserMgr();

	// delete Function
	RealLaserMgr(const RealLaserMgr& _Other) = delete;
	RealLaserMgr(RealLaserMgr&& _Other) noexcept = delete;
	RealLaserMgr& operator=(const RealLaserMgr& _Other) = delete;
	RealLaserMgr& operator=(RealLaserMgr&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:

	int m_iCount = 0;
	float m_fMakeTime = 0.f;
	//���ͷ� ���� ������4 Ÿ��4
	//�����ɶ� ���� ����
	//RealLaser
	//RealLaserTarget
	std::vector<std::shared_ptr<RealLaserTarget>> m_vTarget = {};
	std::vector<std::shared_ptr<RealLaser>> m_vRealLaser = {};
public:
	void DeleteAll();
	void MakeTarget();
	void MakeLaser();
	void StartLaser();


	bool m_bLaserEnd = false;
	bool m_bTarGetStart = false;
	bool m_bLaserStart = false;


};

