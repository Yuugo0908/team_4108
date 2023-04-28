#include "GameScene.h"
#include <imgui.h>
#include <cassert>
#include "SceneManager.h"
#include <Random.h>

void GameScene::Initialize()
{
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	if (!Image2d::LoadTexture(fadeNum, L"Resources/fade.png"))
	{
		assert(0);
	}
	fadeTex = Image2d::Create(fadeNum, { 0.0f,0.0f });
	fadeTex->SetSize({ 1280.0f,720.0f });
	fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	// パーティクル生成
	effectBox = Particle::Create(L"Resources/effectBox.png");
	// 着地時のパーティクル
	jumpEffect.reset(Particle::Create(L"Resources/effectCircle.png"));

	//enemy->ModelInit();
	//rope->Initialize();

	// ライトの生成
	light = Light::Create();
	// ライトの色を設定
	light->SetLightColor({ 1.0f, 1.0f, 1.0f });
	// 3Dオブジェクトにライトをセット
	Object3d::SetLight(light);

	//Bgm->PlayWave("Resources/BGM/bgm.wav", 255, 0.08f);

	// マウスカーソルを非表示
	ShowCursor(false);

	skydomeModel = skydomeModel->CreateFromObject("skydome");
	skydomeObj = Object3d::Create();
	skydomeObj->SetModel(skydomeModel);

	player = new Player;
	player->Initialize({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f});
}

void GameScene::Finalize()
{
	// マウスカーソルを表示
	ShowCursor(true);
}

void GameScene::Update()
{
	skydomeObj->Update();
	player->Update();

	if (player->GetOnGrounding() == true)
	{
		for (int i = 0; i < 8; i++)
		{
			XMFLOAT3 pos = player->GetObj()->GetPosition();
			pos.y -= 0.8f * player->GetObj()->GetScale().y;
			XMFLOAT3 vel = { 0, 0, 0 };
			XMFLOAT3 acc = { static_cast<float>(Random::GetRanNum(0, 100) - 50) / 500, static_cast<float>(Random::GetRanNum(0, 10)) / 500, 0 };
			XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.1f };
			XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			jumpEffect->Add(10, pos, vel, acc, 0.0f, 1.0f, startColor, endColor);
		}
	}
}

void GameScene::Draw()
{
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("PlayerY: %f", player->GetObj().get()->GetPosition().y);
	ImGui::Text("moveY: %f", player->GetmoveY());
	ImGui::Checkbox("onGround", &player->GetOnGround());
	ImGui::End();
#pragma region 背景画像描画
	// 背景画像描画前処理
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());


	// 画像描画後処理
	Image2d::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景画像描画

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	skydomeObj->Draw();
	player->GetObj().get()->Draw();
	jumpEffect->Draw(DirectXCommon::GetInstance()->GetCommandList());

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region 前景画像描画
	// 前景画像描画前処理
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(DirectXCommon::GetInstance()->GetCommandList());
	// 画像描画後処理
	Image2d::PostDraw();
#pragma endregion 前景画像描画
}

void GameScene::jsonObjectInit(const std::string sceneName)
{
	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile(sceneName);

	// レベルデータからオブジェクトを生成、配置
	for (LevelData::ObjectData& objectData : levelData->objects)
	{
		if (objectData.fileName == "player")
		{
			// 座標
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, objectData.trans);
			// 大きさ
			XMFLOAT3 scale;
			XMStoreFloat3(&scale, objectData.scale);
			// 当たり判定
			XMFLOAT3 size;
			XMStoreFloat3(&size, objectData.size);

			// プレイヤーを生成
			player->Initialize(pos, scale);
			//pPos = player->GetObj()->GetPosition();
			player->GetObj()->SetCollisionScale(size);
			// カメラの設定
			camera->Reset();
			//camera->SetTarget(pPos);
			camera->Update();
			continue;
		}

		if (objectData.fileName == "enemy")
		{
			// 座標
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, objectData.trans);
			// 大きさ
			XMFLOAT3 scale;
			XMStoreFloat3(&scale, objectData.scale);
			// 当たり判定
			XMFLOAT3 size;
			XMStoreFloat3(&size, objectData.size);

			// エネミーを生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(player);
			newEnemy->GetObj()->SetPosition(pos);
			newEnemy->GetObj()->SetScale(scale);
			newEnemy->GetObj()->SetCollisionScale(size);
			enemys.push_back(std::move(newEnemy));
			//enemyCount++;
			continue;
		}

		// 3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = Object3d::Create();

		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(levelData->models)::iterator it = levelData->models.find(objectData.fileName);
		if (it != levelData->models.end())
		{
			model = it->second;
		}

		newObject->SetModel(model);

		// 座標
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, objectData.trans);
		newObject->SetPosition(pos);

		// 回転角
		XMFLOAT3 rot;
		XMStoreFloat3(&rot, objectData.rot);
		newObject->SetRotation(rot);

		// 大きさ
		XMFLOAT3 scale;
		XMStoreFloat3(&scale, objectData.scale);
		newObject->SetScale(scale);

		// 当たり判定
		XMFLOAT3 colScale;
		XMStoreFloat3(&colScale, objectData.size);
		newObject->SetCollisionScale(colScale);

		if (objectData.fileName == "stage")
		{
			newObject->SetType(stage_);
		}
		else if (objectData.fileName == "box")
		{
			newObject->SetType(box_);
		}
		else if (objectData.fileName == "wall")
		{
			newObject->SetType(wall_);
		}
		else if (objectData.fileName == "pole")
		{
			newObject->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
			newObject->SetType(pole_);
		}
		else if (objectData.fileName == "skydome")
		{
			newObject->SetType(skydome_);
		}

		// 配列に登録
		jsonObject.push_back(std::move(newObject));
	}
}

void GameScene::jsonObjectUpdate()
{
	for (auto& object : jsonObject)
	{
		object->Update();
	}
}