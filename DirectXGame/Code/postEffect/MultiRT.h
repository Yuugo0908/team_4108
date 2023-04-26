#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "Operator.h"

class MultiRT
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	HRESULT result;

public: // �T�u�N���X
// ���_�f�[�^�\����
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; // �F (RGBA)
		XMMATRIX mat; // �R�c�ϊ��s��
	};

public: // �ÓI�����o�֐�

private: // �ÓI�����o�ϐ�
	// ��ʃN���A�J���[
	static const float clearColor[4];
	// ���_��
	static const int vertNum = 4;
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
public: // �����o�֐�
	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C������
	/// </summary>
	/// <returns></returns>
	void CreateGraphicsPipeline();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// ���_�o�b�t�@����
	/// </summary>
	void CreateVB();

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateCB();

	/// <summary>
	/// �e�N�X�`������
	/// </summary>
	void CreateTex();

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�쐬
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[����
	/// </summary>
	void CreateRTV();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// �f�v�X�X�e���V���r���[����
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList"></param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	/// <param name="cmdList"></param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuffer[2];
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
};

