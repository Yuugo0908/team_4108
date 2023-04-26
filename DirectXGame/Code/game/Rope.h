#pragma once
#include "Object3d.h"
#include "Controller.h"
#include "Easing.h"
#include "Collision.h"
#include "Operator.h"

class Rope
{
public: // �����o�֐�

	bool Initialize();

	void Update(XMFLOAT3& pPos);

	void Throw(XMFLOAT3& pPos, const XMFLOAT3 ePos, const float length);

	bool Collision(const std::unique_ptr<Object3d>& object, XMFLOAT3 pPos);

	void Reset();

	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

	// �~�^��
	void CircularMotion(XMFLOAT3& pos, const XMFLOAT3 centerPos, const float r, int& angle, const int add);

	// �ړ����Ă��邩
	const bool& GetmoveFlag() { return moveFlag; }
	void SetmoveFlag(bool moveFlag) { this->moveFlag = moveFlag; }
	// ���[�v���Ȃ����Ă��邩
	const bool& GetrFlag() { return rFlag; }
	void SetrFlag(bool rFlag) { this->rFlag = rFlag; }
	// ���[�v���΂��Ă��邩
	const bool& GetThrowFlag() { return rThrowFlag; }
	void SetThrowFlag(bool throwFlag) { rThrowFlag = throwFlag; }
	
	const std::unique_ptr<Object3d>& GetObj() { return ropeObj; }

private: // �����o�ϐ�

	Controller* controller = Controller::GetInstance();
	Mouse* mouse = Mouse::GetInstance();

	//���[�v���f��
	Model* ropeModel = nullptr;
	std::unique_ptr<Object3d> ropeObj = nullptr;

	// �ϐ�
	XMFLOAT3 rPos = {};
	XMFLOAT3 rScale = {};
	XMFLOAT3 rRot = {};
	XMFLOAT3 manageRopePos = {}; // ���[�v�ʒu�Ǘ��p
	XMFLOAT3 manageRopeScale = { 0.0f, 0.0f, 0.0f }; // ���[�v�X�P�[���Ǘ��p
	const float maxRope = 15.0f; // ���[�v�̍ő�
	float angleX = 0.0f; // X��
	float angleY = 0.0f; // Y��
	float vecXZ = 0.0f; // XZ���ʏ�̃x�N�g��
	float objLength = 0.0f;
	bool rFlag = false; // �ڐG�t���O
	bool rThrowFlag = false; // ���[�v���΂�
	bool rBackFlag = false; // ���[�v��߂�
	bool rRotFlag = false;

	XMFLOAT3 cSpeed = {};
	float cRot = 0.0f;

	XMFLOAT3 tPos = {};
	float tLength = 0.0f;

	// �ːi�p
	XMFLOAT3 startPos = {}; // �J�n�ʒu
	XMFLOAT3 endPos = {}; // �I�_�ʒu
	float avoidTime = 0.0f; // �o�ߎ���
	bool pEaseFlag = false;
	bool eEaseFlag = false;

	// �ړ��Ǘ��t���O
	bool moveFlag = false; // ���@���ړ����Ă��邩
};

