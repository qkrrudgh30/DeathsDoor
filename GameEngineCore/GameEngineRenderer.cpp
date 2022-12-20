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
#include "GameEngineCamera.h"
#include "GameEnginePixelShader.h"

#include "GameEngineInputLayOut.h"


//////////////////////////////////////////////////////// GameEngineRenderUnit 

GameEngineRenderUnit::GameEngineRenderUnit() 
	: ParentRenderer()
	, Material(nullptr)
	, Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, InputLayOut(nullptr)
	, IsOn(true)
	, Mesh(nullptr)
	, RenderFunction(nullptr)
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

void GameEngineRenderUnit::EngineShaderResourcesSetting(GameEngineRenderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		return;
	}

	ParentRenderer = _Renderer;

	GameEngineCamera* Camera = ParentRenderer->GetCamera();

	if (true == ShaderResources.IsConstantBuffer("LightDatas"))
	{
		ShaderResources.SetConstantBufferLink("LightDatas", &Camera->LightDataObject, sizeof(LightDatas));
	}

	// ���������Դ� ī�޶� ������ ���õǾ� �־�� �Ѵ�.

	//// ������ ������ ������ �Ѵ�.
	if (true == ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	{
		ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &ParentRenderer->GetTransformData(), sizeof(TransformData));
	}

	if (true == ShaderResources.IsConstantBuffer("RENDEROPTION"))
	{
		ShaderResources.SetConstantBufferLink("RENDEROPTION", &ParentRenderer->RenderOptionInst, sizeof(RenderOption));
	}

}

void GameEngineRenderUnit::SetMesh(const std::string& _Name)
{
	Mesh = GameEngineMesh::Find(_Name);

	if (nullptr == Mesh)
	{
		MsgBoxAssert("�������� �ʴ� �Ž��� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	if (nullptr == InputLayOut && nullptr != Material)
	{
		// ������������ ���ؽ� ���̴���
		// �Ž��� ���ؽ� ������ �� �������Ƿ�
		// ��ǲ ���̾ƿ��� ������� �ִ�.
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}

}

void GameEngineRenderUnit::SetMesh(std::shared_ptr<GameEngineMesh> _Mesh)
{
	if (nullptr == _Mesh)
	{
		MsgBoxAssert("�������� �ʴ� �Ž��� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	Mesh = _Mesh;

	if (nullptr == InputLayOut && nullptr != Material)
	{
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}
}

void GameEngineRenderUnit::SetMaterial(const std::string& _Name)
{
	Material = GameEngineMaterial::Find(_Name);

	if (nullptr == Material)
	{
		MsgBoxAssert("�������� �ʴ� ������������ �����Ϸ��� �߽��ϴ�.");
		return;
	}


	if (nullptr == InputLayOut && nullptr != Mesh)
	{
		// ������������ ���ؽ� ���̴���
		// �Ž��� ���ؽ� ������ �� �������Ƿ�
		// ��ǲ ���̾ƿ��� ������� �ִ�.
		InputLayOut = GameEngineInputLayOut::Create(Mesh->GetLayOutDesc(), Material->GetVertexShader());
	}

	ShaderResources.ResourcesCheck(Material);

	EngineShaderResourcesSetting(ParentRenderer);
}

void GameEngineRenderUnit::PushCamera()
{
	if (nullptr == ParentRenderer)
	{
		MsgBoxAssert("�θ𷣴����� ���õ��� ���� ���¿��� ī�޶� ������ �߽��ϴ�.");
	}

	//if (nullptr == Material)
	//{
	//	MsgBoxAssert("���׸����� ���õ��� ���� ���¿��� ī�޶� ������ �߽��ϴ�.");
	//}

	GameEngineCamera* Camera = ParentRenderer->GetCamera();

	if (nullptr == Camera)
	{
		MsgBoxAssert("ī�޶� ���õ��� ���� �������Դϴ�.");
	}

	Camera->PushRenderUnit(shared_from_this());
}

void GameEngineRenderUnit::SetRenderer(GameEngineRenderer* _Renderer)
{
	ParentRenderer = _Renderer;

	EngineShaderResourcesSetting(_Renderer);
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
	// �̸����� �༮���� �����.
	std::shared_ptr < GameEngineMaterial> Clone = GameEngineMaterial::Create();
	Clone->Copy(_Rendering);
	return Clone;
}

void GameEngineRenderUnit::RenderInstancing(float _DeltaTime, size_t _RanderingCount, std::shared_ptr<GameEngineInstancingBuffer> _Buffer)
{
	if (nullptr == Material)
	{
		MsgBoxAssert("������ ������������ ���õ��� ������ �������� �Ҽ� �����ϴ�.");
	}

	if (nullptr == Mesh)
	{
		MsgBoxAssert("�Ž��� �����Ƿ� �������� �Ҽ� �����ϴ�.");
	}

	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("��ǲ ���̾ƿ��� �����Ƿ� �������� �Ҽ� �����ϴ�.");
	}


	// �� �Ž��� 
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

	if (nullptr != RenderFunction)
	{
		if (false == RenderFunction(_DeltaTime))
		{
			return;
		} 
	}

	if (nullptr == Material)
	{
		MsgBoxAssert("������ ������������ ���õ��� ������ �������� �Ҽ� �����ϴ�.");
	}

	if (nullptr == Mesh)
	{
		MsgBoxAssert("�Ž��� �����Ƿ� �������� �Ҽ� �����ϴ�.");
	}

	if (nullptr == InputLayOut)
	{
		MsgBoxAssert("��ǲ ���̾ƿ��� �����Ƿ� �������� �Ҽ� �����ϴ�.");
	}

	//if (false == IsInstancing(GetPipeLine()))
	//{
		// �غ�� ��� ���ҽ����� �� �������ش�.

	// �� �Ž��� 
	Mesh->Setting();

	InputLayOut->Setting();

	// �� �׸��� �������
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology);
	// �� ���̴��� ����� 
	Material->Setting();
	// �� �����͸� �������
	ShaderResources.AllResourcesSetting();
	// �׷���
	Mesh->Render();

	// �׷���

	ShaderResources.AllResourcesReset();
	//}
	//else
	//{
	//	InstancingDataSetting(GetPipeLine());
	//	// �����е��� ���ο� �������� ����� �ν��Ͻ��� �ϸ�
	//	// �� �κ��� �޶����� �մϴ�.
	//	// ������ �����Ʈ¥�� �ν���
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
	Camera->ChangeRenderingOrder(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()), _Order);
}

void GameEngineRenderer::PushRendererToUICamera()
{
	GetActor()->GetLevel()->PushRendererToUICamera(std::dynamic_pointer_cast<GameEngineRenderer>(shared_from_this()));
}

std::shared_ptr<GameEngineRenderUnit> GameEngineRenderer::CreateRenderUnit()
{
	std::shared_ptr<GameEngineRenderUnit> Unit = std::make_shared<GameEngineRenderUnit>();


	Unit->SetRenderer(this);

	Units.push_back(Unit);

	return Unit;
}

//void GameEngineRenderer::Render(float _DeltaTime)
//{
//	//// ������
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
//	//	// 0 ��° ������ ���� �˴ϴ�.
//	//	// ���ʿ� ���� �Ž��� ���� �����մϴ�.
//	//	CopyBuffer[i] = Vertex->Vertexs[TriIndex];
//
//	//	auto& tran = GetTransform();
//
//	//	// ���ؽ����̴�
//	//	CopyBuffer[i] = CopyBuffer[i] * GetTransform().GetWorldViewProjection();
//
//
//	//	// �����Ͷ�����
//	//	//// ����س��� z������ �����°�
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

