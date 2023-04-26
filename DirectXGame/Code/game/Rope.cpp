#include "Rope.h"

bool Rope::Initialize()
{
	// ���f���̐���
	ropeModel = ropeModel->CreateFromObject("rope");
	ropeObj = Object3d::Create();
	ropeObj->SetModel(ropeModel);

	rFlag = false; // �ڐG�t���O
	rThrowFlag = false; // ���[�v���΂�
	rBackFlag = false; // ���[�v��߂�

	// �ʒu�A�X�P�[����ϐ��Ɋi�[
	ropeObj->SetScale({ 0.0f, 0.0f, 0.0f });
	rPos = ropeObj->GetPosition();
	rScale = ropeObj->GetScale();
	ropeObj->Update();

	return true;
}

void Rope::Update(XMFLOAT3& pPos)
{
	startPos = pPos;

	if (!rFlag)
	{
		rPos = startPos + manageRopePos;
		rScale = manageRopeScale;
		ropeObj->SetPosition(rPos);
		ropeObj->SetScale(rScale);

		if (!rThrowFlag && !rBackFlag && (controller->GetPadState(Controller::State::X, Controller::Type::TRIGGER) || mouse->TriggerMouseLeft()))
		{
			rThrowFlag = true;
			avoidTime = 0.0f;
		}
		else if(!rThrowFlag)
		{
			moveFlag = true;
		}

		ropeObj->Update();
		return;
	}

	objLength = GetLength(startPos, endPos);
	angleY = (float)atan2(startPos.x - endPos.x, startPos.z - endPos.z);
	vecXZ = sqrtf((startPos.x - endPos.x) * (startPos.x - endPos.x) + (startPos.z - endPos.z) * (startPos.z - endPos.z));
	angleX = (float)atan2(endPos.y - startPos.y, vecXZ);

	rPos = { (startPos.x + endPos.x) / 2, (startPos.y + endPos.y) / 2, (startPos.z + endPos.z) / 2 };
	rScale = { 0.2f, 0.2f , objLength / 2.0f };
	ropeObj->SetPosition(rPos);
	ropeObj->SetScale(rScale);
	ropeObj->SetRotation({ XMConvertToDegrees(angleX), XMConvertToDegrees(angleY), 0 });
	ropeObj->Update();
}

void Rope::Throw(XMFLOAT3& pPos, const XMFLOAT3 targetPos, const float targetLength)
{
	// �t���O��true����Ȃ��ꍇ�͏��������ă��^�[������
	if (!rThrowFlag && !rBackFlag)
	{
		manageRopePos = {};
		manageRopeScale = {};
		rRotFlag = false;
		cSpeed = Camera::GetInstance()->CameraTrack(pPos);
		cRot = Camera::GetInstance()->CameraRot(pPos);
		return;
	}
	else if (!rRotFlag && targetLength < 15.0f && tLength <= 0.0f)
	{
		tPos = targetPos;
		tLength = targetLength;
		rRotFlag = true;
		moveFlag = false;
	}

	XMVECTOR playerPos = { pPos.x, pPos.y, pPos.z, 1 };
	XMVECTOR enemyPos = { tPos.x, tPos.y, tPos.z, 1 };

	XMVECTOR subPlayerEnemy = XMVectorSubtract(enemyPos, playerPos);

	XMFLOAT3 subPE = { subPlayerEnemy.m128_f32[0], subPlayerEnemy.m128_f32[1], subPlayerEnemy.m128_f32[2] };

	if (rThrowFlag)
	{
		avoidTime += 0.1f;

		if (rRotFlag)
		{
			// Y������̊p�x
			angleY = (float)atan2(pPos.x - tPos.x, pPos.z - tPos.z);
			vecXZ = sqrtf((pPos.x - tPos.x) * (pPos.x - tPos.x) + (pPos.z - tPos.z) * (pPos.z - tPos.z));
			// X������̊p�x
			angleX = (float)atan2(tPos.y - pPos.y, vecXZ);
			rRot = { XMConvertToDegrees(angleX), XMConvertToDegrees(angleY), 0 };
			ropeObj->SetRotation(rRot);

			float timeThrowPos = avoidTime;
			manageRopePos = manageRopePos * (1.0f - timeThrowPos) + subPE * timeThrowPos;
		}

		manageRopeScale.x += 0.02f;
		manageRopeScale.y += 0.02f;

		float timeThrowScale = avoidTime;
		manageRopeScale.z = manageRopeScale.z * (1.0f - timeThrowScale) + subPE.z * timeThrowScale;

		if (avoidTime >= 1.0f)
		{
			avoidTime = 0.0f;
			rThrowFlag = false;
			rBackFlag = true;
		}
	}

	if (rBackFlag)
	{
		avoidTime += 0.1f;

		if (rRotFlag)
		{
			// Y������̊p�x
			angleY = (float)atan2(pPos.x - tPos.x, pPos.z - tPos.z);
			vecXZ = sqrtf((pPos.x - tPos.x) * (pPos.x - tPos.x) + (pPos.z - tPos.z) * (pPos.z - tPos.z));
			// X������̊p�x
			angleX = (float)atan2(tPos.y - pPos.y, vecXZ);
			ropeObj->SetRotation({ XMConvertToDegrees(angleX), XMConvertToDegrees(angleY), 0 });

			float timeBackPos = avoidTime;
			manageRopePos.x = -(manageRopePos.x * (1.0f - timeBackPos) + subPE.x * timeBackPos);
			manageRopePos.y = -(manageRopePos.y * (1.0f - timeBackPos) + subPE.y * timeBackPos);
			manageRopePos.z = -(manageRopePos.z * (1.0f - timeBackPos) + subPE.z * timeBackPos);
		}

		manageRopeScale.x -= 0.02f;
		manageRopeScale.y -= 0.02f;

		float timeBackScale = avoidTime;
		manageRopeScale.z = -(manageRopeScale.z * (1.0f - timeBackScale) + subPE.z * timeBackScale);

		if (avoidTime >= 1.0f)
		{
			avoidTime = 0.0f;
			rThrowFlag = false;
			rBackFlag = false;
			rRotFlag = false;
			manageRopePos = {};
			manageRopeScale = {};
			cSpeed = {};
			cRot = 0.0f;
			tPos = {};
			tLength = 0.0f;
		}
	}
}

bool Rope::Collision(const std::unique_ptr<Object3d>& object, XMFLOAT3 pPos)
{
	//���[�v���΂��Ă��Ȃ������ꍇ�͑����^�[������
	if (!rThrowFlag && !rBackFlag)
	{
		return false;
	}

	// ���C�̓����蔻��(������ɓG�������true����ȊO��false)
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 scale = object->GetCollisionScale();

	XMFLOAT3 lay = pPos;
	XMFLOAT3 vec = manageRopePos;
	XMFLOAT3 center = pos;
	float radius = scale.x;
	bool hitFlag = false;

	center = center - lay;

	float A = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	float B = vec.x * center.x + vec.y * center.y + vec.z * center.z;
	float C = center.x * center.x + center.y * center.y + center.z * center.z - radius * radius;

	if (A == 0.0f)
	{
		return false;
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false;
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false;
	}

	// ���̓����蔻��
	float l_x = rPos.x - pos.x;
	float l_y = rPos.y - pos.y;
	float l_z = rPos.z - pos.z;
	float r = (scale.z + manageRopeScale.z);

	rPos = ropeObj->GetPosition();
	rScale = ropeObj->GetScale();

	if ((l_x * l_x) + (l_y * l_y) + (l_z * l_z) <= (r * r))
	{
		endPos = pos;
		manageRopePos = {};
		manageRopeScale = {};
		avoidTime = 0.0f;
		rThrowFlag = false;
		rBackFlag = false;
		rRotFlag = false;
		tPos = {};
		tLength = 0.0f;
		rFlag = true;
		return true;
	}

	ropeObj->SetPosition(rPos);
	ropeObj->Update();

	return false;
}

void Rope::Reset()
{
	avoidTime = 0.0f;
	rThrowFlag = false;
	rBackFlag = false;

	// �ϐ�
	rPos = {};
	rScale = {};
	rRot = {};
	manageRopePos = {}; // ���[�v�ʒu�Ǘ��p
	manageRopeScale = { 0.0f, 0.0f, 0.0f }; // ���[�v�X�P�[���Ǘ��p
	angleX = 0.0f; // X��
	angleY = 0.0f; // Y��
	vecXZ = 0.0f; // XZ���ʏ�̃x�N�g��
	objLength = 0.0f;
	rFlag = false; // �ڐG�t���O

	// �ːi�p
	startPos = {}; // �J�n�ʒu
	endPos = {}; // �I�_�ʒu

	// �ړ��Ǘ��t���O
	moveFlag = false; // ���@���ړ����Ă��邩
}

void Rope::CircularMotion(XMFLOAT3& pos, const XMFLOAT3 centerPos, const float r, int& angle, const int add)
{
	angle += add;

	// �~�^���̏���
	pos.x = (cosf(3.14f / 180.0f * angle) * r) + centerPos.x;
	pos.y = (sinf(3.14f / 180.0f * angle) * r) + centerPos.y;
	pos.z = (tanf(3.14f / 180.0f * angle) * r) + centerPos.z;
}
