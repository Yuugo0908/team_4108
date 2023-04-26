#pragma once
#include "Object3d.h"
#include "Controller.h"
#include "Easing.h"
#include "Collision.h"
#include "Operator.h"

class Rope
{
public: // メンバ関数

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

	// 円運動
	void CircularMotion(XMFLOAT3& pos, const XMFLOAT3 centerPos, const float r, int& angle, const int add);

	// 移動しているか
	const bool& GetmoveFlag() { return moveFlag; }
	void SetmoveFlag(bool moveFlag) { this->moveFlag = moveFlag; }
	// ロープがつながっているか
	const bool& GetrFlag() { return rFlag; }
	void SetrFlag(bool rFlag) { this->rFlag = rFlag; }
	// ロープを飛ばしているか
	const bool& GetThrowFlag() { return rThrowFlag; }
	void SetThrowFlag(bool throwFlag) { rThrowFlag = throwFlag; }
	
	const std::unique_ptr<Object3d>& GetObj() { return ropeObj; }

private: // メンバ変数

	Controller* controller = Controller::GetInstance();
	Mouse* mouse = Mouse::GetInstance();

	//ロープモデル
	Model* ropeModel = nullptr;
	std::unique_ptr<Object3d> ropeObj = nullptr;

	// 変数
	XMFLOAT3 rPos = {};
	XMFLOAT3 rScale = {};
	XMFLOAT3 rRot = {};
	XMFLOAT3 manageRopePos = {}; // ロープ位置管理用
	XMFLOAT3 manageRopeScale = { 0.0f, 0.0f, 0.0f }; // ロープスケール管理用
	const float maxRope = 15.0f; // ロープの最大
	float angleX = 0.0f; // X軸
	float angleY = 0.0f; // Y軸
	float vecXZ = 0.0f; // XZ平面上のベクトル
	float objLength = 0.0f;
	bool rFlag = false; // 接触フラグ
	bool rThrowFlag = false; // ロープを飛ばす
	bool rBackFlag = false; // ロープを戻す
	bool rRotFlag = false;

	XMFLOAT3 cSpeed = {};
	float cRot = 0.0f;

	XMFLOAT3 tPos = {};
	float tLength = 0.0f;

	// 突進用
	XMFLOAT3 startPos = {}; // 開始位置
	XMFLOAT3 endPos = {}; // 終点位置
	float avoidTime = 0.0f; // 経過時間
	bool pEaseFlag = false;
	bool eEaseFlag = false;

	// 移動管理フラグ
	bool moveFlag = false; // 自機が移動しているか
};

