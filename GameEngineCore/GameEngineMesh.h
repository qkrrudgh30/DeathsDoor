#pragma once
#include <string.h>
#include "GameEngineRes.h"
#include "GameEngineInstancingBuffer.h"

// ���� :
class GameEngineInputLayOut;
class GameEngineVertexBuffer;
class GameEngineIndexBuffer;
class GameEngineMesh : public GameEngineRes<GameEngineMesh>
{
public:
	static std::shared_ptr < GameEngineMesh> Create(const std::string& _Name);

	static std::shared_ptr < GameEngineMesh> Create(const std::string& _Name, const std::string& _Vtx, const std::string& _Idx);

	static std::shared_ptr < GameEngineMesh> Create(std::shared_ptr < GameEngineVertexBuffer> _Vtx, std::shared_ptr < GameEngineIndexBuffer> _Idx);

public:
	// constrcuter destructer
	GameEngineMesh();
	~GameEngineMesh();

	// delete Function
	GameEngineMesh(const GameEngineMesh& _Other) = delete;
	GameEngineMesh(GameEngineMesh&& _Other) noexcept = delete;
	GameEngineMesh& operator=(const GameEngineMesh& _Other) = delete;
	GameEngineMesh& operator=(GameEngineMesh&& _Other) noexcept = delete;

	// ���� ������ �˷��ִ� ��ǲ���̾ƿ��� �� �ܰ迡�� ���õȴ�.
	void SetInputAssembler1VertexBuffer(const std::string& _Name);

	// �׷��� �ؼ� ��ȯ�� ���� ��� �����ؼ� ������ ������̳ĸ� ���⼭ ó���մϴ�.
	// �ε��� ���۰� ���õȴ�.
	void SetInputAssembler2IndexBuffer(const std::string& _Name);

	void SetInputAssembler1VertexBuffer(std::shared_ptr < GameEngineVertexBuffer> _Res);
	void SetInputAssembler2IndexBuffer(std::shared_ptr < GameEngineIndexBuffer> _Res);

	void InputAssembler1VertexBufferSetting();

	void InputAssembler2IndexBufferSetting();

	void Setting();

	void SettingInstancing(std::shared_ptr < GameEngineInstancingBuffer> _Buffer);

	void Render();

	void RenderInstancing(size_t _InstancingCount);

	const class GameEngineLayOutDesc& GetLayOutDesc();

protected:

private:
	std::shared_ptr < GameEngineVertexBuffer> VertexBuffer; // ��ǲ�����1 ����
	// HLSL�̶�� �ٸ� �� ����ϴ� �ܰ迡��.
	// HLSL�� �׷���ī�忡 Ư�� �����Ϳ� ���� ó�� �ܰ踦 �츮�� ���� �ۼ��ؼ�
	// �׷���ī�忡 �÷��ټ� �ִ� �̴ϴ�.
	std::shared_ptr < GameEngineIndexBuffer> IndexBuffer; // ��ǲ�����2 ����

};

