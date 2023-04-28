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
	//初期化処理
	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);
	// 更新処理
	void Update();
	// オブジェクト
	const std::unique_ptr<Object3d>& GetObj() { return playerObj; }
	
	//Geter
	bool& GetOnGround() { return onGround; }
	float& GetmoveY() { return moveY; }

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void MoveProcess();
	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void JumpProcess();
	/// <summary>
	/// 重力処理
	/// </summary>
	void GravityProcess();
	/// <summary>
	/// 地面衝突判定処理
	/// </summary>
	void GroundCollisionProcess();

public:
	/// <summary>
	/// 地面に接した瞬間か
	/// </summary>
	/// <returns>成否</returns>
	bool GetOnGrounding()
	{
		return onGround == true && onGround != oldOnGround;
	}

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
	XMFLOAT3 reSpawnPos = {};
	XMFLOAT3 move = {}; //移動量
	float moveY = 0.0f; //ジャンプ及び重力
	bool onGround = false;
	bool oldOnGround = false;
};

