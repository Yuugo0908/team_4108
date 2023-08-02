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

void Rope::Update(const XMFLOAT3& startPos, const XMFLOAT3& endPos)
{
	// ロープは常に繋がっている
	objLength = GetLength(startPos, endPos);
	angleY = (float)atan2(startPos.x - endPos.x, startPos.z - endPos.z);
	vecXZ = sqrtf((startPos.x - endPos.x) * (startPos.x - endPos.x) + (startPos.z - endPos.z) * (startPos.z - endPos.z));
	angleX = (float)atan2(endPos.y - startPos.y, vecXZ);

	rPos = { (startPos.x + endPos.x) / 2, (startPos.y + endPos.y) / 2, (startPos.z + endPos.z) / 2 + 0.1f };
	rScale = { 0.01f, 1.0f , objLength / 2.0f };
	ropeObj->SetPosition(rPos);
	ropeObj->SetScale(rScale);
	ropeObj->SetRotation({ XMConvertToDegrees(angleX), XMConvertToDegrees(angleY), 0 });
	ropeObj->Update();
}

void Rope::Draw()
{
	ropeObj->Draw();
}
