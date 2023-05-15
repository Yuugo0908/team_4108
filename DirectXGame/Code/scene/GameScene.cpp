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
	landingEffect.reset(Particle::Create(L"Resources/effectCircle.png"));
	// 取得時のパーティクル
	takeEffect.reset(Particle::Create(L"Resources/effectCircle2.png"));
	// かみつき時のパーティクル
	biteEffect.reset(Particle::Create(L"Resources/biteEffectAll.png"));

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
	jsonObjectInit("map2");
	jsonObjectInit("map3");
	jsonObjectInit("map4");
}

void GameScene::Finalize()
{
	// マウスカーソルを表示
	//ShowCursor(true);
}

void GameScene::Update()
{
	bool flag = player->GetMapChange();
	if (flag)
	{
		levelData = nullptr;

		if (mapNumber == 1)
		{
			mapNumber = 2;
		}
		else if (mapNumber == 2)
		{
			mapNumber = 1;
		}
	}
	else if (keyboard->TriggerKey(DIK_Z))
	{
		if (mapNumber == 1)
		{
			mapNumber = 2;
		}
		else if (mapNumber == 2)
		{
			mapNumber = 1;
		}
	}
	jsonObjectUpdate();

	skydomeObj->Update();
	player->Update(map[mapNumber]);

	if (player->GetOnGrounding() == true)
	{
		OnLandingEffect(6);
	}

	if (keyboard->TriggerKey(DIK_RETURN))
	{
		OnBitingEffect();
	}
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
	for (auto& object : map[mapNumber])
	{
		object->Draw();
	}

	// パーティクル描画
	// パーティクルの描画を先にするとエラーが発生するので注意(原因不明)
	landingEffect->Draw(DirectXCommon::GetInstance()->GetCommandList());
	takeEffect->Draw(DirectXCommon::GetInstance()->GetCommandList());
	biteEffect->Draw(DirectXCommon::GetInstance()->GetCommandList());

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
	map.push_back(std::move(mapObject));
}

void GameScene::jsonObjectUpdate()
{
	for (auto& object : map[mapNumber])
	{
		// オブジェクトごとに処理を変えて更新する
		if (object->GetType() == "Ground")
		{
		}
		// 嚙みつけるオブジェクトとして使ってもらえればと
		else if (object->GetType() == "box")
		{
		}
		// 触れるとステージリセット
		else if (object->GetType() == "checkPoint")
		{
		}
		object->Update();
	}
}

void GameScene::OnLandingEffect(int num)
{
	for (int i = 0; i < num; i++)
	{
		XMFLOAT3 pos = player->GetObj()->GetPosition();
		pos.y -= 2.0f * player->GetObj()->GetScale().y;
		XMFLOAT3 vel = { 0, 0, 0 };
		XMFLOAT3 acc = { static_cast<float>(Random::GetRanNum(10, 50)) / 100, static_cast<float>(Random::GetRanNum(0, 3)) / 100, 0 };
		if (i % 2 == 0)
		{
			acc.x = -acc.x;
		}
		XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.05f };
		XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		landingEffect->Add(10, pos, vel, acc, 0.0f, 10.0f, startColor, endColor);
	}
}

void GameScene::OnPickingEffect()
{
	XMFLOAT3 pos = player->GetObj()->GetPosition();
	XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.1f };
	XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	takeEffect->Add(10, pos, { 0, 0, 0 }, { 0, 0, 0 }, 20.0f, 0.0f, startColor, endColor);
}

void GameScene::OnBitingEffect()
{
	XMFLOAT3 pos = player->GetObj()->GetPosition();
	XMFLOAT3 vel = { 0, 0, 0 };
	XMFLOAT3 acc = { 0, 0, 0 };
	XMFLOAT4 startColorA = { 0.9f, 0.4f, 0.5f, 0.5f };
	XMFLOAT4 startColorB = { 0.7f, 0.7f, 0.4f, 0.5f };
	XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	biteEffect->Add(7, pos, vel, acc, 10.0f, 30.0f, startColorA, endColor);
	biteEffect->Add(7, pos, vel, acc, 10.0f, 40.0f, startColorB, endColor);
}
