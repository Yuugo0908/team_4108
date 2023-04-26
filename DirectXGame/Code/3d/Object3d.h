#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Operator.h"

class Object3d
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W(���[���h���W)
		float a;
		XMFLOAT4 color;
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static bool StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	// �`��㏈��
	static void PostDraw();
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<Object3d> Create();
	// �O���t�B�b�N�p�C�v���C������
	static bool CreateGraphicsPipeline();
	// ���C�g�̃Z�b�g
	static void SetLight(Light* light)
	{
		Object3d::light = light;
	}
	// �J�����̃Z�b�g
	static void SetCamera(Camera* camera)
	{
		Object3d::camera = camera;
	}

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;	
	// ���_�V�F�[�_�I�u�W�F�N�g
	static ComPtr<ID3DBlob> vsBlob;
	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	static ComPtr<ID3DBlob> psBlob;
	// �G���[�I�u�W�F�N�g
	static ComPtr<ID3DBlob> errorBlob;
	// ���C�g
	static Light* light;
	// �J����
	static Camera* camera;
public: // �����o�֐�
	bool Initialize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	// ���f���X�P�[���̎擾
	const XMFLOAT3& GetScale() { return scale; }
	// ���f���X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	// ��]�p�̎擾
	const XMFLOAT3& GetRotation() { return rotation; }
	// ��]�p�̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	// ���f���̐ݒ�
	void SetModel(Model* model) { this->model = model; }
	// �F�̐ݒ�
	void SetColor(XMFLOAT4 color) { this->color = color; }
	// �I�u�W�F�N�g�̃^�C�v�ݒ�
	void SetType(int type) { this->type = type; }
	// �I�u�W�F�N�g�^�C�v�̎擾
	const int& GetType() { return type; }
	// �`��t���O�̐ݒ�
	void SetDrawFlag(bool dFlag) { drawFlag = dFlag; }
	// �`��t���O�̎擾
	const bool& GetDrawFlag() { return drawFlag; }

	// ���x���G�f�B�^�p �����蔻��X�P�[���ݒ�
	void SetCollisionScale(XMFLOAT3 collisionScale) { this->collisionScale = collisionScale; }
	// ���x���G�f�B�^�p �����蔻��X�P�[���擾
	const XMFLOAT3& GetCollisionScale() { return collisionScale; }

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBufferB0; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	Object3d* parent = nullptr;
	Model* model = nullptr;
	// �I�u�W�F�N�g�^�C�v
	int type = 0;
	// �R���W�����X�P�[��
	XMFLOAT3 collisionScale = {};
	// �`��t���O
	bool drawFlag = true;
};