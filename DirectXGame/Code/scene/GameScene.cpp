#include "GameScene.h"
#include <imgui.h>
#include <cassert>
#include "SceneManager.h"
#include <Random.h>
#include "Helper.h"

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

	CsvFile::CsvToVector(mapNumber, "test");

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
	player->Initialize({ -130.0f, 9.0f, 0.0f }, {5.0f, 5.0f, 1.0f});

	jsonObjectInit("map1");
	jsonObjectInit("map2");
	jsonObjectInit("map3");
	jsonObjectInit("map4");
	jsonObjectInit("map5");
	jsonObjectInit("map6");
	jsonObjectInit("map7");
	jsonObjectInit("map8");
}

void GameScene::Finalize()
{
	// マウスカーソルを表示
	//ShowCursor(true);
}

void GameScene::Update()
{
	skydomeObj->Update();
	player->Update(map[mapNumber[CsvFile::now_y][CsvFile::now_x]]);

	jsonObjectUpdate();

	if (player->GetOnGrounding() == true)
	{
		XMFLOAT3 pos = player->GetBodyPos();
		pos.y -= 1.0f * player->GetObj()->GetScale().y;
		OnLandingEffect(6, pos);
	}

	if (player->GetIsLonger() == true)
	{
		OnBitingEffect(player->GetHeadPos());
	}
}

void GameScene::Draw()
{
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("Player: %f, %f, %f", player->GetObj()->GetPosition().x, player->GetObj()->GetPosition().y, player->GetObj()->GetPosition().z);
	ImGui::Text("moveY: %f", player->GetmoveY());
	ImGui::Text("bodyPos: %f, %f, %f", player->GetBodyPos().x, player->GetBodyPos().y, player->GetBodyPos().z);
	ImGui::Text("headPos: %f, %f, %f,", player->GetHeadPos().x, player->GetHeadPos().y, player->GetHeadPos().z);
	ImGui::Text("GetHeadState: %d", player->GetHeadState());
	ImGui::Checkbox("onGround", &player->GetOnGround());
	ImGui::Text("mapNum: %d", mapNumber[CsvFile::now_y][CsvFile::now_x] + 1);
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
	player->GetObj()->Draw();
	player->GetHedObj()->Draw();

	// マップオブジェクト描画
	for (auto& object : map[mapNumber[CsvFile::now_y][CsvFile::now_x]])
	{
		object->object->Draw();
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
		Object3d* newObject = Object3d::Create();

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

		// タイプごとに移動先の座標を設定するかを決める
		if (newObject->GetType() == "Ground_Move")
		{
			XMFLOAT3 movePos;
			XMStoreFloat3(&movePos, objectData.movePos);
			// 移動先の座標を設定(取得する際はゲッターを使う)
			newObject->SetMovePos(movePos);
		}

		// 配列に登録
		mapObject.push_back(new MapData(newObject, pos));
	}
	map.push_back(mapObject);
	mapSave.push_back(mapObject);
	mapObject.erase(mapObject.begin(), mapObject.end());
}

void GameScene::jsonObjectUpdate()
{
	int div = 120;
	XMFLOAT3 moveVec = { 0, 0, 0 };

	int index = 0;
	int keyIndex = 0;
	std::vector<int> doorIndex;
	for (auto& object : map[mapNumber[CsvFile::now_y][CsvFile::now_x]])
	{
		// オブジェクトごとに処理を変えて更新する
		if (object->object->GetType() == "Ground")
		{
			GroundTypeUpdate(index, object->object);
		}
		// 嚙みつけるオブジェクトとして使ってもらえればと
		else if (object->object->GetType() == "box")
		{
			BoxTypeUpdate(index, object->object, object->gravity);
		}
		// 触れるとステージリセット
		else if (object->object->GetType() == "checkPoint")
		{
			CheckPointTypeUpdate(index, object->object);
		}
		// 鍵
		else if (object->object->GetType() == "key")
		{
			KeyTypeUpdate(keyIndex, index, object->object);
		}
		// ドア
		else if (object->object->GetType() == "door")
		{
			DoorTypeUpdate(doorIndex, index, object->object);
		}
		// 移動するオブジェクトの更新
		else if (object->object->GetType() == "Ground_Move")
		{
			GroundMoveTypeUpdate(index, object, object->originPos, div, moveVec);
		}

		object->object->Update();
		index++;
	}

	if (keyIndex != 0)
	{
		map[mapNumber[CsvFile::now_y][CsvFile::now_x]].erase(map[mapNumber[CsvFile::now_y][CsvFile::now_x]].begin() + keyIndex - 1);
	}
	if (doorIndex.empty() == false)
	{
		for (const auto& m : doorIndex)
		{
			map[mapNumber[CsvFile::now_y][CsvFile::now_x]].erase(map[mapNumber[CsvFile::now_y][CsvFile::now_x]].begin() + m - 1);
		}
	}
}

void GameScene::GroundTypeUpdate(int index, Object3d* object)
{

}

void GameScene::BoxTypeUpdate(int index, Object3d* object, float& gravity)
{
	XMFLOAT3 pos = object->GetPosition();
	pos.y += gravity;
	gravity += addGravity;
	gravity = max(gravity, maxGravity);
	XMFLOAT3 pPos = player->GetBodyPos();
	for (int i = 0; i < map[mapNumber[CsvFile::now_y][CsvFile::now_x]].size(); i++)
	{
		if (i == index) continue;

		if (Collision::CollisionBoxToBox(map[mapNumber[CsvFile::now_y][CsvFile::now_x]][i]->object->GetPosition(), map[mapNumber[CsvFile::now_y][CsvFile::now_x]][i]->object->GetScale(), pos, object->GetScale()))
		{
			pos.y += (map[mapNumber[CsvFile::now_y][CsvFile::now_x]][i]->object->GetPosition().y + map[mapNumber[CsvFile::now_y][CsvFile::now_x]][i]->object->GetScale().y) - (pos.y - object->GetScale().y);
			gravity = 0.0f;
			break;
		}
	}
	object->SetPosition(pos);
}

void GameScene::CheckPointTypeUpdate(int index, Object3d* object)
{
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 scale = object->GetScale();
	XMFLOAT3 pPos = player->GetBodyPos();
	XMFLOAT3 pScale = player->GetObj()->GetScale();

	XMFLOAT3 pScaleHalf = { pScale.x / 2, pScale.y, pScale.z };

	if (Collision::CollisionBoxPoint(pos, scale, pPos, pScaleHalf) && CsvFile::check_change_flag == false)
	{
		copy(mapSave.begin(), mapSave.end(), map.begin());
		CsvFile::check_change_flag = true;
	}
}

void GameScene::KeyTypeUpdate(int keyIndex, int index, Object3d* object)
{
	if (player->GetIsKey() == false && IsCanGetKey(object->GetPosition(), player->GetBodyPos(), object->GetScale().x, player->GetObj()->GetScale().x))
	{
		OnPickingEffect(object->GetPosition());
		player->SetIKey(true);
		keyIndex = index + 1;
	}
}

void GameScene::DoorTypeUpdate(std::vector<int>& doorIndex, int index, Object3d* object)
{
	if (player->GetIsKey() == true && IsCanOpenDoor(object->GetPosition(), player->GetBodyPos(), object->GetScale().x, player->GetObj()->GetScale().x))
	{
		OnPickingEffect(object->GetPosition());
		player->SetIKey(false);
		doorIndex.emplace_back(index + 1);
	}
}

void GameScene::GroundMoveTypeUpdate(int index, MapData* mapData, const XMFLOAT3& originPos, int divide, XMFLOAT3& moveVec)
{
	XMFLOAT3 pPos = player->GetBodyPos();
	XMFLOAT3 pScale = player->GetObj()->GetScale();
	XMFLOAT3 oPos = mapData->object->GetPosition();
	XMFLOAT3 oScale = mapData->object->GetScale();
	if (IsStandingMap(mapData->object) == true)
	{
		mapData->isMove = true;
	}
	else
	{
		mapData->isMove = false;
	}

	XMFLOAT3 movePos = Easing::lerp(originPos, mapData->object->GetMovePos(), static_cast<float>(mapData->moveFrame) / divide);
	if (player->GetIsHit() == false)
	{
		moveVec = movePos - mapData->object->GetPosition();
	}

	mapData->object->SetPosition(movePos);

	if (mapData->isMove == true)
	{
		mapData->moveFrame++;
		mapData->moveFrame = min(mapData->moveFrame, divide);

		player->PushBack(map[mapNumber[CsvFile::now_y][CsvFile::now_x]], moveVec);
	}
	else
	{
		mapData->moveFrame--;
		mapData->moveFrame = max(mapData->moveFrame, 0);
	}
}

void GameScene::OnLandingEffect(int num, const XMFLOAT3& pPos)
{
	for (int i = 0; i < num; i++)
	{
		XMFLOAT3 pos = pPos;
		XMFLOAT3 acc = { static_cast<float>(Random::GetRanNum(10, 50)) / 100, static_cast<float>(Random::GetRanNum(0, 3)) / 100, 0 };
		if (i % 2 == 0)
		{
			acc.x = -acc.x;
		}
		XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.05f };
		XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		landingEffect->Add(10, pos, { 0, 0, 0 }, acc, 0.0f, 10.0f, startColor, endColor);
	}
}

void GameScene::OnPickingEffect(const XMFLOAT3& pPos)
{
	XMFLOAT3 pos = pPos;
	XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.1f };
	XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	takeEffect->Add(10, pos, { 0, 0, 0 }, { 0, 0, 0 }, 20.0f, 0.0f, startColor, endColor);
}

void GameScene::OnBitingEffect(const XMFLOAT3& pPos)
{
	XMFLOAT3 pos = pPos;
	XMFLOAT4 startColorA = { 0.9f, 0.4f, 0.5f, 0.5f };
	XMFLOAT4 startColorB = { 0.7f, 0.7f, 0.4f, 0.5f };
	XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	biteEffect->Add(7, pos, { 0, 0, 0 }, { 0, 0, 0 }, 15.0f, 35.0f, startColorA, endColor);
	biteEffect->Add(7, pos, { 0, 0, 0 }, { 0, 0, 0 }, 15.0f, 45.0f, startColorB, endColor);
}

bool GameScene::IsCanGetKey(const XMFLOAT3& keyPos, const XMFLOAT3& playerPos, float keyRadius, float playerRadius)
{
	// 誤差
	float error = 0.1f;

	// 一定の距離なら
	if (GetLength(keyPos, playerPos) <= keyRadius + playerRadius + error)
	{
		return true;
	}

	return false;
}

bool GameScene::IsCanOpenDoor(const XMFLOAT3& doorPos, const XMFLOAT3& playerPos, float doorRadius, float playerRadius)
{
	// 誤差
	float error = 0.1f;

	// 一定の距離なら
	if (GetLength(doorPos, playerPos) <= doorRadius + playerRadius + error)
	{
		return true;
	}

	return false;
}

bool GameScene::IsStandingMap(Object3d* object)
{
	XMFLOAT3 pPos = player->GetBodyPos();
	XMFLOAT3 pScale = player->GetObj()->GetScale();
	XMFLOAT3 oPos = object->GetPosition();
	XMFLOAT3 oScale = object->GetScale();
	if (pPos.x - pScale.x < oPos.x + oScale.x && oPos.x - oScale.x < pPos.x + pScale.x && oPos.y + oScale.y + pScale.y == pPos.y)
	{
		return true;
	}

	return false;
}
