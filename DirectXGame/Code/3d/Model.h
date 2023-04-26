#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "Operator.h"

class Model
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//�}�e���A��
	struct Material
	{
		std::string name; //�}�e���A����
		XMFLOAT3 ambient; //�A���r�G���g�e���x
		XMFLOAT3 diffuse; //�f�B�t���[�Y�e���x
		XMFLOAT3 specular; //�X�y�L�����[�e���x
		float alpha; //�A���t�@
		std::string textureFilename; //�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material()
		{
			ambient =	{ 0.0f, 0.0f, 0.0f };
			diffuse =	{ 0.0f, 0.0f, 0.0f };
			specular =	{ 1.0f, 1.0f, 1.0f };
			alpha =		1.0f;
		}
	};

	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //�A���r�G���g�W��
		float pad1; //�p�f�B���O
		XMFLOAT3 diffuse; //�f�B�t���[�Y�W��
		float pad2; //�p�f�B���O
		XMFLOAT3 specular; //�X�y�L�����[�W��
		float alpha; //�A���t�@
	};

private: // �萔

public: // �ÓI�����o�֐�
	// �ÓI������
	static bool StaticInitialize(ID3D12Device* device);

	Model* CreateFromObject(const std::string& text);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuffer;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuffer;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// �}�e���A��
	Material material;

private:// �ÓI�����o�֐�
	// �f�X�N���v�^�q�[�v�̏�����
	bool InitializeDescriptorHeap();
	// �e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	// ���f���쐬
	void CreateModel(const std::string& text);
	//�}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

public: // �����o�֐�
	bool Initialize(const std::string& text);
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBufferB1; // �萔�o�b�t�@
};