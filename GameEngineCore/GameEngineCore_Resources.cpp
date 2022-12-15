#include "PreCompile.h"
#include "GameEngineCore.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include "GameEngineLevel.h"
#include "GameEngineVertexs.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineStructuredBuffer.h"
#include <math.h>


// Resources Header
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineInstancingBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineFont.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineFBXAnimation.h"

#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineRasterizer.h"
#include "GameEngineBlend.h"
#include "GameEngineMaterial.h"

void EngineInputLayOut() 
{
	// �� 1��
	// float4 Postion0
	// float4 Postion1
	// float4 Postion2
	// float4 Postion3
	// float4 Postion4
	// float4 Postion5
	// float4 Postion6

	// �ϳ��� �������ϴ�.
	GameEngineVertex::LayOut.AddInputLayOut("POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 16
	GameEngineVertex::LayOut.AddInputLayOut("TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 32
	GameEngineVertex::LayOut.AddInputLayOut("COLOR", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 48
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 48
	GameEngineVertex::LayOut.AddInputLayOut("BINORMAL", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 48
	GameEngineVertex::LayOut.AddInputLayOut("TANGENT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 48
	GameEngineVertex::LayOut.AddInputLayOut("BLENDWEIGHT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0); // 48
	GameEngineVertex::LayOut.AddInputLayOut("BLENDINDICES", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT, 0); // 48
	GameEngineVertex::LayOut.OffsetReset();

	// �ν��Ͻ� �����Ϳ��� �־��ݴϴ�.
	GameEngineVertex::LayOut.AddInputLayOut("ROWINDEX", DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1); // 48
	// GameEngineVertex::LayOut.AddInputLayOut("MYPOS", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1); // 48
}

void EngineSubSetting()
{
	{
		D3D11_BLEND_DESC Desc = { 0 };

		// ��
		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE;
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		//         ����

		//           src                                         dest
		// �������  float4(1.0f, 0.0f, 0.0f, 0.5f) * �ҽ����� + float4(0.0f, 0.0f, 1.0f, 0.5f) * ��������

		//        = float4(1.0f, 0.0f, 1.0f, 1.0f);
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		// �������  float4(0.0f, 0.0f, 0.0f, 0.0f) * float4(0.0f, 0.0f, 0.0f) + float4(0.0f, 0.0f, 1.0f, 1.0f) * ��������
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		// float4(0.0f, 0.0f, 1.0f, 1.0f)* (0.5f, 0.5f, 0.5f)
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		// �����ʸ� ���� ó���ϴ� �ɼ�
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		// Pixel Shader                RenderTarget         
		// src color                   dest color
		// 
		//                                              ���� ���ϴ� ��� ��   0,0,0,0
		// 0, 0, 0, 1  * (1, 1, 1, 1) +  0, 0, 0, 0 * (1-1, 1-1, 1-1, 1-1) => 0,0,0,0
		// 0, 0, 0, 1  +  0, 0, 0, 0 => 0,0,0,1
		// 1 + 0 = 1


		// ������         0, 0, 1, 1


		GameEngineBlend::Create("AlphaBlend", Desc);
	}

	{
		// 1, 1, 1, 1 * ���� + 0, 0��, * ����

		D3D11_BLEND_DESC Desc = { 0 };

		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE;
		Desc.RenderTarget[0].BlendEnable = true;
		// Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		// Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		// Pixel Shader                RenderTarget         
		// src color                   dest color
		//                                              ���� ���ϴ� ��� ��   0,0,0,0
		// 0, 0, 0, 1  * (1, 1, 1, 1) +  0, 0, 0, 0 * (1, 1, 1, 1) => 0,0,0,0
		// 0, 0, 0, 1  +  0, 0, 0, 0 => 0,0,0,0
		// 1 + 0 = 1


		// ������         0, 0, 1, 1


		GameEngineBlend::Create("TransparentBlend", Desc);
	}

	{
		//D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
		//D3D11_CULL_MODE CullMode = D3D11_CULL_NONE;
		//BOOL FrontCounterClockwise = FALSE;
		//INT DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
		//FLOAT DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
		//FLOAT SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		//BOOL DepthClipEnable = TRUE;
		//BOOL ScissorEnable = FALSE;
		//BOOL MultisampleEnable = TRUE;
		//BOOL AntialiasedLineEnable = FALSE;

		//D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		//	D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		//	D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, TRUE, FALSE };

		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_SOLID, D3D11_CULL_NONE };

		GameEngineRasterizer::Create("EngineRasterizer", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = {0};

		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;
		GameEngineDepthStencil::Create("EngineBaseDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };

		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;
		GameEngineDepthStencil::Create("AlwaysDepth", Desc);
	}
	
}

void EngineTextureLoad() 
{
	{
		D3D11_SAMPLER_DESC Desc = {D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT};
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.MipLODBias = 0.0f;
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("POINTCLAMP", Desc);
	}

	{
		D3D11_SAMPLER_DESC Desc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.MipLODBias = 0.0f;
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("POINTWRAP", Desc);
	}

	{
		D3D11_SAMPLER_DESC Desc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR };
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.MipLODBias = 0.0f;
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("LINEARWRAP", Desc);
	}

	{
		D3D11_SAMPLER_DESC Desc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR };
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		Desc.MipLODBias = 0.0f;
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("LINEARMIRROR", Desc);
	}


	GameEngineDirectory Dir;

	Dir.MoveParentToExitsChildDirectory("GameEngineResources");
	Dir.Move("GameEngineResources");
	Dir.Move("Texture");

	std::vector<GameEngineFile> Shaders = Dir.GetAllFile();

	for (size_t i = 0; i < Shaders.size(); i++)
	{
		GameEngineTexture::Load(Shaders[i].GetFullPath());
	}
}

void ShaderCompile()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToExitsChildDirectory("GameEngineResources");
	Dir.Move("GameEngineResources");
	Dir.Move("GameEngineShader");

	std::vector<GameEngineFile> Shaders = Dir.GetAllFile("hlsl");

	for (size_t i = 0; i < Shaders.size(); i++)
	{
		GameEngineShader::AutoCompile(Shaders[i].GetFullPath());
	}
}



void EngineRenderingPipeLine()
{
	

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("CalDeferredLight");
		NewPipe->SetVertexShader("CalDeferredLight.hlsl");
		NewPipe->SetPixelShader("CalDeferredLight.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("Color");
		NewPipe->SetVertexShader("Color.hlsl");
		NewPipe->SetPixelShader("Color.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("DeferredColor");
		NewPipe->SetVertexShader("DeferredColor.hlsl");
		NewPipe->SetPixelShader("DeferredColor.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("Texture");
		NewPipe->SetVertexShader("Texture.hlsl");
		NewPipe->SetPixelShader("Texture.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("TextureAtlas");
		NewPipe->SetVertexShader("TextureAtlas.hlsl");
		NewPipe->SetPixelShader("TextureAtlas.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("TextureAnimation");
		NewPipe->SetVertexShader("TextureAnimation.hlsl");
		NewPipe->SetPixelShader("TextureAnimation.hlsl");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("3DDebug");
		//NewPipe->SetInputAssembler1VertexBuffer("Box");
		//NewPipe->SetInputAssembler2IndexBuffer("Box");
		NewPipe->SetVertexShader("Debug3D.hlsl");
		NewPipe->SetPixelShader("Debug3D.hlsl");
		NewPipe->SetOutputMergerDepthStencil("AlwaysDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("TargetMerge");
		//NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		//NewPipe->SetInputAssembler2IndexBuffer("FullRect");
		NewPipe->SetVertexShader("TargetMerge.hlsl");
		NewPipe->SetPixelShader("TargetMerge.hlsl");
		NewPipe->SetOutputMergerDepthStencil("AlwaysDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("DebugTexture");
		NewPipe->SetVertexShader("DebugTexture.hlsl");
		NewPipe->SetPixelShader("DebugTexture.hlsl");
	}

	// PostEffect
	{
		std::shared_ptr<GameEngineMaterial> NewPipe = GameEngineMaterial::Create("Blur");
		//NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		//NewPipe->SetInputAssembler2IndexBuffer("FullRect");
		NewPipe->SetVertexShader("Blur.hlsl");
		NewPipe->SetPixelShader("Blur.hlsl");
	}
}

void EngineMesh() 
{

	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.push_back({ float4(-0.5f, 0.5f)	, float4(0.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) }); // ���� ��
		Vertex.push_back({ float4(0.5f, 0.5f)	, float4(1.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) });  // ������ ����
		Vertex.push_back({ float4(0.5f, -0.5f)	, float4(1.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) }); // ������ �Ʒ���
		Vertex.push_back({ float4(-0.5f, -0.5f)	, float4(0.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) }); // ���� �Ʒ���
		GameEngineVertexBuffer::Create("Rect", Vertex);
	}


	{
		std::vector<int> Index;

		// ù��° �ﰢ��
		// ����Ʈ �����ڷ� ���ڸ� �ڿ� �߰����ִ� ��� �߰� �Լ�.
		Index.resize(6);

		// ù��°
		Index[0] = 0;
		Index[1] = 1;
		Index[2] = 2;

		// �ι�°
		Index[3] = 0;
		Index[4] = 2;
		Index[5] = 3;

		GameEngineIndexBuffer::Create("Rect", Index);
	}

	{
		GameEngineMesh::Create("rect");
	}

	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.push_back({ float4(-1.0f, 1.0f)	, float4(0.0f, 0.0f) }); // ���� ��
		Vertex.push_back({ float4(1.0f, 1.0f)	, float4(1.0f, 0.0f) });  // ������ ����
		Vertex.push_back({ float4(1.0f, -1.0f)	, float4(1.0f, 1.0f) }); // ������ �Ʒ���
		Vertex.push_back({ float4(-1.0f, -1.0f)	, float4(0.0f, 1.0f) }); // ���� �Ʒ���
		GameEngineVertexBuffer::Create("FullRect", Vertex);
	}


	{
		std::vector<int> Index;

		// ù��° �ﰢ��
		// ����Ʈ �����ڷ� ���ڸ� �ڿ� �߰����ִ� ��� �߰� �Լ�.
		Index.resize(6);

		// ù��°
		Index[0] = 0;
		Index[1] = 1;
		Index[2] = 2;

		// �ι�°
		Index[3] = 0;
		Index[4] = 2;
		Index[5] = 3;

		GameEngineIndexBuffer::Create("FullRect", Index);
	}

	{
		GameEngineMesh::Create("FullRect");
	}




	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.resize(24);
		// �ո�
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, 1.0f) };
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, 1.0f) };
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, 1.0f) };
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, 1.0f) };

		// �޸�
		Vertex[4] = {float4::VectorRotationToDegreeXAxis(Vertex[0].POSITION, 180.f), float4(0.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) };
		Vertex[5] = {float4::VectorRotationToDegreeXAxis(Vertex[1].POSITION, 180.f), float4(1.0f, 0.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) };
		Vertex[6] = {float4::VectorRotationToDegreeXAxis(Vertex[2].POSITION, 180.f), float4(1.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) };
		Vertex[7] = {float4::VectorRotationToDegreeXAxis(Vertex[3].POSITION, 180.f), float4(0.0f, 1.0f), float4(1.0f), float4(0.0f, 0.0f, -1.0f) };

		// ����
		Vertex[8] = {float4::VectorRotationToDegreeYAxis(Vertex[0].POSITION, -90.f)  , float4(0.0f, 0.0f), float4(1.0f), float4(1.0f, 0.0f, 0.0f) };
		Vertex[9] = {float4::VectorRotationToDegreeYAxis(Vertex[1].POSITION, -90.f)  , float4(1.0f, 0.0f), float4(1.0f), float4(1.0f, 0.0f, 0.0f) };
		Vertex[11] = { float4::VectorRotationToDegreeYAxis(Vertex[3].POSITION, -90.f), float4(1.0f, 1.0f), float4(1.0f), float4(1.0f, 0.0f, 0.0f) };
		Vertex[10] ={ float4::VectorRotationToDegreeYAxis(Vertex[2].POSITION, -90.f) , float4(0.0f, 1.0f), float4(1.0f), float4(1.0f, 0.0f, 0.0f) };

		// ������
		Vertex[12] = {float4::VectorRotationToDegreeYAxis(Vertex[0].POSITION, 90.f), float4(0.0f, 0.0f), float4(1.0f), float4(-1.0f, 0.0f, 0.0f) };
		Vertex[13] = {float4::VectorRotationToDegreeYAxis(Vertex[1].POSITION, 90.f), float4(1.0f, 0.0f), float4(1.0f), float4(-1.0f, 0.0f, 0.0f) };
		Vertex[14] = {float4::VectorRotationToDegreeYAxis(Vertex[2].POSITION, 90.f), float4(1.0f, 1.0f), float4(1.0f), float4(-1.0f, 0.0f, 0.0f) };
		Vertex[15] = {float4::VectorRotationToDegreeYAxis(Vertex[3].POSITION, 90.f), float4(0.0f, 1.0f), float4(1.0f), float4(-1.0f, 0.0f, 0.0f) };

		// ��
		Vertex[16] = {float4::VectorRotationToDegreeXAxis(Vertex[0].POSITION, -90.f) , float4(0.0f, 0.0f) , float4(1.0f), float4(0.0f, 1.0f, 0.0f) };
		Vertex[17] = {float4::VectorRotationToDegreeXAxis(Vertex[1].POSITION, -90.f) , float4(1.0f, 0.0f) , float4(1.0f), float4(0.0f, 1.0f, 0.0f) };
		Vertex[18] = {float4::VectorRotationToDegreeXAxis(Vertex[2].POSITION, -90.f) , float4(1.0f, 1.0f) , float4(1.0f), float4(0.0f, 1.0f, 0.0f) };
		Vertex[19] = { float4::VectorRotationToDegreeXAxis(Vertex[3].POSITION, -90.f), float4(0.0f, 1.0f) , float4(1.0f), float4(0.0f, 1.0f, 0.0f)  };

		// �Ʒ�
		Vertex[20] = {float4::VectorRotationToDegreeXAxis(Vertex[0].POSITION, 90.f) , float4(0.0f, 0.0f), float4(1.0f), float4(0.0f, -1.0f, 0.0f) };
		Vertex[21] = { float4::VectorRotationToDegreeXAxis(Vertex[1].POSITION, 90.f) , float4(1.0f, 0.0f), float4(1.0f), float4(0.0f, -1.0f, 0.0f) };
		Vertex[22] = { float4::VectorRotationToDegreeXAxis(Vertex[2].POSITION, 90.f) , float4(1.0f, 1.0f), float4(1.0f), float4(0.0f, -1.0f, 0.0f) };
		Vertex[23] = { float4::VectorRotationToDegreeXAxis(Vertex[3].POSITION, 90.f), float4(0.0f, 1.0f), float4(1.0f), float4(0.0f, -1.0f, 0.0f) };


		GameEngineVertexBuffer::Create("Box", Vertex);
	}

	{
		std::vector<int> Index;
		// Index.resize(36);
		for (int i = 0; i < 6; i++)
		{
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 1);
			Index.push_back(i * 4 + 0);

			Index.push_back(i * 4 + 3);
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 0);
		}

		GameEngineIndexBuffer::Create("Box", Index);
	}

	{
		GameEngineMesh::Create("box");
	}


	// Sphere
		// ���Ǿ�
	{
		GameEngineVertex V;
		std::vector<GameEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		// �ϱ������� �����մϴ�.
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 0.0f);
		// �븻 ���� Ȥ�� �������Ͷ�� �Ҹ���
		// �鿡 ������ ���͸� �ǹ��ϰ� �ȴ�.
		// ���� �ݻ��Ҷ� �ʼ�.
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 40; // ���� ���� �����Դϴ�.
		UINT iSliceCount = 40; // ���κ��� ����

		float yRotAngle = GameEngineMath::PI / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PI * 2) / (float)iSliceCount;

		// UV�� ���μ��� ���ݰ��� ���Ѵ�.
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// �� ���ݿ� ���� ������
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // ��ġ ũ�� ���� ������ �ֱ� ���ؼ�
				};

				// V.Pos *= GameEngineRandom::RandomFloat(-0.9f, 0.1f);

				V.TEXCOORD = float4(yUvRatio * z, zUvRatio * y);
				V.NORMAL = V.POSITION.Normalize3DReturn();
				V.NORMAL.w = 0.0f;

				V.TANGENT.x = -Radius * sinf(phi) * sinf(theta);
				V.TANGENT.y = 0.0f;
				V.TANGENT.z = Radius * sinf(phi) * cosf(theta);
				V.TANGENT = V.TANGENT.Normalize3DReturn();
				V.TANGENT.w = 0.0f;

				V.BINORMAL = float4::Cross3D(V.TANGENT, V.NORMAL);
				V.BINORMAL = V.BINORMAL.Normalize3DReturn();
				V.BINORMAL.w = 0.0f;

				VBVector.push_back(V);
			}
		}

		// ������
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(V);

		// �ε��� ���۸� ����ϴ�.
		IBVector.clear();

		// �ϱ����� �̷�� ���� ����°�.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
			IBVector.push_back(0);
			IBVector.push_back(i + 2);
			IBVector.push_back(i + 1);
		}

		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);

			}
		}

		// ���������� ������ �ε���
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
			IBVector.push_back(iBotIndex);
			IBVector.push_back(iBotIndex - (i + 2));
			IBVector.push_back(iBotIndex - (i + 1));
		}

		GameEngineVertexBuffer::Create("Sphere", VBVector);
		GameEngineIndexBuffer::Create("Sphere", IBVector);
		GameEngineMesh::Create("Sphere");

	}

	GameEngineFont::Load("����");
}

void GameEngineCore::EngineResourcesInitialize()
{
	// �簢�� �ڽ� ������ �ؽ�ó ���
	// �������ؿ��� �⺻������ ������� �Ѵٰ� �����ϴ�
	// ���ҽ����� �̴ϼȶ������ϴ� �ܰ�
	EngineTextureLoad();
	EngineInputLayOut();
	EngineMesh();
	EngineSubSetting();
	ShaderCompile();

	EngineRenderingPipeLine();

	// ���̴� �ε�

}


void GameEngineCore::EngineResourcesDestroy()
{
	GameEngineMaterial::ResourcesDestroy();

	GameEnginePixelShader::ResourcesDestroy();
	GameEngineVertexShader::ResourcesDestroy();

	GameEngineInputLayOut::ResourcesDestroy();
	GameEngineVertexBuffer::ResourcesDestroy();
	GameEngineIndexBuffer::ResourcesDestroy();
	GameEngineMesh::ResourcesDestroy();
	GameEngineFBXMesh::ResourcesDestroy();
	GameEngineFBXAnimation::ResourcesDestroy();

	GameEngineRenderTarget::ResourcesDestroy();
	GameEngineTexture::ResourcesDestroy();
	GameEngineDepthStencil::ResourcesDestroy();
	GameEngineFolderTexture::ResourcesDestroy();
	GameEngineSampler::ResourcesDestroy();
	GameEngineRasterizer::ResourcesDestroy();
	GameEngineBlend::ResourcesDestroy();
	GameEngineConstantBuffer::ResourcesDestroy();
	GameEngineStructuredBuffer::ResourcesDestroy();
	GameEngineInstancingBuffer::ResourcesDestroy();
	GameEngineSound::ResourcesDestroy();
	GameEngineFont::ResourcesDestroy();

	GameEngineDevice::Destroy();
}