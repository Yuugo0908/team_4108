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
public:
	enum BodyState
	{
		STATE_BODY_NORMAL,
		STATE_BODY_JUMP_UP,
		STATE_BODY_JUMP_DOWN,
		STATE_BODY_MOVE
	};

	enum HeadState
	{
		STATE_NORMAL,
		STATE_INJECTION,
		STATE_BITE,
		STATE_BACK,
		STATE_INJECTIONLOCK
	};

	enum HeadInjectionState
	{
		STATE_NULL,
		STATE_MOVE,
		STATE_BITEHIT,
		STATE_UNBITEHIT,
	};

	enum BodyCollsionState
	{
		BODYSTATE_NULL,
		BODYSTATE_CEILING_COLISION,
		BODYSTATE_X_COLISION,
		BODYSTATE_Y_COLISION,
		BODYSTATE_XY_COLISION
	};

	enum LimitPos
	{
		NONE,
		UP_LIMIT,
		DOWN_LIMIT,
		RIGHT_LIMIT,
		LEFT_LIMIT
	};

public: // メンバ関数
	//初期化処理
	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);
	// 更新処理
	void Update(std::vector<std::unique_ptr<Object3d>> &mapObjects);
	// オブジェクト
	const std::unique_ptr<Object3d>& GetObj() { return playerObj; }
	const std::unique_ptr<Object3d>& GetHedObj() { return playerHedObj; }

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
	/// 地面Y軸衝突判定処理
	/// </summary>
	void GroundCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 地形ブロックX軸衝突処理
	/// </summary>
	/// <param name="mapObjects">マップのブロック</param>
	void BlockCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 天井に関してのブロック衝突判定
	/// </summary>
	/// <param name="mapObjects">マップのブロック</param>
	void CeilingBlockCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 頭射出処理
	/// </summary>
	void HeadInjectionProcess();
	/// <summary>
	/// 頭射出移動処理
	/// </summary>
	void HeadInjectionMoveProcess();
	/// <summary>
	/// 噛みつけるか判定
	/// </summary>
	void HeadBiteCollisionProcess();
	/// <summary>
	/// 頭戻る処理
	/// </summary>
	void HeadBackMoveProcess();
	/// <summary>
	/// 噛みつき処理
	/// </summary>
	void HeadBiteProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 頭更新処理
	/// </summary>
	void HeadUpdateProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 頭当たり判定処理
	/// </summary>
	/// <returns>頭の当たり判定結果</returns>
	HeadInjectionState HeadCollision(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 頭とブロックの当たり判定
	/// </summary>
	/// <returns>当たり判定</returns>
	bool HeadBlockCollisionCheck(std::vector<std::unique_ptr<Object3d>>& mapObjects);

	bool BodyBlockCollisionCheck(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// マップ変更
	/// </summary>
	void MapChange(std::vector<std::unique_ptr<Object3d>>& mapObjects);
	/// <summary>
	/// 酸ブロックとの当たり判定
	/// </summary>
	/// <param name="mapObjects"></param>
	void AcidProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects);

	bool TimeCheck(float& time);
public:

	//Geter
	bool& GetOnGround() { return onGround; }
	float& GetmoveY() { return moveY; }
	XMFLOAT3& GetBodyPos() { return pPos; }
	XMFLOAT3& GetHeadPos() { return hPos; }
	XMFLOAT3& GetHeadInjectPos() { return headInjectDis; }
	float& GetBiteTimer() { return biteTimer; }
	HeadState& GetHeadState() { return headState; }
	/// <summary>
	/// 地面に接した瞬間か
	/// </summary>
	/// <returns>成否</returns>
	bool GetOnGrounding()
	{
		return onGround == true && onGround != oldOnGround;
	}
	bool& GetMapChange() { return mapChangeFlag; }
	bool& GetIsLonger() { return islonger; }

private: // メンバ変数

	Keyboard* keyboard = Keyboard::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	Camera* camera = Camera::GetInstance();

	// モデル
	Model* playerModel = nullptr;
	std::unique_ptr<Object3d> playerObj = nullptr;
	std::unique_ptr<Object3d> playerHedObj = nullptr;

	// プレイヤー
	XMFLOAT3 pDirection = {1.0f, 0.0f, 0.0f}; //向き
	XMFLOAT3 objPos = {}; //描画専用体座標
	XMFLOAT3 pPos = {};//体座標
	XMFLOAT3 oldpPos = {};
	XMFLOAT3 hPos = {};//頭座標
	XMFLOAT3 headInjectDis = {}; //頭射出後位置
	XMFLOAT3 headBackDis = {}; //頭戻り量
	XMFLOAT3 pPosOld = {};
	XMFLOAT3 pScale = {};//大きさ
	XMFLOAT3 pRot = {};//回転
	XMFLOAT3 reSpawnPos = {};
	XMFLOAT3 move = {}; //移動量
	XMFLOAT3 hmove = {}; //頭の移動量
	XMFLOAT3 direction = {}; //向いている方向
	float moveY = 0.0f; //ジャンプ及び重力
	bool onGround = false;
	bool oldOnGround = false;
	HeadState headState = STATE_NORMAL;
	BodyState bodyState = STATE_BODY_NORMAL;
	int hitHeadMapObjNum = 0;
	int hitbodyMapObjNum = 0;
	float biteTimer = 5.0f;
	float timeMax = 1.0f;
	float moveTime = 1.0f;
	BodyCollsionState bodyColState = BODYSTATE_NULL;
	int colisionBlockNum = 0;
	bool islonger = false;

	LimitPos limitPos = NONE;
	XMFLOAT3 checkPointPos = {};
	bool mapChangeFlag = false;
};

