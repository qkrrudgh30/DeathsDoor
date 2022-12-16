#include "PreCompile.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineInstancing.h"
#include "GameEngineStructuredBuffer.h"
#include "GameEnginePixelShader.h"
#include <GameEngineBase/GameEngineWindow.h>



GameEngineCamera::GameEngineCamera() 
	: CameraForwardRenderTarget(nullptr)
	, CameraDeferredRenderTarget(nullptr)
{
	// �����찡 �����е� �����ϱ� ���� ���� �����̶� ���⼭ �ϴ°Ű�.
	Size = GameEngineWindow::GetInst()->GetScale();
	Mode = CAMERAPROJECTIONMODE::PersPective;
	Near = 0.1f;
	Far = 10000.0f;
	Fov = 60.0f;

	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = Size.x;
	ViewPortDesc.Height = Size.y;
	ViewPortDesc.MinDepth = 0.0f;
	ViewPortDesc.MaxDepth = 1.0f;

	AllRenderUnit_.insert(std::make_pair(RENDERINGPATHORDER::FORWARD, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>()));
	AllRenderUnit_.insert(std::make_pair(RENDERINGPATHORDER::DEFERRED, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>()));

	DeferredCalLightUnit = std::make_shared<GameEngineRenderUnit>();

	DeferredCalLightUnit->SetMesh("FullRect");
	DeferredCalLightUnit->SetMaterial("CalDeferredLight");
	DeferredCalLightUnit->ShaderResources.SetConstantBufferLink("LightDatas", LightDataObject);

	DeferredMergeUnit = std::make_shared<GameEngineRenderUnit>();

	DeferredMergeUnit->SetMesh("FullRect");
	DeferredMergeUnit->SetMaterial("CalDeferredMerge");
	
}

GameEngineCamera::~GameEngineCamera() 
{
}

bool ZSort(std::shared_ptr<GameEngineRenderer> _Left, std::shared_ptr<GameEngineRenderer> _Right)
{
	return _Left->GetTransform().GetWorldPosition().z > _Right->GetTransform().GetWorldPosition().z;
}

bool ZSortUnit(std::shared_ptr<GameEngineRenderUnit> _Left, std::shared_ptr<GameEngineRenderUnit> _Right)
{
	return _Left->GetRenderer()->GetTransform().GetWorldPosition().z > _Right->GetRenderer()->GetTransform().GetWorldPosition().z;
}

GameEngineInstancing& GameEngineCamera::GetInstancing(const std::string& _Name)
{
	return InstancingMap[_Name];
}

void GameEngineCamera::Render(float _DeltaTime)
{
	// ���� ��ó�� ���۵� ������ ������ �ʰ� ����ϴ� �������� ����Ѵ�.
	// ���������κ��� �����Ͷ����� �ܰ������� ������ ���� �����ű� ������.
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortDesc);

	// �����ϱ� ���� 
	View.LookToLH(
		GetActor()->GetTransform().GetLocalPosition(), 
		GetActor()->GetTransform().GetForwardVector(), 
		GetActor()->GetTransform().GetUpVector());

	switch (Mode)
	{
	case CAMERAPROJECTIONMODE::PersPective:
		Projection.PerspectiveFovLH(Fov, Size.x, Size.y, Near, Far);
		break;
	case CAMERAPROJECTIONMODE::Orthographic:
		Projection.OrthographicLH(Size.x, Size.y, Near, Far);
		break;
	default:
		break;
	}

	float4 WindowSize = GameEngineWindow::GetInst()->GetScale();

	{
		LightDataObject.Count = static_cast<int>(AllLight.size());
		int LightCount = 0;
		for ( std::shared_ptr<GameEngineLight> Light : AllLight)
		{
			Light->LightDataUpdate(this);
			LightDataObject.Lights[LightCount++] = Light->GetLightData();
		}
	}



	// ������ Ÿ���� ���õǰ�
	CameraForwardRenderTarget->Clear();
	CameraForwardRenderTarget->Setting();
	CurTarget = CameraForwardRenderTarget;

	// ��� ���� �����ϰ�
	for (std::pair<const int, std::list<std::shared_ptr<GameEngineRenderer>>>& Group : AllRenderer_)
	{
		float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);

		std::list<std::shared_ptr<GameEngineRenderer>>& RenderList = Group.second;
		RenderList.sort(ZSort);

		for (std::shared_ptr<GameEngineRenderer>& Renderer : Group.second)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			Renderer->RenderOptionInst.DeltaTime = _DeltaTime;
			Renderer->RenderOptionInst.SumDeltaTime += _DeltaTime;
			Renderer->GetTransform().SetView(View);
			Renderer->GetTransform().SetProjection(Projection);
			Renderer->GetTransform().CalculateWorldViewProjection();
		}
	}

	{
		std::map<RENDERINGPATHORDER, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>::iterator ForwardIter 
			= AllRenderUnit_.find(RENDERINGPATHORDER::FORWARD);

		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& OrderMap = ForwardIter->second;

		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderStartIter = OrderMap.begin();
		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderEndIter = OrderMap.end();

		for (std::pair<const int, std::list<std::shared_ptr<GameEngineRenderUnit>>>& Group : OrderMap)
		{
			float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);

			std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderList = Group.second;
			RenderList.sort(ZSortUnit);

			for (std::shared_ptr<GameEngineRenderUnit>& Unit : Group.second)
			{
				if (false == Unit->GetIsOn())
				{
					continue;
				}
				// �ν��Ͻ� ���� ����
				Unit->Render(ScaleTime);
			}
		}
	}

	//// ������ �ν��Ͻ��� ������
	{
		// ���̴� ���ҽ� ������ �ٸ��ֵ��� ������
		std::unordered_map<std::string, GameEngineInstancing>::iterator StartIter = InstancingMap.begin();
		std::unordered_map<std::string, GameEngineInstancing>::iterator EndIter = InstancingMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			StartIter->second.RenderInstancing(_DeltaTime);
		}
	}

	// ������ Ÿ���� ���õǰ�
	CameraDeferredGBufferRenderTarget->Clear(false);
	CameraDeferredGBufferRenderTarget->Setting();
	CurTarget = CameraDeferredGBufferRenderTarget;

	{
		std::map<RENDERINGPATHORDER, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>::iterator ForwardIter 
			= AllRenderUnit_.find(RENDERINGPATHORDER::DEFERRED);

		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& OrderMap = ForwardIter->second;

		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderStartIter = OrderMap.begin();
		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderEndIter = OrderMap.end();

		for (std::pair<const int, std::list<std::shared_ptr<GameEngineRenderUnit>>>& Group : OrderMap)
		{
			float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);

			std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderList = Group.second;
			RenderList.sort(ZSortUnit);

			for (std::shared_ptr<GameEngineRenderUnit>& Unit : Group.second)
			{
				if (false == Unit->GetIsOn())
				{
					continue;
				}
				// �ν��Ͻ� ���� ����
				Unit->Render(ScaleTime);
			}
		}
	}

	// ���۵��� ������� �� ���Դ�.

	// CameraDeferredRenderTarget �̳༮�� Ÿ������ ������ �׷��� �Ѵ�.
	CameraDeferredLightRenderTarget->Clear();
	CameraDeferredLightRenderTarget->Effect(DeferredCalLightUnit);


	CameraDeferredRenderTarget->Clear();
	CameraDeferredRenderTarget->Effect(DeferredMergeUnit);


	CameraRenderTarget->Clear();
	CameraRenderTarget->Merge(CameraForwardRenderTarget);
	CameraRenderTarget->Merge(CameraDeferredRenderTarget);

}

void GameEngineCamera::SetCameraOrder(CAMERAORDER _Order)
{
	GetActor()->GetLevel()->PushCamera(std::dynamic_pointer_cast<GameEngineCamera>(shared_from_this()), _Order);
}

void GameEngineCamera::Start()
{
	CameraRenderTarget = GameEngineRenderTarget::Create();

	CameraRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	CameraRenderTarget->SettingDepthTexture(GameEngineDevice::GetBackBuffer()->GetDepthTexture());

	// ������ Ÿ�� �����.
	CameraForwardRenderTarget = GameEngineRenderTarget::Create();

	CameraForwardRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	// CameraForwardRenderTarget->CreateDepthTexture();
	CameraForwardRenderTarget->SettingDepthTexture(GameEngineDevice::GetBackBuffer()->GetDepthTexture());

	// ���۵� Ÿ�� �����
	CameraDeferredGBufferRenderTarget = GameEngineRenderTarget::Create();
	// ��ǻ����� 0
	CameraDeferredGBufferRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);
	// ������ 1
	CameraDeferredGBufferRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);
	// �븻 2
	CameraDeferredGBufferRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	DeferredCalLightUnit->ShaderResources.SetTexture("PositionTex", CameraDeferredGBufferRenderTarget->GetRenderTargetTexture(1));
	DeferredCalLightUnit->ShaderResources.SetTexture("NormalTex", CameraDeferredGBufferRenderTarget->GetRenderTargetTexture(2));

	CameraDeferredGBufferRenderTarget->SettingDepthTexture(GameEngineDevice::GetBackBuffer()->GetDepthTexture());

	// ���ο� Ÿ�� ����Ʈ Ÿ��
	CameraDeferredLightRenderTarget = GameEngineRenderTarget::Create();
	// ��ǻ�����Ʈ
	CameraDeferredLightRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);
	// ����ŧ��
	CameraDeferredLightRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);
	// �ں��Ʈ
	CameraDeferredLightRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	// 
	CameraDeferredRenderTarget = GameEngineRenderTarget::Create();

	CameraDeferredRenderTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	CameraDeferredRenderTarget->SettingDepthTexture(GameEngineDevice::GetBackBuffer()->GetDepthTexture());

	//DiffuseTex
	//DiffuseLightTex 
	//SpacularLightTex
	//AmbientLightTex 

	DeferredMergeUnit->ShaderResources.SetTexture("DiffuseTex", CameraDeferredGBufferRenderTarget->GetRenderTargetTexture(0));

	DeferredMergeUnit->ShaderResources.SetTexture("DiffuseLightTex", CameraDeferredLightRenderTarget->GetRenderTargetTexture(0));

	DeferredMergeUnit->ShaderResources.SetTexture("SpacularLightTex", CameraDeferredLightRenderTarget->GetRenderTargetTexture(1));

	DeferredMergeUnit->ShaderResources.SetTexture("AmbientLightTex", CameraDeferredLightRenderTarget->GetRenderTargetTexture(2));

	// CameraRenderTarget->CreateDepthTexture()
}

// n���� ���� ����Ʈ�� �ִ°� �̹� �������־� 
void GameEngineCamera::PushRenderer(std::shared_ptr<GameEngineRenderer> _Renderer)
{
	AllRenderer_[_Renderer->RenderingOrder].push_back(_Renderer);
}

void GameEngineCamera::PushRenderUnit(std::shared_ptr < GameEngineRenderUnit> _RenderUnit)
{
	if (nullptr == _RenderUnit)
	{
		MsgBoxAssert("��������Ʈ�� �������� �ʽ��ϴ�.");
	}

	RENDERINGPATHORDER Path = RENDERINGPATHORDER::FORWARD;

	// ���⿡�� ���� ������ ����.
	if (nullptr != _RenderUnit->GetMaterial())
	{
		if (true == _RenderUnit->GetMaterial()->GetPixelShader()->GetIsDeferred())
		{
			Path = RENDERINGPATHORDER::DEFERRED;
		}
	}

	_RenderUnit->SetPath(Path);

	AllRenderUnit_[Path][_RenderUnit->GetRenderer()->RenderingOrder].push_back(_RenderUnit);
}


void GameEngineCamera::Release(float _DelataTime)
{
	{
		for (size_t i = 0; i < static_cast<size_t>(RENDERINGPATHORDER::MAX); i++)
		{
			std::map<RENDERINGPATHORDER, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>::iterator ForwardIter
				= AllRenderUnit_.find(static_cast<RENDERINGPATHORDER>(i));

			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& OrderMap = ForwardIter->second;

			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderStartIter = OrderMap.begin();
			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderEndIter = OrderMap.end();

			for (std::pair<const int, std::list<std::shared_ptr<GameEngineRenderUnit>>>& Group : OrderMap)
			{
				std::list<std::shared_ptr<GameEngineRenderUnit>>& List = Group.second;

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator GroupStart = List.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator GroupEnd = List.end();

				for (; GroupStart != GroupEnd; )
				{
					if (true == (*GroupStart)->IsDeath())
					{
						GroupStart = List.erase(GroupStart);
					}
					else
					{
						++GroupStart;
					}

				}
			}
		}

	}

	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator StartGroupIter = AllRenderer_.begin();
	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator EndGroupIter = AllRenderer_.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<std::shared_ptr<GameEngineRenderer>>& Group = StartGroupIter->second;
		std::list<std::shared_ptr<GameEngineRenderer>>::iterator GroupStart = Group.begin();
		std::list<std::shared_ptr<GameEngineRenderer>>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			(*GroupStart)->ReleaseUpdate(_DelataTime);
			if (true == (*GroupStart)->IsDeath())
			{
				GroupStart = Group.erase(GroupStart);
			}
			else
			{
				++GroupStart;
			}

		}
	}
}

float4 GameEngineCamera::GetMouseScreenPosition() 
{
	POINT P;

	GetCursorPos(&P);

	ScreenToClient(GameEngineWindow::GetHWND(), &P);

	return { static_cast<float>(P.x), static_cast<float>(P.y) };
}

void GameEngineCamera::Update(float _DeltaTime) 
{
	float4 MousePos = GetMouseWorldPosition();
	MousePos.w = 0.0f;
	MouseDir = MousePos - PrevMouse;
	PrevMouse = MousePos;
}

// ����Ʈ�� �ִ°���?
float4 GameEngineCamera::GetMouseWorldPosition()
{
	float4 Pos = GetMouseScreenPosition();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.x, Size.y, 0, 0, 0, 1);
	ViewPort.Inverse();

	float4x4 ProjectionInvers = Projection.InverseReturn();

	Pos = Pos * ViewPort;
	Pos = Pos * ProjectionInvers;
	// ���콺�� ����Ʈ�� ��ǥ��?

	return Pos;
}


float4 GameEngineCamera::GetMouseWorldPositionToActor()
{
	return GetTransform().GetWorldPosition() + GetMouseWorldPosition();
}

void GameEngineCamera::ChangeRenderingOrder(std::shared_ptr<GameEngineRenderer> _Renderer, int _ChangeOrder)
{
	// 0��°���� �����ǰ�
	AllRenderer_[_Renderer->GetRenderingOrder()].remove(_Renderer);

	_Renderer->RenderingOrder = _ChangeOrder;

	// 10000��°�� �̵��Ѵ�.
	AllRenderer_[_Renderer->GetRenderingOrder()].push_back(_Renderer);
}

void GameEngineCamera::OverRenderer(std::shared_ptr < GameEngineCamera> _NextCamera)
{
	if (nullptr == _NextCamera)
	{
		MsgBoxAssert("next camera is nullptr! fuck you");
		return;
	}

	for (size_t i = 0; i < static_cast<size_t>(RENDERINGPATHORDER::MAX); i++)
	{
		std::map<RENDERINGPATHORDER, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>::iterator ForwardIter
			= AllRenderUnit_.find(static_cast<RENDERINGPATHORDER>(i));

		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& OrderMap = ForwardIter->second;
		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderStartIter = OrderMap.begin();
		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>::iterator OrderEndIter = OrderMap.end();

		for (std::pair<const int, std::list<std::shared_ptr<GameEngineRenderUnit>>>& Group : OrderMap)
		{
			std::list<std::shared_ptr<GameEngineRenderUnit>>& List = Group.second;

			std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator GroupStart = List.begin();
			std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator GroupEnd = List.end();

			for (; GroupStart != GroupEnd; )
			{
				std::shared_ptr<GameEngineRenderUnit> Unit = (*GroupStart);

				if (nullptr == Unit->GetRenderer())
				{
					MsgBoxAssert("�������� �������� �ʴ� ����Ʈ �Դϴ�.");
				}

				std::shared_ptr<GameEngineActor> Root = Unit->GetRenderer()->GetRoot<GameEngineActor>();

				if (true == Root->IsLevelOver)
				{
					_NextCamera->AllRenderUnit_[static_cast<RENDERINGPATHORDER>(i)][OrderStartIter->first].push_back(*GroupStart);
					GroupStart = List.erase(GroupStart);
				}
				else
				{
					++GroupStart;
				}
			}
		}
	}


	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator StartGroupIter = AllRenderer_.begin();
	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator EndGroupIter = AllRenderer_.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<std::shared_ptr<GameEngineRenderer>>& Group = StartGroupIter->second;
		std::list<std::shared_ptr<GameEngineRenderer>>::iterator GroupStart = Group.begin();
		std::list<std::shared_ptr<GameEngineRenderer>>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			std::shared_ptr<GameEngineActor> Root = (*GroupStart)->GetRoot<GameEngineActor>();

			if (true == Root->IsLevelOver)
			{
				_NextCamera->AllRenderer_[StartGroupIter->first].push_back(*GroupStart);
				GroupStart = Group.erase(GroupStart);
			}
			else
			{
				++GroupStart;
			}

		}
	}
}

float4 GameEngineCamera::GetWorldPositionToScreenPosition(const float4& _Pos)
{
	float4 Pos = _Pos;

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.x, Size.y, 0, 0, 0, 1);

	Pos = Pos * View;
	Pos = Pos * Projection;
	Pos /= Pos.w;

	Pos = Pos * ViewPort;
	return Pos;
}

void GameEngineCamera::PushLight(std::shared_ptr<class GameEngineLight> _Light)
{
	if (true == AllLight.contains(_Light))
	{
		return;
	}

	AllLight.insert(_Light);
}