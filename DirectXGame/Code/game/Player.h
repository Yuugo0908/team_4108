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
public: // メンバ関数

	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);

	void Update(bool rFlag, bool moveFlag);

	void Rush(XMFLOAT3 targetPos, bool& flag, float& avoidTime);

	void Avoid();

	void Jump();

	bool Damage(const std::unique_ptr<Object3d>& object);

	void Reset();

	void ReSpawn();

	void TrackRot(const XMFLOAT3& startPos, const XMFLOAT3& endPos);

	// マップ当たり判定
	bool MapCollide(XMFLOAT3 boxPos, XMFLOAT3 boxScale);

	bool StageCollide(XMFLOAT3 stagePos, XMFLOAT3 stageScale, bool& reverseFlag);

	bool PoleCollide(XMFLOAT3 polePos, XMFLOAT3 poleScale);

	// オブジェクト
	const std::unique_ptr<Object3d>& GetObj() { return playerObj; }
	// 回避
	const bool& GetAvoidFlag() { return avoidFlag; }
	void SetAvoidFlag(bool avoidFlag) { this->avoidFlag = avoidFlag; }
	// 着地
	const bool& GetOnGround() { return onGround; }
	void SetOnGround(bool onGround) { this->onGround = onGround; }

	// ジャンプ
	void SetJumpFlag(bool jumpFlag) { this->jumpFlag = jumpFlag; }

private: // メンバ変数

	Keyboard* keyboard = Keyboard::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	Camera* camera = Camera::GetInstance();

	// モデル
	Model* playerModel = nullptr;
	std::unique_ptr<Object3d> playerObj = nullptr;

	// プレイヤー
	XMFLOAT3 pPos = {};//座標
	XMFLOAT3 pPosOld = {};
	XMFLOAT3 pScale = {};//大きさ
	XMFLOAT3 pRot = {};//回転
	XMFLOAT3 inertiaSave = {}; // 慣性
	XMFLOAT3 reSpawnPos = {};

	float pSpeed = 0.35f;
	bool onGround = false;//自由落下のフラグ
	bool jumpFlag = false;
	float pMove = 0.0f;//移動量
	float pAcc = 0.2f;//加速
	float pVel = 0.2f;//速度
	float pDown = 0.0f;
	float pGra = 0.1f;//重力
	float rate = 1.0f; // 斜め移動時の制限
	int damageInterval = 0; // 攻撃を受けた際のインターバル

	// 突進用
	bool avoidFlag = false; // 回避開始フラグ
	int avoidCount = 0;

	// カメラ距離取得用
	XMFLOAT3 cameraTrack = {};
	float cameraRot = 0.0f;
};

