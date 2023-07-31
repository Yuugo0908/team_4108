#include "Rope.h"

Rope::Rope()
{
}

Rope::~Rope()
{
	safe_delete(ropeModel);
}

Rope* Rope::GetInstance()
{
	static Rope instance;

	return &instance;
}

bool Rope::Initialize()
{
	// モデルの生成
	ropeModel = ropeModel->CreateFromObject("rope");
	ropeObj = Object3d::Create();
	ropeObj->SetModel(ropeModel);

	rFlag = false; // 接触フラグ
	rThrowFlag = false; // ロープを飛ばす

	// 位置、スケールを変数に格納
	ropeObj->SetScale({ 0.0f, 0.0f, 0.0f });
	rPos = ropeObj->GetPosition();
	rScale = ropeObj->GetScale();
	ropeObj->Update();

	return true;
}

void Rope::Update(XMFLOAT3& pPos)
{
	startPos = pPos;

	// ロープが接着していなければ、ロープはプレイヤーと同じ座標に置き、発射を待つ
	if (!rFlag)
	{
		rPos = startPos + manageRopePos;
		rScale = manageRopeScale;
		ropeObj->SetPosition(rPos);
		ropeObj->SetScale(rScale);

		if (!rThrowFlag && (controller->GetPadState(Controller::State::X, Controller::Type::TRIGGER) || mouse->TriggerMouseLeft()))
		{
			rThrowFlag = true;
			avoidTime = 0.0f;
		}

		ropeObj->Update();
		return;
	}

	// ロープが接着していれば
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
	// フラグがtrueじゃない場合は初期化してリターンする
	if (!rThrowFlag)
	{
		manageRopePos = {};
		manageRopeScale = {};
		return;
	}

	if (targetLength <= baseLength && tLength <= 0.0f)
	{
		tPos = targetPos;
		tLength = targetLength;
	}

	XMVECTOR startPos = { pPos.x, pPos.y, pPos.z, 1 };
	XMVECTOR endPos = { tPos.x, tPos.y, tPos.z, 1 };

	XMVECTOR subPlayerEnemy = XMVectorSubtract(endPos, startPos);

	XMFLOAT3 subPE = { subPlayerEnemy.m128_f32[0], subPlayerEnemy.m128_f32[1], subPlayerEnemy.m128_f32[2] };

	if (rThrowFlag)
	{
		avoidTime += 0.05f;

		// Y軸周りの角度
		angleY = (float)atan2(pPos.x - tPos.x, pPos.z - tPos.z);
		vecXZ = sqrtf((pPos.x - tPos.x) * (pPos.x - tPos.x) + (pPos.z - tPos.z) * (pPos.z - tPos.z));
		// X軸周りの角度
		angleX = (float)atan2(tPos.y - pPos.y, vecXZ);
		rRot = { XMConvertToDegrees(angleX), XMConvertToDegrees(angleY), 0 };
		ropeObj->SetRotation(rRot);

		float timeThrowPos = avoidTime * (2.0f - avoidTime);
		manageRopePos = (manageRopePos * (1.0f - timeThrowPos) + subPE * timeThrowPos) / 2;

		manageRopeScale.x += 0.01f;
		manageRopeScale.y += 0.01f;

		float timeThrowScale = avoidTime * (2.0f - avoidTime);
		manageRopeScale.z = (manageRopeScale.z * (1.0f - timeThrowScale) + tLength * timeThrowScale) / 2;

		if (avoidTime >= 1.0f)
		{
			avoidTime = 0.0f;
			this->endPos = tPos;
			manageRopePos = {};
			manageRopeScale = {};
			avoidTime = 0.0f;
			rThrowFlag = false;
			tPos = {};
			tLength = 0.0f;
			rFlag = true;
		}
	}
}

void Rope::Reset()
{
	avoidTime = 0.0f;
	rThrowFlag = false;

	// 変数
	rPos = {};
	rScale = {};
	rRot = {};
	manageRopePos = {}; // ロープ位置管理用
	manageRopeScale = { 0.0f, 0.0f, 0.0f }; // ロープスケール管理用
	angleX = 0.0f; // X軸
	angleY = 0.0f; // Y軸
	vecXZ = 0.0f; // XZ平面上のベクトル
	objLength = 0.0f;
	rFlag = false; // 接触フラグ

	// 突進用
	startPos = {}; // 開始位置
	endPos = {}; // 終点位置
}

void Rope::CircularMotion(XMFLOAT3& pos, const XMFLOAT3 centerPos, const float r, int& angle, const int add)
{
	angle += add;

	// 円運動の処理
	pos.x = (cosf(3.14f / 180.0f * angle) * r) + centerPos.x;
	pos.y = (sinf(3.14f / 180.0f * angle) * r) + centerPos.y;
	pos.z = (tanf(3.14f / 180.0f * angle) * r) + centerPos.z;
}
