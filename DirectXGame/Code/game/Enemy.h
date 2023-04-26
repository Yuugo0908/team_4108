#pragma once
#include "Object3d.h"
#include "Collision.h"
#include "Operator.h"
#include "Player.h"
#include "Bullet.h"

#include <stdlib.h>
#include <time.h>

class Enemy
{
public:
	// 行動フェーズ
	enum class Phase
	{
		attack,
		move,
		stay,
	};

	bool ModelInit();

	bool Initialize(Player* player);

	bool BulletCreate();

	void Update();

	void Finalize();

	void Draw();

	void Attack();

	void Move();

	void Stay();

	void Jump();

	void Spawn();

	void TrackRot(const XMFLOAT3& startPos, const XMFLOAT3& endPos);

	void Reset();

	bool ObstacleDetection(XMFLOAT3 pPos, XMFLOAT3 boxPos, XMFLOAT3 boxScale);

	bool EnemyCollision(const std::unique_ptr<Object3d>& object);

	bool BulletCollision();

	// マップ当たり判定
	bool MapCollide(const XMFLOAT3 boxPos, const XMFLOAT3 boxScale);
	bool StageCollide(const XMFLOAT3 stagePos, const XMFLOAT3 stageScale);

	const std::unique_ptr<Object3d>& GetObj() { return enemyObj; }

	const bool& GetAlive() { return eAlive; }
	void SetAlive(bool eAlive) { this->eAlive = eAlive; }

	void SetAttackFlag(bool attackFlag) { this->attackFlag = attackFlag; }
	const bool& GetAttackFlag() { return attackFlag; }
	// 着地
	void SetOnGround(bool onGround) { this->onGround = onGround; }

	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

private:
	Player* player = nullptr;

	static Model* enemyModel;
	std::unique_ptr<Object3d> enemyObj = nullptr;

	// 弾複数生成用
	static Model* bulletModel;
	std::list<std::unique_ptr<Bullet>> bullets;

	// エネミー
	XMFLOAT3 ePos = {};
	XMFLOAT3 eOldPos = {};
	XMFLOAT3 eScale = {};
	XMFLOAT3 eRot = {};
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 randPos = {}; // ランダムなスポーン位置

	Phase phase = Enemy::Phase::move;

	float eUp = 0.0f; // 上昇
	float eDown = 0.0f;// 下降
	float eGra = 0.1f; // 重力
	float PElength = 0.0f;
	float lengthOld = 0.0f;

	int eAliveCount = 0;
	int attackCount = 0;

	bool eFlag = false; // 自由落下のフラグ
	bool eAlive = false;// 生きているかのフラグ
	bool attackFlag = false;// 攻撃がプレイヤーに当たるかのフラグ
	bool jumpFlag = false;
	bool onGround = false;
	bool hitFlag = false;

	// プレイヤー
	XMFLOAT3 pPos = {};
};

