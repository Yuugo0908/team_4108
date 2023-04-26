#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <vector>
class Collision
{
public: // �T�u�N���X
	struct Sphere
	{
		// ���S���W
		XMVECTOR center = { 0, 0, 0 };
		// ���a
		float radius = 1.0f;
	};
	struct Plane
	{
		// �@���x�N�g��
		XMVECTOR normal = { 0, 0, 0 };
		// ���_����̋���
		float distance = 0.0f;
	};
	struct Ray
	{
		// �n�_���W
		XMVECTOR start = { 0, 0, 0 };
		// ����
		XMVECTOR dir = { 1, 0, 0 };
	};
	struct Triangle
	{
		// ���_���W
		XMVECTOR p0;
		XMVECTOR p1;
		XMVECTOR p2;
		// �@���x�N�g��
		XMVECTOR normal;
	};

public: // �ÓI�����o�֐�

	//�I�u�W�F�N�g���m�̓����蔻��
	static bool CollisionObject(const std::unique_ptr<Object3d>& object_a, const std::unique_ptr<Object3d>& object_b);
	// ���Ƌ��̓����蔻��
	static bool CollisionSphere(const Sphere& sphere1, const Sphere& sphere2);
	// ���ƕ��ʂ̓����蔻��
	static bool CollisionSpherePlane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter);

	// ���C�ƕ��ʂ̓����蔻��
	static bool CollisionRayPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, XMVECTOR* inter = nullptr);
	// ���C�Ƌ��̓����蔻��
	static bool CollisionRaySphere(const Ray& ray, const Sphere& sphere);
	// ���C��BOX�̓����蔻��
	static bool CollisionRayBox(const XMFLOAT3 startPos, const XMFLOAT3 endPos, const XMFLOAT3 boxPos, const XMFLOAT3 boxScale);

	// BOX�Ɠ_�̓����蔻��
	static bool CollisionBoxPoint(const XMFLOAT3 boxPos, const XMFLOAT3 boxRadius, XMFLOAT3& pos, const XMFLOAT3 radius, XMFLOAT3 oldPos);
	// �n�ʂƂ̓����蔻��
	static bool CollisionStage(const XMFLOAT3 stagePos, const XMFLOAT3 stageRadius, XMFLOAT3& pos, const XMFLOAT3 radius, XMFLOAT3 oldPos);
	// �I�u�W�F�N�g���m�̋������擾
	static float GetLength(XMFLOAT3 pos_a, XMFLOAT3 pos_b);
};

