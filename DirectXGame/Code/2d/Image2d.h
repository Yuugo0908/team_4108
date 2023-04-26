#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "Operator.h"

class Image2d
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	// �ÓI������
	static bool StaticInitialize(ID3D12Device* device);
	// �e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);
	// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	// �`��㏈��
	static void PostDraw();
	// �摜����
	static Image2d* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);
	// �O���t�B�b�N�p�C�v���C������
	static bool CreateGraphicsPipeline();

protected: // �ÓI�����o�ϐ�
	// �e�N�X�`���̍ő喇��
	static const int srvCount = 512;
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
	// �ˉe�s��
	static XMMATRIX matProjection;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuffer[srvCount];

public: // �����o�֐�
	// �R���X�g���N�^
	Image2d(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	// ������
	bool Initialize();
	// �p�x�̐ݒ�
	void SetRotation(float rotation);
	// ���W�̐ݒ�
	void SetPosition(XMFLOAT2 position);
	// �T�C�Y�̐ݒ�
	void SetSize(XMFLOAT2 size);
	// �F�̐ݒ�
	void SetColor(XMFLOAT4 color);
	// �A���J�[�|�C���g�̐ݒ�
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	// ���E���]�̐ݒ�
	void SetIsFlipX(bool isFlipX);
	// �㉺���]�̐ݒ�
	void SetIsFlipY(bool isFlipY);
	// �e�N�X�`���͈͐ݒ�
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	// �`��
	void Draw();
	// ���_�f�[�^�]��
	void TransferVertices();

protected: // �����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position{};
	// �摜���A����
	XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	XMFLOAT2 texBase = { 0, 0 };
	// �e�N�X�`�����A����
	XMFLOAT2 texSize = { 100.0f, 100.0f };
};