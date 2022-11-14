#include "PreCompile.h"
#include "GameEngineFBXAnimationRenderer.h"
#include "GameEngineMaterial.h"
#include "GameEngineVertexShader.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXAnimationRenderer::GameEngineFBXAnimationRenderer() 
{
	
}

GameEngineFBXAnimationRenderer::~GameEngineFBXAnimationRenderer() 
{
	Animations.clear();
}


// _Index
// �ִϸ��̼��� ���� fbx�� AnimationDatas(vector)�� index�� �ǹ��Ѵ�.

void FBXRendererAnimation::Init(int _Index)
{
	// GameENgineFBXAnimation�� ��� ������ ���������°��� 
	// CalFbxExBoneFrameTransMatrix�� ȣ��ǰ� �� ���Դϴ�.
	// �ִϸ��̼��� ����� ���Ǵ°̴ϴ�.
	Aniamtion->AnimationMatrixLoad(Mesh, ParentRenderer->CastThis<GameEngineFBXAnimationRenderer>(), _Index);
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = 0;
	End = static_cast<unsigned int>(FBXAnimationData->TimeEndCount);
	FrameTime = 0.02f;

}

void FBXRendererAnimation::Update(float _DeltaTime)
{
	// 0~24��������?
	CurFrameTime += _DeltaTime;
	//                      0.1
	// 1
	while (CurFrameTime >= FrameTime)
	{
		// ������ �ð��� ���Ե���?
		// ������ �ð��� �߿��մϴ�.
		CurFrameTime -= FrameTime;
		++CurFrame;

		if (CurFrame >= End)
		{
			CurFrame = Start;
		}
	}

	unsigned int NextFrame = CurFrame;

	++NextFrame;

	if (NextFrame >= End)
	{
		NextFrame = 0;
	}

	// mesh      subset
	std::vector<std::vector<GameEngineRenderUnit>>& Units = ParentRenderer->GetAllRenderUnit();

	for (size_t UnitSetIndex = 0; UnitSetIndex < Units.size(); ++UnitSetIndex)
	{
		for (size_t RenderUnitIndex = 0; RenderUnitIndex < Units[UnitSetIndex].size(); ++RenderUnitIndex)
		{
			GameEngineRenderUnit& Render = Units[UnitSetIndex][RenderUnitIndex];

			// ����!!!! ����!!!! ���� ����� ����������������������Ӥ���
			std::map<size_t, std::vector<float4x4>>::iterator MatrixIter = ParentRenderer->AnimationBoneMatrixs.find(UnitSetIndex);

			if (MatrixIter == ParentRenderer->AnimationBoneMatrixs.end())
			{
				continue;
			}

			// 68�� 
			std::vector<float4x4>& AnimationBoneMatrix = MatrixIter->second;

			std::map<size_t, std::vector<AnimationBoneData>>::iterator AnimationDataIter = ParentRenderer->AnimationBoneDatas.find(UnitSetIndex);
			// 68�� 
			std::vector<AnimationBoneData>& AnimationBoneData = AnimationDataIter->second;

			size_t MeshIndex = MatrixIter->first;

			for (int i = 0; i < AnimationBoneMatrix.size(); i++)
			{
				Bone* BoneData = ParentRenderer->GetFBXMesh()->FindBone(MeshIndex, i);

				if (true == FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData.empty())
				{
					// ���� �����ӵ����Ͱ� �������� �ֽ��ϴ�.
					// ���� �׵������ �� �ִ� ��찡 �����ϴ�.
					// �⺻��ķ� �������ִ°�.
					// GameEngineDebug::MsgBoxError("������� ������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
					AnimationBoneMatrix[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}

				// ���������Ӱ� 
				FbxExBoneFrameData& CurData = FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData[CurFrame];
				// ������������ ������ �ʿ��ѵ�
				FbxExBoneFrameData& NextData = FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData[NextFrame];

				// �ִϸ��̼��� �ٲ�� ���� �ѹ��� �����ؾ� �Ѵ�.
				AnimationBoneData[i].Scale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
				// ���� ���ʹϿ�
				AnimationBoneData[i].RotQuaternion = float4::SLerpQuaternion(CurData.Q, NextData.Q, CurFrameTime);
				// ���� ������
				AnimationBoneData[i].Pos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);
				// ���Ӱ� �ٲ� �ִϸ��̼�

				// �ִϸ��̼��� �ٲ�� ���� �ѹ��� �����ؾ� �Ѵ�.
				//PrevAnimation[i].Scale;
				//PrevAnimation[i].RotQuaternion;
				//PrevAnimation[i].Pos;

				//AnimationBoneData[i].Scale;
				//AnimationBoneData[i].RotQuaternion;
				//AnimationBoneData[i].Pos;

				// float4::Lerp(PrevAnimation[i].Scale, AnimationBoneData[i].Scale, 0.0);

				size_t Size = sizeof(float4x4);
				// �ڽ��� ������İ� �����ش� ū �ǹ̴� ����.

				float4x4 Mat = float4x4::Affine(AnimationBoneData[i].Scale, AnimationBoneData[i].RotQuaternion, AnimationBoneData[i].Pos);

				//AnimationBoneData[i].Index = i;
				//AnimationBoneData[i].Scale *= ParentRenderer->GetTransform()->GetTransformData().WorldWorld_;
				//AnimationBoneData[i].RotQ = ParentRenderer->BoneData[i].RotQ.QuaternionMultiplyfloat4x4(ParentRenderer->GetTransform()->GetTransformData().WorldWorld_);
				//AnimationBoneData[i].Pos *= ParentRenderer->GetTransform()->GetTransformData().WorldWorld_;
				//AnimationBoneData[i].RotEuler = float4::QuaternionToEulerAngles(ParentRenderer->BoneData[i].RotQ) + ParentRenderer->GetTransform()->GetWorldRotation();
				//AnimationBoneData[i].RotEuler *= GameEngineMath::RadianToDegree;

				AnimationBoneMatrix[i] = BoneData->BonePos.Offset * Mat;
				//AnimationBoneMatrix[i].Transpose();
			}
		}

	}
}

void FBXRendererAnimation::Reset()
{
	Start = 0;
}

void GameEngineFBXAnimationRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return;
	}

	if (false == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("FBX �ִϸ��̼� �������� �ִϸ��̼��� �Ұ����� ��Ƽ������ �����߽��ϴ�.");
		return;
	}


	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
}

GameEngineRenderUnit* GameEngineFBXAnimationRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t _MeshIndex, size_t _SubSetIndex /*= 0*/)
{
	std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Find(_Material);

	if (nullptr == Mat)
	{
		MsgBoxAssert("�������� �ʴ� ��Ƽ���� �Դϴ�.");
		return nullptr;
	}

	if (false == Mat->GetVertexShader()->IsStructuredBuffer("ArrAniMationMatrix"))
	{
		MsgBoxAssert("FBX �ִϸ��̼� �������� �ִϸ��̼��� �Ұ����� ��Ƽ������ �����߽��ϴ�.");
		return nullptr;
	}

	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgBoxAssert("�������� �ʴ� FBXMesh�� �����߽��ϴ�.");
		return nullptr;
	}

	if (AnimationBoneMatrixs.end() == AnimationBoneMatrixs.find(_MeshIndex))
	{
		size_t BoneCount = FindFBXMesh->GetBoneCount(_MeshIndex);
		AnimationBoneMatrixs.insert(std::make_pair(_MeshIndex, std::vector<float4x4>()));
		AnimationBoneMatrixs[_MeshIndex].resize(BoneCount);

		AnimationBoneDatas.insert(std::make_pair(_MeshIndex, std::vector<AnimationBoneData>()));
		AnimationBoneDatas[_MeshIndex].resize(BoneCount);
	}

	FindFBXMesh->GetMeshInfosCount();

	// �ؽ�ó ������ �θ���� �þƼ� ó�����ְ�
	GameEngineRenderUnit* Unit = GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, _MeshIndex, _SubSetIndex);

	if (nullptr == Unit)
	{
		MsgBoxAssert("FBX�ִϸ��̼� ��������Ʈ�� ������ ���߽��ϴ�.");
		return nullptr;
	}

	// �̶� ��Ʈ��ó�� ���۸� �����Ұų�.
	if (Unit->ShaderResources.IsStructuredBuffer("ArrAniMationMatrix"))
	{
		GameEngineStructuredBufferSetter* AnimationBuffer = Unit->ShaderResources.GetStructuredBuffer("ArrAniMationMatrix");

		// ?? _MeshIndex
		AnimationBuffer->Res = GetFBXMesh()->GetAnimationStructuredBuffer(_MeshIndex);

		if (nullptr == AnimationBuffer->Res)
		{
			MsgBoxAssert("�ִϸ��̼ǿ� ��Ʈ��ó�� ���۰� �������� �ʽ��ϴ�.");
			return Unit;
		}

		if (0 == AnimationBoneMatrixs[_MeshIndex].size())
		{
			return Unit;
		}

		// ��ũ�� �ɾ��ذ�.
		AnimationBuffer->SetData = &AnimationBoneMatrixs[_MeshIndex][0];
		AnimationBuffer->Size = AnimationBoneMatrixs[_MeshIndex].size() * sizeof(float4x4);
		AnimationBuffer->Bind();

	}

	return Unit;
}

void GameEngineFBXAnimationRenderer::CreateFBXAnimation(const std::string& _AnimationName
	, const std::string& _AnimationFBX, int _Index)
{
	// ���� ���� fbx�� ���õǾ� �ִ��� �˻��Ѵ�.
	if (nullptr == GetFBXMesh())
	{
		MsgBoxAssert("��� FBX�� ���õǾ� ���� �ʽ��ϴ�.");
		return;
	}

	// �����ִ� �Լ��� �ܺο��� ȣ���ϸ� �˴ϴ�.

	if (Animations.end() != Animations.find(_AnimationName))
	{
		MsgBoxAssertString("�̹� �����ϴ� �̸��� �ִϸ��̼��Դϴ�." + _AnimationName);
		return;
	}

	std::shared_ptr<GameEngineFBXAnimation> Animation = GameEngineFBXAnimation::Find(_AnimationFBX);

	if (nullptr == Animation)
	{
		MsgBoxAssertString("GameEngineFBXAnimation�� �������� �ʽ��ϴ�. " + _AnimationFBX);
		return;
	}

	std::shared_ptr<FBXRendererAnimation> NewAnimation = std::make_shared<FBXRendererAnimation>();

	NewAnimation->Mesh = GetFBXMesh();
	NewAnimation->Aniamtion = Animation;
	NewAnimation->ParentRenderer = this;

	NewAnimation->Init(_Index);

	// �̼��� �ִϸ��̼� ������ ��Ŀ� ���� ����� �̶� �̷����µ�.
	// �̰� �������̱� ������ �Ƹ� ���� �и���.
	// �ٸ� ���̳� �Լ��� �ѹ� �ε��ϰ� �츮 �������� �����ϴ� ���� �ؾ��Ұ̴ϴ�.

	RenderOptionInst.IsAnimation = 1;

	Animations.insert(std::make_pair(_AnimationName, NewAnimation));

	Animation;
}

void GameEngineFBXAnimationRenderer::ChangeAnimation(const std::string& _AnimationName)
{
	std::map<std::string, std::shared_ptr<FBXRendererAnimation>>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	CurAnimation = FindIter->second;
}

void GameEngineFBXAnimationRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation)
	{
		return;
	}

	CurAnimation->Update(_DeltaTime);
}