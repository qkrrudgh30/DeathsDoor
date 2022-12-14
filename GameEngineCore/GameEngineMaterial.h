#pragma once
#include "GameEngineRes.h"


// 설명 :
class GameEngineInputLayOut;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEngineIndexBuffer;
class GameEngineRasterizer;
class GameEnginePixelShader;
class GameEngineDepthStencil;
class GameEngineBlend;
class GameEngineInstancingBuffer;
class GameEngineMaterial : public GameEngineRes<GameEngineMaterial>
{
public:
	static void AllShaderReset();

	static std::shared_ptr < GameEngineMaterial> Create();

	static std::shared_ptr < GameEngineMaterial> Create(const std::string& _Name);

public:
	// constrcuter destructer
	GameEngineMaterial();
	~GameEngineMaterial();

	// delete Function
	GameEngineMaterial(const GameEngineMaterial& _Other) = delete;
	GameEngineMaterial(GameEngineMaterial&& _Other) noexcept = delete;
	GameEngineMaterial& operator=(const GameEngineMaterial& _Other) = delete;
	GameEngineMaterial& operator=(GameEngineMaterial&& _Other) noexcept = delete;

	// void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	// 범위를 지정하려면 면이 필요하지?
	// 면을 이루려면 점이 필요하죠?
	// 리소스는 점을 n개를 버퍼라는 개념으로 그래픽카드한테 쏴주는거다.

	// 이 점은 모니터 공간으로 가야 랜더링
	// WinApi때 그 좌표계로 가야 한다.


	// 버텍스 쉐이더
	// 그렇게 넣어준 점을 이동시켜야 내가 원하는 곳에 범위를 잡을수 있다.
	// 점 하나하나를 원하는 위치에 옮긴다.
	// 즉 점을 내마음대로 수치를 변경할수 있는 단계
	void SetVertexShader(const std::string& _Name);

	void SetVertexShader(std::shared_ptr<GameEngineVertexShader> _Shader);

	// 그리고 나서 이 점들을 진짜 모니터 위치로 옮겨주는 역할을 
	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	// 랜더러 자체가 들어올 가능성이 높네요.
	void Setting();

	void SettingInstancing();

	// void RenderingInstancing(int _RenderingCount, std::shared_ptr < GameEngineInstancingBuffer> _Buffer);

	//inline GameEngineVertexBuffer* GetVertexBuffer()
	//{
	//	return VertexBuffer;
	//}

	inline std::shared_ptr <GameEngineVertexShader> GetVertexShader()
	{
		return VertexShader;
	}

	inline std::shared_ptr < GameEnginePixelShader> GetPixelShader()
	{
		return PixelShader;
	}

	void Copy(std::shared_ptr<GameEngineMaterial> _Original);

protected:

	
private:
	//GameEngineInputLayOut* InputLayOut; // 인풋어셈블러1 세팅
	//GameEngineVertexBuffer* VertexBuffer; // 인풋어셈블러1 세팅
	//// HLSL이라는 다른 언어를 사용하는 단계에요.
	//// HLSL은 그래픽카드에 특정 데이터에 대한 처리 단계를 우리가 직접 작성해서
	//// 그래픽카드에 올려줄수 있는 겁니다.
	//GameEngineIndexBuffer* IndexBuffer; // 인풋어셈블러2 세팅
	//D3D11_PRIMITIVE_TOPOLOGY Topology;// 인풋어셈블러2 세팅

	// 정점을 이렇게 처리해.
	std::shared_ptr < GameEngineVertexShader> VertexShader; // 버텍스쉐이더 세팅

	std::shared_ptr<GameEngineRasterizer> Rasterizer; // 레스터라이저 세팅

	// 픽셀을 이렇게 처리해.
	std::shared_ptr < GameEnginePixelShader> PixelShader; // 픽셀쉐이더 세팅

	std::shared_ptr < GameEngineDepthStencil> DepthStencil; // 아웃풋머저 누가 앞인지 세팅 

	std::shared_ptr < GameEngineBlend> Blend; // 아웃풋머저 색상통합 세팅 



	// 아웃풋 머저중 랜더타겟세팅은 이미 해놨다.
	// 디바이스의 백버퍼를 사용해서 만든 랜터타겟이 매번 세팅되고 있다.
	// 거기에 그려라.

	// 내가 세팅해준다.

	// 아래쪽 함수들의 실행


	void InputAssembler1InstancingVertexBufferSetting(std::shared_ptr < GameEngineInstancingBuffer> _Buffer);

	void VertexShaderSetting();

	void InstancingVertexShaderSetting();
	

	void RasterizerSetting();

	void PixelShaderSetting();

	void OutputMergerBlendSetting();

	void OutputMergerDepthStencilSetting();

	// void Draw();

	void InstancingDraw(int _RenderingCount);

	void InstancingDataCollect();
};


