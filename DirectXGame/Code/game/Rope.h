#pragma once
#include "Object3d.h"
#include "Controller.h"
#include "Easing.h"
#include "Collision.h"
#include "Operator.h"
#include "SafeDelete.h"

class Rope
{
private:
	Rope();

	~Rope();

public:
	// �R�s�[�R���X�g���N�^�𖳌���
	Rope(const Rope& obj) = delete;
	// ������Z�q�𖳌���
	Rope& operator=(const Rope& obj) = delete;
public: // �����o�֐�

	static Rope* GetInstance();

	bool Initialize();

	void Update(XMFLOAT3& pPos);

	void Throw(XMFLOAT3& pPos, const XMFLOAT3 ePos, const float length);

	void Reset();

	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

	// �~�^��
	void CircularMotion(XMFLOAT3& pos, const XMFLOAT3 centerPos, const float r, int& angle, const int add);

	// ���[�v���Ȃ����Ă��邩
	const bool GetrFlag() { return rFlag; }
	void SetrFlag(bool rFlag) { this->rFlag = rFlag; }
	// ���[�v���΂��Ă��邩
	const bool GetThrowFlag() { return rThrowFlag; }
	void SetThrowFlag(bool throwFlag) { rThrowFlag = throwFlag; }
	
	const Object3d* GetObj() { return ropeObj; }

private: // �����o�ϐ�

	Controller* controller = Controller::GetInstance();
	Mouse* mouse = Mouse::GetInstance();

	//���[�v���f��
	Model* ropeModel = nullptr;
	Object3d* ropeObj = nullptr;

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
	// �^�[�Q�b�g���鋗���̊
	const float baseLength = 15.0f;

	XMFLOAT3 tPos = {};
	float tLength = 0.0f;

	// �ːi�p
	XMFLOAT3 startPos = {}; // �J�n�ʒu
	XMFLOAT3 endPos = {}; // �I�_�ʒu
	float avoidTime = 0.0f; // �o�ߎ���
	bool pEaseFlag = false;
	bool eEaseFlag = false;
};

