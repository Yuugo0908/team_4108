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
#include "Rope.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Collision.h"
#include "Light.h"
#include "Easing.h"
#include "LevelLoader.h"
#include "Particle.h"
#include "BaseScene.h"
#include "SafeDelete.h"

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

private: // メンバ変数
	Keyboard* keyboard = Keyboard::GetInstance();
	Controller* controller = Controller::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Camera* camera = Camera::GetInstance();
	Light* light = nullptr;

	Rope* rope = nullptr;
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	// 複数生成用リスト
	std::list<std::unique_ptr<Enemy>> enemys;
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
		HPTextNum = 1, HPBarNum, HPGaugeNum, fadeNum,
	};

	// jsonオブジェクト
	std::vector<std::unique_ptr<Object3d>> jsonObject{};

	// 画像
	Image2d* fadeTex = nullptr;
	float alpha = 1.0f;

	// 音声
	Audio* Bgm = Audio::GetInstance();

	// パーティクル
	Particle* effectBox = nullptr;
	Particle* effectCircle = nullptr;
	Particle* effectCircle2 = nullptr;
	Particle* effectTarget = nullptr;
	Particle* effectAvoid = nullptr;
};