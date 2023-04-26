#pragma once
#include "Object3d.h"
#include "Collision.h"
#include "Operator.h"
#include "Camera.h"

class Bullet
{
public: // メンバ関数

	bool Initialize(Model* bulletModel, const XMFLOAT3& pPos, const XMFLOAT3& ePos);

	void Update();

	void Search();

	void Draw();

	bool MapCollide(XMFLOAT3 boxPos, XMFLOAT3 boxScale);

	const std::unique_ptr<Object3d>& GetObj() { return bulletObj; }

	const XMFLOAT3& GetPos() { return bPos; }
	void SetPos(XMFLOAT3 bPos) { this->bPos = bPos; }

	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

private: // メンバ変数
	std::unique_ptr<Object3d> bulletObj = nullptr;

	// バレット
	XMFLOAT3 bPos = {};
	XMFLOAT3 bOldPos = {};
	XMFLOAT3 bScale = {};
	XMFLOAT3 bSpeed = {};
	bool attackFlag = false;
	bool searchFlag = false;
	bool shakeFlag = false; // カメラのシェイク
	int attackCount = 0;

	// プレイヤー
	XMFLOAT3 pPos = {};

	// エネミー
	XMFLOAT3 ePos = {};
	XMFLOAT3 ePosOld = {};
};

