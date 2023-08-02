#pragma once

#include "DirectXCommon.h"
#include "Keyboard.h"
#include "Controller.h"
#include "Mouse.h"
#include "Image2d.h"
#include "DebugText.h"
#include "Object3d.h"
#include "Model.h"
#include "Operator.h"
#include "Camera.h"
#include "Audio.h"
#include "Player.h"
#include "Collision.h"
#include "Light.h"
#include "Easing.h"
#include "LevelLoader.h"
#include "Particle.h"
#include "BaseScene.h"
#include "CsvFile.h"
#include "SafeDelete.h"
#include "MapData.h"
#include "Rope.h"

#include <stdlib.h>
#include <time.h>
#include <list>

class GameScene : public BaseScene
{
public: // メンバ関数
	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 毎フレーム処理
	void Update() override;
	// 描画
	void Draw() override;

	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

	// jsonオブジェクトの初期化
	void jsonObjectInit(const std::string sceneName);
	// jsonオブジェクトの更新
	void jsonObjectUpdate();

	// オブジェクトタイプごとの処理
	void GroundTypeUpdate(int index, Object3d* object);
	void BoxTypeUpdate(int index, Object3d* object, float& gravity);
	void CheckPointTypeUpdate(int index, Object3d* object);
	void KeyTypeUpdate(int& keyIndex, int index, Object3d* object);
	void DoorTypeUpdate(std::vector<int>& doorIndex, int index, Object3d* object);
	void GroundMoveTypeUpdate(int index, MapData* mapData, const XMFLOAT3& originPos, int divide);



	/// <summary>
	/// 着地時エフェクト
	/// </summary>
	/// <param name="num">発生数</param>
	/// <param name="pPos">発生座標</param>
	void OnLandingEffect(int num, const XMFLOAT3& pPos);

	/// <summary>
	/// 取得時エフェクト
	/// </summary>
	/// <param name="pPos">発生座標</param>
	void OnPickingEffect(const XMFLOAT3& pPos);

	/// <summary>
	/// かみつき時のパーティクル
	/// </summary>
	/// <param name="pPos">発生座標</param>
	void OnBitingEffect(const XMFLOAT3& pPos);

	/// <summary>
	/// 鍵を取得できるか
	/// </summary>
	/// <param name="keyPos">鍵穴の座標</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="keyRadius">鍵穴の横幅</param>
	/// <param name="playerRadius">プレイヤーの横幅</param>
	/// <returns>成否</returns>
	bool IsCanGetKey(const XMFLOAT3& keyPos, const XMFLOAT3& playerPos, float keyRadius, float playerRadius);

	/// <summary>
	/// ドアを開けられるか
	/// </summary>
	/// <param name="doorPos">ドアの座標</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="doorRadius">ドアの横幅</param>
	/// <param name="playerRadius">プレイヤーの横幅</param>
	/// <returns>成否</returns>
	bool IsCanOpenDoor(const XMFLOAT3& doorPos, const XMFLOAT3& playerPos, const XMFLOAT3& doorRadius, const XMFLOAT3& playerRadius);

	/// <summary>
	/// 動く床に立っているか
	/// </summary>
	/// <returns></returns>
	bool CheckHitGroundMoveType(Object3d* object);

	/// <summary>
	/// X軸の押し戻し
	/// </summary>
	/// <param name="pPos">プレイヤーの座標</param>
	/// <param name="pR">プレイヤーの半径</param>
	/// <param name="oPos">オブジェクトの座標</param>
	/// <param name="oR">オブジェクトの半径</param>
	void PushBackX(XMFLOAT3& pPos, const XMFLOAT3& pScale, const XMFLOAT3& oPos, const XMFLOAT3& oScale);

	/// <summary>
	/// Y軸の押し戻し
	/// </summary>
	/// <param name="pPos">プレイヤーの座標</param>
	/// <param name="pR">プレイヤーの半径</param>
	/// <param name="oPos">オブジェクトの座標</param>
	/// <param name="oR">オブジェクトの半径</param>
	/// <param name="hit">当たったか</param>
	void PushBackY(XMFLOAT3& pPos, const XMFLOAT3& pScale, const XMFLOAT3& oPos, const XMFLOAT3& oScale, bool& hit);

private: // メンバ変数
	Keyboard* keyboard = Keyboard::GetInstance();
	Controller* controller = Controller::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Camera* camera = Camera::GetInstance();
	Rope* rope = Rope::GetInstance();
	Light* light = nullptr;

	// 複数生成用リスト
	LevelData* levelData = nullptr;

	enum Scene
	{
		title_, tutorial_, game_, clear_, failure_
	};

	enum ObjectType
	{
		sphere_, box_, stage_, wall_, pole_, skydome_,
	};

	enum Image2dNum
	{
		HPTextNum = 1, HPBarNum, HPGaugeNum, fadeNum, backNum
	};

	// jsonオブジェクト
	std::vector<MapData*> mapObject{};
	// 全マップデータのリスト
	std::vector<std::vector<MapData*>> map{};
	// 保存用
	std::vector<std::vector<MapData*>> mapSave{};
	const float addGravity = -0.1f;
	const float maxGravity = -2.0f;
	// マップ番号管理(csvファイル)
	std::vector<std::vector<int>> mapNumber;

	// 画像
	Image2d* fadeTex = nullptr;
	Image2d* backGround = nullptr;
	float alpha = 1.0f;

	// 音声
	Audio* Bgm = Audio::GetInstance();

	// パーティクル
	Particle* effectBox = nullptr;
	Particle* effectCircle = nullptr;
	Particle* effectCircle2 = nullptr;
	Particle* effectTarget = nullptr;
	Particle* effectAvoid = nullptr;

	// 着地時のパーティクル
	std::unique_ptr<Particle> landingEffect = nullptr;
	// 取得時のパーティクル
	std::unique_ptr<Particle> takeEffect = nullptr;
	// かみつき時のパーティクル
	std::unique_ptr<Particle> biteEffect = nullptr;

	//player
	Player* player = nullptr;

	// ドア
	bool doorOpen = false;

	int waitTimer = 0;
	const int maxWait = 30;
};