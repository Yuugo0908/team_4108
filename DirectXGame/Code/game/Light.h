#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include "Operator.h"

class Light
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightVec; // ���C�g�̃x�N�g��
		XMFLOAT3 lightColor; // ���C�g�̐F
	};
private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static bool StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static Light* Create();
private: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// ���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1, 0, 0, 0 };
	// ���C�g�F
	XMFLOAT3 lightColor = { 0.8f, 0.8f, 0.8f };
	// �_�[�e�B�t���O
	bool dirty = false;
public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// ������
	/// </summary>
	void TransferConstBuffer();
	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	/// <param name = "lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir);
	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name = "lightcolor">���C�g����</param>
	void SetLightColor(const XMFLOAT3& lightcolor);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
};

