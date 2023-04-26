#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <vector>
#include <DirectXTex.h>
#include <fbxsdk.h>

#include "Operator.h"

// DirectX::���ȗ�
using TexMetadata = DirectX::TexMetadata;
using ScratchImage = DirectX::ScratchImage;

struct Node
{
	// ���O
	std::string name;
	// ���[�J���X�P�[��
	XMVECTOR scaling = { 1, 1, 1, 0 };
	// ���[�J����]�p
	XMVECTOR rotation = { 0, 0, 0, 0 };
	// ���[�J���ړ�
	XMVECTOR translation = { 0, 0, 0, 1 };
	// ���[�J���ό`�s��
	XMMATRIX transform;
	// �O���[�o���ό`�s��
	XMMATRIX globalTransform;
	// �O���[�o���ό`�t�s��
	XMMATRIX inverseGlobalTransform;
	// �e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::���ȗ�
	template <class T> using vector = std::vector<T>;
	using string = std::string;
public:
	// �t�����h�N���X
	friend class FbxLoader;
public: // �萔
	static const int MAX_BONE_INDICES = 4;
public:// �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;	// xyz���W
		XMFLOAT3 normal;// �@���x�N�g��
		XMFLOAT2 uv;	// uv���W
		UINT boneIndex[MAX_BONE_INDICES]; // �{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES]; // �{�[�� �d��
	};
	struct Bone
	{
		// ���O
		std::string name;
		// �����p���̋t�s��
		XMMATRIX invInitialPose;
		// �N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		// �R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
private:
	// FBX�V�[��
	FbxScene* fbxScene = nullptr;
	// ���f����
	std::string name;
	// �m�[�h�z��
	std::vector<Node> nodes;
	// �{�[���z��
	std::vector<Bone> bones;
	// ���b�V�������m�[�h
	Node* meshNode = nullptr;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// �A���r�G���g�W��
	XMFLOAT3 ambient = { 1, 1, 1 };
	// �f�B�t���[�Y�W��
	XMFLOAT3 diffuse = { 1, 1, 1 };
	// �e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuffer;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuffer;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
public:
	//�f�X�g���N�^
	~FbxModel();
	void CreateBuffers(ID3D12Device* device);
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// getter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
	// ���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//���f���̕ό`�t�s��擾
	const XMMATRIX& GetInverseGlobalTransform()
	{
		meshNode->inverseGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshNode->globalTransform);
		return meshNode->inverseGlobalTransform;
	}
};