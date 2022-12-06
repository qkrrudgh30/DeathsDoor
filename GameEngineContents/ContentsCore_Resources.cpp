#include "PreCompile.h"
#include "ContentsCore.h"

#include <math.h>

#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>

// Resources Header
#include <GameEngineCore/GameEngineStructuredBuffer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineVertexs.h>
#include <GameEngineCore/GameEngineConstantBuffer.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineVertexBuffer.h>
#include <GameEngineCore/GameEngineIndexBuffer.h>
#include <GameEngineCore/GameEngineInstancingBuffer.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineFolderTexture.h>
#include <GameEngineCore/GameEngineSampler.h>
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineDepthStencil.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineInputLayOut.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineVertexShader.h>
#include <GameEngineCore/GameEnginePixelShader.h>
#include <GameEngineCore/GameEngineRasterizer.h>
#include <GameEngineCore/GameEngineBlend.h>
#include <GameEngineCore/GameEngineMaterial.h>

void CreateContentsMesh()
{

}

void CreateContentsInputLayout()
{
	GameEngineVertex::LayOut.AddInputLayOut("CLONEDTEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
}

void CompileContentsShader()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToExitsChildDirectory("GameEngineResources");
	Dir.Move("ContentsResources");
	Dir.Move("Shader");

	std::vector<GameEngineFile> Shaders = Dir.GetAllFile("hlsl");

	for (size_t i = 0; i < Shaders.size(); i++)
	{
		GameEngineShader::AutoCompile(Shaders[i].GetFullPath());
	}
}

void CreateContentsMaterial()
{
	std::weak_ptr<GameEngineMaterial> FadeMaterial = GameEngineMaterial::Create("Fade");
	FadeMaterial.lock()->SetVertexShader("Fade.hlsl");
	FadeMaterial.lock()->SetPixelShader("Fade.hlsl");

	std::weak_ptr<GameEngineMaterial> ContentsBlurMaterial = GameEngineMaterial::Create("ContentsBlur");
	ContentsBlurMaterial.lock()->SetVertexShader("ContentsBlur.hlsl");
	ContentsBlurMaterial.lock()->SetPixelShader("ContentsBlur.hlsl");

	std::weak_ptr<GameEngineMaterial> ContentsBloomMaterial = GameEngineMaterial::Create("ContentsBloom");
	ContentsBloomMaterial.lock()->SetVertexShader("ContentsBloom.hlsl");
	ContentsBloomMaterial.lock()->SetPixelShader("ContentsBloom.hlsl");

	std::weak_ptr<GameEngineMaterial> ContentsPaperBurnMaterial = GameEngineMaterial::Create("PaperBurn");
	ContentsPaperBurnMaterial.lock()->SetVertexShader("PaperBurn.hlsl");
	ContentsPaperBurnMaterial.lock()->SetPixelShader("PaperBurn.hlsl");

	std::weak_ptr<GameEngineMaterial> ContentsBillboardMaterial = GameEngineMaterial::Create("Billboard");
	ContentsBillboardMaterial.lock()->SetVertexShader("Billboard.hlsl");
	ContentsBillboardMaterial.lock()->SetPixelShader("Billboard.hlsl");

	std::weak_ptr<GameEngineMaterial> ContentsPreviousColorMaterial = GameEngineMaterial::Create("PreviousColor");
	ContentsPreviousColorMaterial.lock()->SetVertexShader("PreviousColor.hlsl");
	ContentsPreviousColorMaterial.lock()->SetPixelShader("PreviousColor.hlsl");
	
	// std::weak_ptr<GameEngineMaterial> ContentsShaderMaterial = GameEngineMaterial::Create("ContentsShader");
	// ContentsBlurMaterial.lock()->SetVertexShader("ContentsShader.hlsl");
	// ContentsBlurMaterial.lock()->SetPixelShader("ContentsShader.hlsl");
}

void CreateContentsBlendAndDepthStencilDesc()
{
	{
		D3D11_BLEND_DESC Desc = { 0 };

		Desc.AlphaToCoverageEnable = FALSE;
			// �ȼ��� ������ ������� ������ �� ����-�˻�(alpha-to-coverage)�� ���� ���ø� ����� ������� ���θ� �����մϴ�.
		Desc.IndependentBlendEnable = FALSE;
			// ���� ������ ��󿡼� �������� ȥ���� ������� ���θ� �����մϴ�. �������� ȥ���� ����ϵ��� �����Ϸ��� TRUE �� �����մϴ�.
			// FALSE�� �����ϸ� RenderTarget[0] ����� ���˴ϴ�. RenderTarget[1..7]�� ���õ˴ϴ�.

		Desc.RenderTarget[0].BlendEnable = true;
			// ȥ���� ����ϰų� ������� �ʵ��� �����մϴ�.

		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			// �ȼ� ���̴��� ����ϴ� RGB ���� ���� ������ �۾��� �����մϴ�. BlendOp ����� SrcBlend �� DestBlend �۾��� �����ϴ� ����� �����մϴ�.
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			// ������ ����� ���� RGB ���� ���� ������ �۾��� �����մϴ�. BlendOp ����� SrcBlend �� DestBlend �۾��� �����ϴ� ����� �����մϴ�.
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
			// SrcBlend �� DestBlend �۾��� �����ϴ� ����� �����մϴ�.

		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			// �ȼ� ���̴��� ����ϴ� ���� ���� ���� ������ �۾��� �����մϴ�. _COLOR ������ ȥ�� �ɼ��� ������ �ʽ��ϴ�. 
			// BlendOpAlpha ����� SrcBlendAlpha �� DestBlendAlpha �۾��� �����ϴ� ����� �����մϴ�.
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			// ������ ����� ���� ���� ���� ���� ������ �۾��� �����մϴ�. _COLOR ������ ȥ�� �ɼ��� ������ �ʽ��ϴ�. 
			// BlendOpAlpha ����� SrcBlendAlpha �� DestBlendAlpha �۾��� �����ϴ� ����� �����մϴ�.
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
			// SrcBlendAlpha �� DestBlendAlpha �۾��� �����ϴ� ����� �����մϴ�.

		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			// ���� ����ũ�Դϴ�. https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_color_write_enable#constants

		GameEngineBlend::Create("Lighten", Desc);
	}
}

void LoadContentsUITexture()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToExitsChildDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Asset");
	Dir.Move("GlobalUI");

	std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

	for (size_t i = 0; i < Shaders.size(); i++)
	{
		GameEngineTexture::Load(Shaders[i].GetFullPath());
	}
}

void ContentsCore::InitializeContentsResource()
{
	CreateContentsMesh();

	CreateContentsInputLayout();

	CompileContentsShader();

	CreateContentsMaterial();

	CreateContentsBlendAndDepthStencilDesc();

	LoadContentsUITexture();
}

void ContentsCore::DestroyContentsResource()
{

}
