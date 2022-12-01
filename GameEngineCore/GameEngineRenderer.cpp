#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineWindow.h>
#include "GameEngineCamera.h"
#include "GameEngineInstancing.h"


#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineMesh.h"

#include "GameEngineInputLayOut.h"


//////////////////////////////////////////////////////// GameEngineRenderUnit 

GameEngineRenderUnit::GameEngineRenderUnit() 
	: ParentRenderer()
	, Material(nullptr)
	, Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, InputLayOut(nullptr)  
	, IsOn(true)
{
	SetMesh("rect");
}

GameEngineRenderUnit::GameEngineRenderUnit(const GameEngineRenderUnit& _Render)
{
	ParentRenderer = _Render.ParentRenderer;
	Mesh = _Render.Mesh;
	Material = _Render.Material;
	InputLayOut = _Render.InputLayOut;
	Topology = _Render.Topology;

	if (nullptr == Material)
	{
		return;
	}
	ShaderResources.ResourcesCheck(Material);
}

void GameEngineRenderUnit::EngineShaderResourcesSetting(std::shared_ptr<GameEngineRenderer> _Renderer)
{
	if (nullptr == _Renderer)
	{
		return;
	}

	ParentRenderer = _Renderer;

	//// 랜더러 쪽으로 빠져야 한다.
	if (true == ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	{
		ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &ParentRenderer.lock()->GetTransformData(), sizeof(TransformData));
	}

	if (true == ShaderResources.IsConstantBuffer("RENDEROPTION"))
	{
		ShaderResources.SetConstantBufferLink("RENDEROPTION", &ParentRenderer.lock()->RenderOptionInst, sizeof(RenderOption));
	}

}

void GameEngineRenderUnit::SetMesh(const std::string& _Name)
{
	Mesh = GameEngineMesh::Find(_Name);

	if (nullptr == Mesh)
	{
		MsgBoxAssert("존재하지 않는 매쉬를 세팅하려고 했습니다.");
		return;
	}

	if (nullptr == InputLayOut && nullptr != Material)
	{
		// 파이프라인의 버텍스 쉐이더와
		// 매쉬의 버텍스 정보가 다 모여있으므로
		// 인풋 레이아웃을 만들수가 있다.
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}

}

void GameEngineRenderUnit::SetMesh(std::shared_ptr<GameEngineMesh> _Mesh)
{
	if (nullptr == _Mesh)
	{
		MsgBoxAssert("존재하지 않는 매쉬를 세팅하려고 했습니다.");
		return;
	}

	Mesh = _Mesh;

	if (nullptr == InputLayOut && nullptr != Material)
	{
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}
}

void GameEngineRenderUnit::SetPipeLine(const std::string& _Name)
{
	Material = GameEngineMaterial::Find(_Name);

	if (nullptr == Material)
	{
		MsgBoxAssert("존재하지 않는 파이프라인을 세팅하려고 했습니다.");
		return;
	}


	if (nullptr == InputLayOut && nullptr != Mesh)
	{
		// 파이프라인의 버텍스 쉐이더와
		// 매쉬의 버텍스 정보가 다 모여있으므로
		// 인풋 레이아웃을 만들수가 있다.
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}


	ShaderResources.ResourcesCheck(Material);

}

void GameEngineRenderUnit::SetRenderer(std::shared_ptr<GameEngineRenderer> _Renderer)
{
	ParentRenderer = _Renderer;

	EngineShaderResourcesSetting(ParentRenderer.lock());
}

std::shared_ptr<GameEngineMesh> GameEngineRenderUnit::GetMesh()
{
	return Mesh;
}

std::shared_ptr<GameEngineMaterial> GameEngineRenderUnit::GetMaterial()
{
	return Material;
}

std::shared_ptr < GameEngineMaterial> GameEngineRenderUnit::GetCloneMaterial()
{
	if (false == Material->IsOriginal())
	{
		return Material;
	}

	Material = CloneMaterial(Material);
	return Material;
}


std::shared_ptr < GameEngineMaterial> GameEngineRenderUnit::CloneMaterial(std::shared_ptr<GameEngineMaterial> _Rendering)
{
	// 이름없는 녀석으로 만든다.
	std::shared_ptr < GameEngineMaterial> Clone = GameEngineMaterial::Create();
	Clone->Copy(_Rendering);
	return Clone;
}

void GameEngineRenderUnit::RenderInstancing(float _DeltaTime, size_t _RanderingCount, std::shared_ptr<GameEngineInstancingBuffer> _Buffer)
{
	if (nullptr == Material)
	{
		MsgBoxAssert("랜더링 파이프라인이 세팅되지 않으면 랜더링을 할수 없습니다.");
	}

	if (nullptr == Mesh)
	{
		MsgBoxAssert("매쉬가 없으므로 랜더링을 할수 없습니다.");
	}

	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("인풋 레이아웃이 없으므로 랜더링을 할수 없습니다.");
	}


	// 이 매쉬를 
	Mesh->SettingInstancing(_Buffer);
	InputLayOut->Setting();
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology);
	Material->SettingInstancing();
	ShaderResources.AllResourcesSetting();

	// GameEngineDevice::GetContext()->DrawIndexedInstanced(IndexBuffer->GetIndexCount(), _RenderingCount, 0, 0, 0);


	Mesh->RenderInstancing(_RanderingCount);
	ShaderResources.AllResourcesReset();

}

void GameEngineRenderUnit::Render(float _DeltaTime)
{
	if (false == IsOn)
	{
		return;
	}

	if (nullptr == Material)
	{
		MsgBoxAssert("랜더링 파이프라인이 세팅되지 않으면 랜더링을 할수 없습니다.");
	}

	if (nullptr == Mesh)
	{
		MsgBoxAssert("매쉬가 없으므로 랜더링을 할수 없습니다.");
	}

	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("인풋 레이아웃이 없으므로 랜더링을 할수 없습니다.");
	}

	//if (false == IsInstancing(GetPipeLine()))
	//{
		// 준비된 모든 리소스들을 다 세팅해준다.

	// 이 매쉬를 
	Mesh->Setting();

	InputLayOut->Setting();

	// 이 그리는 방식으로
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology);
	// 이 쉐이더와 색깔로 
	Material->Setting();
	// 이 데이터를 기반으로
	ShaderResources.AllResourcesSetting();
	// 그려라
	Mesh->Render();

	// 그려라

	ShaderResources.AllResourcesReset();
	//}
	//else
	//{
	//	InstancingDataSetting(GetPipeLine());
	//	// 여러분들이 새로운 랜더러를 만들고 인스턴싱을 하면
	//	// 이 부분이 달라져야 합니다.
	//	// 유저가 몇바이트짜리 인스턴
	//	// Camera->PushInstancingIndex(PipeLine);
	//}
}

//////////////////////////////////////////////////////// GameEngineRenderer

GameEngineRenderer::GameEngineRenderer()
	: CameraOrder(CAMERAORDER::MAINCAMERA)
	, RenderingOrder(0)
{
}

GameEngineRenderer::~GameEngineRenderer() 
{
}

void GameEngineRenderer::Start() 
{
	PushRendererToMainCamera();
}

void GameEngineRenderer::PushRendererToMainCamera()
{
	GetActor()->GetLevel()->PushRendererToMainCamera(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()));
}

void GameEngineRenderer::SetRenderingOrder(int _Order)
{
	Camera.lock()->ChangeRenderingOrder(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()), _Order);
}

void GameEngineRenderer::PushRendererToUICamera()
{
	GetActor()->GetLevel()->PushRendererToUICamera(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()));
}


//void GameEngineRenderer::Render(float _DeltaTime)
//{
//	//// 랜더링
//	////GameEngineVertexBuffer* Vertex = GameEngineVertexBuffer::Find("Rect");
//	////GameEngineIndexBuffer* Index = GameEngineIndexBuffer::Find("Rect");
//
//	//GameEngineVertexBuffer* Vertex = GameEngineVertexBuffer::Find("Box");
//	//GameEngineIndexBuffer* Index = GameEngineIndexBuffer::Find("Box");
//	//
//	//std::vector<POINT> DrawVertex;
//	//DrawVertex.resize(Index->Indexs.size());
//
//	//std::vector<float4> CopyBuffer;
//	//CopyBuffer.resize(Index->Indexs.size());
//
//
//	//for (size_t i = 0; i < Index->Indexs.size(); i++)
//	//{
//	//	int TriIndex = Index->Indexs[i];
//
//	//	// 0 번째 순서의 점이 됩니다.
//	//	// 최초에 원본 매쉬의 점을 복사합니다.
//	//	CopyBuffer[i] = Vertex->Vertexs[TriIndex];
//
//	//	auto& tran = GetTransform();
//
//	//	// 버텍스쉐이더
//	//	CopyBuffer[i] = CopyBuffer[i] * GetTransform().GetWorldViewProjection();
//
//
//	//	// 레스터라이저
//	//	//// 기록해놨던 z값으로 나뉘는것
//	//	CopyBuffer[i] = CopyBuffer[i] / CopyBuffer[i].w;
//
//
//	//	DrawVertex[i] = CopyBuffer[i].GetConvertWindowPOINT();
//	//}
//	//
//	//for (size_t i = 0; i < DrawVertex.size(); i += 3)
//	//{
//	//	Polygon(GameEngineWindow::GetHDC(), &DrawVertex[i], 3);
//	//}
//}

void GameEngineRenderer::ChangeCamera(CAMERAORDER _Order)
{
	GetActor()->GetLevel()->PushRenderer(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()), _Order);
}
