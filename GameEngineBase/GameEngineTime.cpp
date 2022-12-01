#include "PreCompile.h"
#include "GameEngineTime.h"


GameEngineTime* GameEngineTime::Inst_ = new GameEngineTime();


GameEngineTime::GameEngineTime() 
	: GlobalScale(1.0f)
	, FrameLimit(- 1)
	, FrameUpdate(true)
{
	Reset();
}

GameEngineTime::~GameEngineTime() 
{
}

void GameEngineTime::Reset() 
{
	SumFPS = 0;
	FrameCheckTime = 1.0f;
	Prev = std::chrono::steady_clock::now();
	Update();
}

void GameEngineTime::Update() 
{
	///////////////// �⺻ ���� ��ŸŸ�� ���ϰ�
	std::chrono::steady_clock::time_point Current = std::chrono::steady_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(Current - Prev);

	DeltaTimed = time_span.count();

	Prev = Current;
	// ���� �ٸ� �������� �ٲܶ� ���
	DeltaTimef = static_cast<float>(DeltaTimed);

	/// �������� üũ�Ѵ�.
	FrameCheckTime -= DeltaTimef;
	if (0.0f >= FrameCheckTime)
	{
		FrameCheckTime += 1.0f;
		FPS = SumFPS;
		SumFPS = 0;
	}



	FrameUpdate = false;
	if (FrameLimit == -1)
	{
		++SumFPS;
		SumDeltaTimef = DeltaTimef;
		FrameUpdate = true;
		UpdateCount = 1;
		return;
	}

	CalDeltaTimef += DeltaTimef;

	if (CalDeltaTimef >= FrameTime)
	{
		SumDeltaTimef = CalDeltaTimef;
		while (CalDeltaTimef >= FrameTime)
		{
			++SumFPS;
			CalDeltaTimef -= FrameTime;
			++UpdateCount;
		}
		FrameUpdate = true;
	}

	// �������� ���ѵǾ� �ִ� ��Ȳ�̴�.

	// 0.5f



	//if (FrameLimit == -1)
	//{
	//	FrameUpdate = true;

	//	if (DeltaTimef >= 0.00001f)
	//	{
	//		++SumFPSCount;
	//		SumFPS += static_cast<int>(1.0f / DeltaTimef);

	//		if (0 >= FrameCheckTime)
	//		{
	//			FPS = SumFPS / SumFPSCount;
	//			FrameCheckTime = 1.0f;
	//			SumFPSCount = 0;
	//			SumFPS = 0;
	//		}
	//	}
	//}
	//else if(FrameCheckTime <= 0.0f)
	//{
	//	FrameUpdate = true;
	//	++SumFPSCount;

	//	if (0 >= FrameCheckTime)
	//	{
	//		FPS = SumFPSCount;
	//		FrameCheckTime = 1.0f;
	//		SumFPS = 0;
	//		SumFPSCount = 0;
	//	}

	//	SumDeltaTimef -= FrameTime;
	//}


}