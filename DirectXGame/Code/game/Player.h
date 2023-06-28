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
#include "CsvFile.h"
#include "MapData.h"

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

	enum BiteBlockState
	{
		NOTBITE,
		NORMAL,
		NOTGRAVIT,
	};

public: // メンバ関数
	//初期化処理
	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);
	// 更新処理
	void Update(std::vector<MapData*> &mapObjects);
	// 押し戻し
	void AddMove(const XMFLOAT3& move);
	// オブジェクト
	Object3d* GetObj() { return playerObj; }
	Object3d* GetHedObj() { return playerHedObj; }

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
	void GroundCollisionProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 地形ブロックX軸衝突処理
	/// </summary>
	/// <param name="mapObjects">マップのブロック</param>
	void BlockCollisionProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 天井に関してのブロック衝突判定
	/// </summary>
	/// <param name="mapObjects">マップのブロック</param>
	void CeilingBlockCollisionProcess(std::vector<MapData*> &mapObjects);
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
	void HeadBiteProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 頭更新処理
	/// </summary>
	void HeadUpdateProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 頭当たり判定処理
	/// </summary>
	/// <returns>頭の当たり判定結果</returns>
	HeadInjectionState HeadCollision(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 頭とブロックの当たり判定
	/// </summary>
	/// <returns>当たり判定</returns>
	bool HeadBlockCollisionCheck(std::vector<MapData*> &mapObjects);

	bool BodyBlockCollisionCheck(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// マップ変更
	/// </summary>
	void MapChange(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 酸ブロックとの当たり判定
	/// </summary>
	/// <param name="mapObjects"></param>
	void AcidProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// チェックポイントに戻る
	/// </summary>
	void ReturnCheckpoint();
	/// <summary>
	/// チェックポイントとの当たり判定
	/// </summary>
	/// <param name="mapObjects"></param>
	void CheckPointProcess(std::vector<MapData*>& mapObjects);
	/// <summary>
	/// 引き寄せブロック処理
	/// </summary>
	/// <param name="mapObjects"></param>
	void AttractBiteProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// ブロック運び処理
	/// </summary>
	/// <param name="mapObjects"></param>
	void CarryBlockProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// 時間確認
	/// </summary>
	/// <param name="time"></param>
	/// <returns></returns>
	bool TimeCheck(float& time);

public:
	//Geter
	bool& GetOnGround() { return onGround; }
	float& GetmoveY() { return moveY; }
	XMFLOAT3& GetBodyPos() { return pPos; }
	void SetBodyPos(const XMFLOAT3& pos) { pPos = pos; }
	XMFLOAT3& GetHeadPos() { return hPos; }
	void SetHeadPos(const XMFLOAT3& pos) { hPos = pos; }
	void SetBodyMove(const XMFLOAT3& move) { this->move = move; }
	XMFLOAT3& GetHeadInjectPos() { return headInjectDis; }
	float& GetBiteTimer() { return biteTimer; }
	HeadState& GetHeadState() { return headState; }

	bool GetNotGravityFlag();
	int GetHeadHitMapNum() { return hitHeadMapObjNum; }
	bool GetOnGrounding()
	{
		return onGround == true && oldOnGround == false;
	}
	void OnGrounding()
	{
		this->onGround = true;
		this->oldOnGround = true;
	}
	bool& GetIsLonger() { return islonger; }
	bool& GetIsKey() { return isKey; }
	void SetIKey(bool iskey) { this->isKey = iskey; }
	bool& GetIsHit() { return isHit; }

private: // メンバ変数

	Keyboard* keyboard = Keyboard::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	Camera* camera = Camera::GetInstance();

	// モデル
	Model* playerModel = nullptr;
	Object3d* playerObj = nullptr;
	Object3d* playerHedObj = nullptr;

	// プレイヤー
	XMFLOAT3 pDirection = {1.0f, 0.0f, 0.0f};	//向き
	XMFLOAT3 objPos = {};						//描画専用体座標
	XMFLOAT3 pPos = {};							//体座標
	XMFLOAT3 oldpPos = {};						//前フレームの体の位置
	XMFLOAT3 hPos = {};							//頭座標
	XMFLOAT3 headInjectDis = {};				//頭射出後位置
	XMFLOAT3 headBackDis = {};					//頭戻り量
	XMFLOAT3 pPosMovePrevious = {};				//体移動前の位置
	XMFLOAT3 pScale = {};						//大きさ
	XMFLOAT3 pRot = {};							//回転
	XMFLOAT3 reSpawnPos = {};					//
	XMFLOAT3 move = {};							//移動量
	XMFLOAT3 hmove = {};						//頭の移動量
	XMFLOAT3 direction = {};					//向いている方向
	float moveY = 0.0f;							//ジャンプ及び重力
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
	bool isKey = false;
	bool isHit = false;
	LimitPos limitPos = NONE;
	BiteBlockState biteBlockState = NOTBITE;
};

