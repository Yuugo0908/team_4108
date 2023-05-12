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
	fadeTex = Image2d::Create(fadeNum, { 0.0f, 0.0f });
	fadeTex->SetSize({ 1280.0f, 720.0f });
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
	//ShowCursor(false);

	skydomeModel = skydomeModel->CreateFromObject("skydome");
	skydomeObj = Object3d::Create();
	skydomeObj->SetModel(skydomeModel);
	skydomeObj->SetScale({7.0f, 5.0f, 5.0f});

	player = new Player;
	player->Initialize({ 0.0f, 9.0f, 0.0f }, {5.0f, 1.0f, 5.0f});

	jsonObjectInit("map1");
}

void GameScene::Finalize()
{
	// マウスカーソルを表示
	//ShowCursor(true);
}

void GameScene::Update()
{
	skydomeObj->Update();
	player->Update(mapObject);

	if (player->GetOnGrounding() == true)
	{
		for (int i = 0; i < 5; i++)
		{
			XMFLOAT3 pos = player->GetObj()->GetPosition();
			pos.y -= 2.0f * player->GetObj()->GetScale().y;
			XMFLOAT3 vel = { 0, 0, 0 };
			XMFLOAT3 acc = { static_cast<float>(Random::GetRanNum(0, 100) - 50) / 100, static_cast<float>(Random::GetRanNum(0, 2)) / 100, 0 };
			XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.05f };
			XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			jumpEffect->Add(10, pos, vel, acc, 0.0f, 10.0f, startColor, endColor);
		}
	}

	if (keyboard->TriggerKey(DIK_Z))
	{
		mapObject.erase(mapObject.begin(), mapObject.end());
		levelData = nullptr;

		if (mapNumber == 1)
		{
			mapNumber = 2;
			jsonObjectInit("map2");
		}
		else if (mapNumber == 2)
		{
			mapNumber = 1;
			jsonObjectInit("map1");
		}
	}

	jsonObjectUpdate();
}

void GameScene::Draw()
{
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("PlayerY: %f", player->GetObj().get()->GetPosition().y);
	ImGui::Text("moveY: %f", player->GetmoveY());
	ImGui::Text("bodyPosX: %f", player->GetBodyPos().y);
	ImGui::Text("headPosX: %f", player->GetHeadPos().y);
	ImGui::Text("GetBiteTimer: %f", player->GetBiteTimer());
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

	// 3Dオブジェクト描画
	skydomeObj->Draw();
	player->GetObj().get()->Draw();
	player->GetHedObj().get()->Draw();

	// マップオブジェクト描画
	for (auto& object : mapObject)
	{
		object->Draw();
	}

	// パーティクル描画
	// パーティクルの描画を先にするとエラーが発生するので注意(原因不明)
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

		// オブジェクトのタイプをセット
		newObject->SetType(objectData.objType);

		// 配列に登録
		mapObject.push_back(std::move(newObject));
	}
}

void GameScene::jsonObjectUpdate()
{
	for (auto& object : mapObject)
	{
		// オブジェクトごとに処理を変えて更新する
		if (object->GetType() == "Ground")
		{
			object->Update();
		}
		// 嚙みつけるオブジェクトとして使ってもらえればと
		else if (object->GetType() == "box")
		{
			object->Update();
		}
		// 触れるとステージリセット
		else if (object->GetType() == "checkPoint")
		{
			object->Update();
		}
	}
}