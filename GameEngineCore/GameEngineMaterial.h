#pragma once
#include "GameEngineRes.h"


// ���� :
class GameEngineInputLayOut;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEngineIndexBuffer;
class GameEngineRasterizer;
class GameEnginePixelShader;
class GameEngineDepthStencil;
class GameEngineBlend;
class GameEngineMaterial : public GameEngineRes<GameEngineMaterial>
{
public:
	static void AllShaderReset();

	static GameEngineMaterial* Create();

	static GameEngineMaterial* Create(const std::string& _Name);

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

	// ������ �����Ϸ��� ���� �ʿ�����?
	// ���� �̷���� ���� �ʿ�����?
	// ���ҽ��� ���� n���� ���۶�� �������� �׷���ī������ ���ִ°Ŵ�.

	// �� ���� ����� �������� ���� ������
	// WinApi�� �� ��ǥ��� ���� �Ѵ�.


	// ���ؽ� ���̴�
	// �׷��� �־��� ���� �̵����Ѿ� ���� ���ϴ� ���� ������ ������ �ִ�.
	// �� �ϳ��ϳ��� ���ϴ� ��ġ�� �ű��.
	// �� ���� ��������� ��ġ�� �����Ҽ� �ִ� �ܰ�
	void SetVertexShader(const std::string& _Name);

	void SetVertexShader(GameEngineVertexShader* _Shader);

	// �׸��� ���� �� ������ ��¥ ����� ��ġ�� �Ű��ִ� ������ 
	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	// ������ ��ü�� ���� ���ɼ��� ���׿�.
	void Setting();

	void RenderingInstancing(int _RenderingCount, class GameEngineInstancingBuffer* _Buffer);

	//inline GameEngineVertexBuffer* GetVertexBuffer()
	//{
	//	return VertexBuffer;
	//}

	inline GameEngineVertexShader* GetVertexShader() 
	{
		return VertexShader;
	}

	inline GameEnginePixelShader* GetPixelShader()
	{
		return PixelShader;
	}

	void Copy(GameEngineMaterial* _Original);

protected:

	
private:
	//GameEngineInputLayOut* InputLayOut; // ��ǲ�����1 ����
	//GameEngineVertexBuffer* VertexBuffer; // ��ǲ�����1 ����
	//// HLSL�̶�� �ٸ� �� ����ϴ� �ܰ迡��.
	//// HLSL�� �׷���ī�忡 Ư�� �����Ϳ� ���� ó�� �ܰ踦 �츮�� ���� �ۼ��ؼ�
	//// �׷���ī�忡 �÷��ټ� �ִ� �̴ϴ�.
	//GameEngineIndexBuffer* IndexBuffer; // ��ǲ�����2 ����
	//D3D11_PRIMITIVE_TOPOLOGY Topology;// ��ǲ�����2 ����

	// ������ �̷��� ó����.
	GameEngineVertexShader* VertexShader; // ���ؽ����̴� ����

	GameEngineRasterizer* Rasterizer; // �����Ͷ����� ����

	// �ȼ��� �̷��� ó����.
	GameEnginePixelShader* PixelShader; // �ȼ����̴� ����

	GameEngineDepthStencil* DepthStencil; // �ƿ�ǲ���� ���� ������ ���� 

	GameEngineBlend* Blend; // �ƿ�ǲ���� �������� ���� 



	// �ƿ�ǲ ������ ����Ÿ�ټ����� �̹� �س���.
	// ����̽��� ����۸� ����ؼ� ���� ����Ÿ���� �Ź� ���õǰ� �ִ�.
	// �ű⿡ �׷���.

	// ���� �������ش�.

	// �Ʒ��� �Լ����� ����


	void InputAssembler1InstancingVertexBufferSetting(class GameEngineInstancingBuffer* _Buffer);

	void VertexShaderSetting();


	void RasterizerSetting();

	void PixelShaderSetting();

	void OutputMergerBlendSetting();

	void OutputMergerDepthStencilSetting();

	// void Draw();

	void InstancingDraw(int _RenderingCount);

	void InstancingDataCollect();
};


