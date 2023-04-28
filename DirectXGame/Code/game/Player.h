#pragma once
#include "Object3d.h"
#include "Keyboard.h"
#include "Controller.h"
#include "Collision.h"
#include "Easing.h"
#include "Operator.h"
#include "Camera.h"
#include "Particle.h"
#include "DirectXCommon.h"

class Player
{
public: // �����o�֐�
	//����������
	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);
	// �X�V����
	void Update();
	// �I�u�W�F�N�g
	const std::unique_ptr<Object3d>& GetObj() { return playerObj; }
	
	//Geter
	bool& GetOnGround() { return onGround; }
	float& GetmoveY() { return moveY; }

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void MoveProcess();
	/// <summary>
	/// �W�����v����
	/// </summary>
	void JumpProcess();
	/// <summary>
	/// �d�͏���
	/// </summary>
	void GravityProcess();
	/// <summary>
	/// �n�ʏՓ˔��菈��
	/// </summary>
	void GroundCollisionProcess();

public:
	/// <summary>
	/// �n�ʂɐڂ����u�Ԃ�
	/// </summary>
	/// <returns>����</returns>
	bool GetOnGrounding()
	{
		return onGround == true && onGround != oldOnGround;
	}

private: // �����o�ϐ�

	Keyboard* keyboard = Keyboard::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	Camera* camera = Camera::GetInstance();

	// ���f��
	Model* playerModel = nullptr;
	std::unique_ptr<Object3d> playerObj = nullptr;

	// �v���C���[
	XMFLOAT3 pPos = {};//���W
	XMFLOAT3 pPosOld = {};
	XMFLOAT3 pScale = {};//�傫��
	XMFLOAT3 pRot = {};//��]
	XMFLOAT3 reSpawnPos = {};
	XMFLOAT3 move = {}; //�ړ���
	float moveY = 0.0f; //�W�����v�y�яd��
	bool onGround = false;
	bool oldOnGround = false;
};

