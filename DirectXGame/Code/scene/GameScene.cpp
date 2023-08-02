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
	if (!Image2d::LoadTexture(99, L"Resources/wrench.png"))
	{
		assert(0);
	}

	fadeTex = Image2d::Create(fadeNum, { 0.0f, 0.0f });
	fadeTex->SetSize({ 1280.0f, 720.0f });
	fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	if (!Image2d::LoadTexture(backNum, L"Resources/backGround.png"))
	{
		assert(0);
	}
	backGround = Image2d::Create(backNum, { 0.0f,0.0f });
	backGround->SetSize({ 1280.0f,720.0f });
	backGround->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	// パーティクル生成
	// 着地時のパーティクル
	landingEffect.reset(Particle::Create(L"Resources/effectCircle.png"));
	// 取得時のパーティクル
	takeEffect.reset(Particle::Create(L"Resources/effectCircle2.png"));
	// かみつき時のパーティクル
	biteEffect.reset(Particle::Create(L"Resources/biteEffectAll.png"));

	CsvFile::CsvToVector(mapNumber, "mapNumber");

	// ライトの生成
	light = Light::Create();
	// ライトの色を設定
	light->SetLightColor({ 1.0f, 1.0f, 1.0f });
	// 3Dオブジェクトにライトをセット
	Object3d::SetLight(light);

	// マウスカーソルを非表示
	//ShowCursor(false);

	player = new Player;
	player->Initialize({ -130.0f, 9.0f, 0.0f }, {5.0f, 5.0f, 1.0f});

	rope->Initialize();

	jsonObjectInit("map1");
	jsonObjectInit("map2");
	jsonObjectInit("map3");
	jsonObjectInit("map4");
	jsonObjectInit("map5");
	jsonObjectInit("map6");
	jsonObjectInit("map7");
	jsonObjectInit("map8");
	jsonObjectInit("map9");

	// BGM
	Bgm->PlayWave("Resources/BGM/bgm.wav", 255, 0.08f);
}

void GameScene::Finalize()
{
	// マウスカーソルを表示
	//ShowCursor(true);
} 

void GameScene::Update()
{
	player->Update(map[mapNumber[CsvFile::now_y][CsvFile::now_x]]);
	rope->Update(player->GetHeadPos(), player->GetBodyPos());
	jsonObjectUpdate();

	if (player->GetOnGrounding() == true)
	{
		XMFLOAT3 pos = player->GetBodyPos();
		pos.y -= 1.0f * player->GetBodyObj()->GetScale().y;
		OnLandingEffect(6, pos);
	}

	if (player->GetIsLonger() == true)
	{
		OnBitingEffect(player->GetHeadPos());
	}

	FadeSpanaProcess();
}

void GameScene::Draw()
{
	//ImGui::Begin("config1");//ウィンドウの名前
	//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	//ImGui::Text("Player: %f, %f, %f", player->GetObj()->GetPosition().x, player->GetObj()->GetPosition().y, player->GetObj()->GetPosition().z);
	//ImGui::Text("moveY: %f", player->GetmoveY());
	//ImGui::Text("bodyPos: %f, %f, %f", player->GetBodyPos().x, player->GetBodyPos().y, player->GetBodyPos().z);
	//ImGui::Text("headPos: %f, %f, %f,", player->GetHeadPos().x, player->GetHeadPos().y, player->GetHeadPos().z);
	//ImGui::Text("GetHeadState: %d", player->GetHeadState());
	//ImGui::Checkbox("onGround", &player->GetOnGround());
	//ImGui::Text("mapNum: %d", mapNumber[CsvFile::now_y][CsvFile::now_x] + 1);
	//ImGui::End();

#pragma region 背景画像描画
	// 背景画像描画前処理
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	backGround->Draw();

	// 画像描画後処理
	Image2d::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景画像描画

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// 3Dオブジェクト描画
	rope->Draw();
	player->GetBodyObj()->Draw();
	player->GetHeadObj()->Draw();

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

	fadeSpanaTex->Draw();

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
			GroundMoveTypeUpdate(index, object, object->originPos, div);
		}
		else if (object->object->GetType() == "Box_Move")
		{
			if (player->GetIsReturn() == true)
			{
				object->object->SetPosition(object->originPos);
			}
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
	// 落下するブロックの更新処理
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
	// チェックポイントの処理(当たり判定)
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 scale = object->GetScale();
	XMFLOAT3 pPos = player->GetBodyPos();
	XMFLOAT3 pScale = player->GetBodyObj()->GetScale();

	XMFLOAT3 pScaleHalf = { pScale.x / 2, pScale.y, pScale.z };

	if (Collision::CollisionBoxPoint(pos, scale, pPos, pScaleHalf) && CsvFile::check_change_flag == false)
	{
		copy(mapSave.begin(), mapSave.end(), map.begin());
		CsvFile::check_change_flag = true;
	}
}

void GameScene::KeyTypeUpdate(int& keyIndex, int index, Object3d* object)
{
	// 鍵の当たり判定
	if (IsCanGetKey(object->GetPosition(), player->GetBodyPos(), object->GetScale().x, player->GetBodyObj()->GetScale().x))
	{
		// 鍵取得
		Audio::GetInstance()->PlayWave("Resources/SE/se7.wav", 0, 0.1f);
		OnPickingEffect(object->GetPosition());
		player->SetIKey(true);
		keyIndex = index + 1;
	}
}

void GameScene::DoorTypeUpdate(std::vector<int>& doorIndex, int index, Object3d* object)
{
	// ドアの当たり判定
	if (player->GetIsKey() == true && IsCanOpenDoor(object->GetPosition(), player->GetBodyPos(), object->GetScale(), player->GetBodyObj()->GetScale()))
	{
		// ドアに入る
		Audio::GetInstance()->PlayWave("Resources/SE/se8.wav", 0, 0.1f);
		OnPickingEffect(object->GetPosition());
		player->SetIKey(false);
		doorOpen = true;
	}

	object->SetTiring({ 0.5f, 1.0f });

	if (doorOpen)
	{
		object->SetOffset({ 1.0f, 1.0f });
	}
	else
	{
		object->SetOffset({ 0.5f, 1.0f });
	}
}

void GameScene::GroundMoveTypeUpdate(int index, MapData* mapData, const XMFLOAT3& originPos, int divide)
{
	if (player->GetIsReturn() == true)
	{
		mapData->isMove = false;
		mapData->moveFrame = 0;
		mapData->object->SetPosition(mapData->originPos);
	}

	if (CheckHitGroundMoveType(mapData->object) == true)
	{
		XMFLOAT3 moveVec = { 0, 0, 0 };
		XMFLOAT3 movePos = Easing::lerp(originPos, mapData->object->GetMovePos(), static_cast<float>(mapData->moveFrame) / divide);
		if (player->GetIsHit() == false)
		{
			moveVec = movePos - mapData->object->GetPosition();
		}
 		mapData->object->SetPosition(movePos);

		if (mapData->isMove == false && player->GetOnGround() == false)
		{
			XMFLOAT3 pPos = player->GetBodyObj()->GetPosition();
			pPos.y -= 1.0f * player->GetBodyObj()->GetScale().y;
			OnLandingEffect(6, pPos);
		}
		mapData->isMove = true;
		mapData->moveFrame++;
		mapData->moveFrame = min(mapData->moveFrame, divide);

		player->OnGrounding();
		player->SetPositionPlayer(player->GetBodyPos(), moveVec);
	}
	else
	{
		XMFLOAT3 movePos = Easing::lerp(originPos, mapData->object->GetMovePos(), static_cast<float>(mapData->moveFrame) / divide);
		mapData->object->SetPosition(movePos);

		mapData->isMove = false;
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

bool GameScene::IsCanOpenDoor(const XMFLOAT3& doorPos, const XMFLOAT3& playerPos, const XMFLOAT3& doorRadius, const XMFLOAT3& playerRadius)
{
	// 誤差
	float error = 0.1f;

	// 一定の距離なら
	if (Collision::CollisionBoxToBox(doorPos, doorRadius * 1.5f, playerPos, playerRadius))
	{
		return true;
	}

	return false;
}

bool GameScene::CheckHitGroundMoveType(Object3d* object)
{
	XMFLOAT3 pPos = player->GetBodyPos();
	XMFLOAT3 pScale = player->GetBodyObj()->GetScale();
	XMFLOAT3 oldPPos = player->GetBodyOldPos();
	XMFLOAT3 oPos = object->GetPosition();
	XMFLOAT3 oScale = object->GetScale();
	float lenX = Helper::LengthFloat2({ pPos.x, 0 }, { oPos.x, 0 });
	float lenY = Helper::LengthFloat2({ 0, pPos.y }, { 0, oPos.y });
	bool hit = false;

	if (lenY < pScale.y + oScale.y)
	{
		if (pScale.x + oScale.x <= Helper::LengthFloat2({ oldPPos.x, 0 }, { oPos.x, 0 }))
		{
 			PushBackX(pPos, pScale, oPos, oScale);
		}
	}
	if (lenX < pScale.x + oScale.x)
	{
		PushBackY(pPos, pScale, oPos, oScale, hit);
	}

	return hit;
}

void GameScene::PushBackX(XMFLOAT3& pPos, const XMFLOAT3& pScale, const XMFLOAT3& oPos, const XMFLOAT3& oScale)
{
	if (pPos.x - pScale.x < oPos.x + oScale.x && oPos.x < pPos.x)
	{
		pPos.x = oPos.x + oScale.x + pScale.x;
		player->SetPositionPlayer(pPos);
	}
	else if (oPos.x - oScale.x < pPos.x + pScale.x && pPos.x < oPos.x)
	{
		pPos.x = oPos.x - oScale.x - pScale.x;
		player->SetPositionPlayer(pPos);
	}
}

void GameScene::PushBackY(XMFLOAT3& pPos, const XMFLOAT3& pScale, const XMFLOAT3& oPos, const XMFLOAT3& oScale, bool& hit)
{
	if (pPos.y - pScale.y - 2.5f < oPos.y + oScale.y && oPos.y < pPos.y && player->GetmoveY() <= 0)
	{
		player->OnGrounding();
		pPos.y = oPos.y + oScale.y + pScale.y;
		player->SetPositionPlayer(pPos);
		bool isNearBox = false;
		for (auto& box : map[mapNumber[CsvFile::now_y][CsvFile::now_x]])
		{
			if (box->object->GetType() != "box")
			{
				continue;
			}
			if (Collision::CollisionBoxPoint(oPos, oScale, box->object->GetPosition(), box->object->GetScale()) == false)
			{
				continue;
			}
			isNearBox = true;
			break;
		}
		if (isNearBox == false)
		{
			hit = true;
		}
	}
	else if (oPos.y - oScale.y < pPos.y + pScale.y && pPos.y < oPos.y)
	{
		pPos.y = oPos.y - oScale.y - pScale.y;
		player->SetPositionPlayer(pPos);
	}
}

void GameScene::FadeSpanaProcess()
{
	//体の位置に頭が引き寄せられる
	const float end = 1000.0f;
	const float start = 100.0f;
	const float timeMax = 4.0f;
	static float nowTime = 0.0f;
	float timeRate = min(nowTime / timeMax, 1.0f);	//タイムレート 0.0f->1.0f
	static bool flagA = false;
	static bool flagB = false;
	
	//if (flagA == false && player->GetAcidSinkFlag() == false || player->GetRestartCheckFlag() == false)
	//{
	//	return;
	//}

	if (player->GetAcidSinkFlag() == true || flagA == true)
	{
		flagA = true;
		float num = Easing::lerp(start, end, timeRate);
		if (num >= end)
		{
			fadeSpanaTex->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			nowTime = 0.0f;
			flagA = false;
			return;
		}


		SpanaTexSize = { SpanaTexSize.x + num, SpanaTexSize.y + (num / 2) };
		float flame = 60.0f;
		nowTime += 1.0f / flame;
	}
	else if(player->GetRestartCheckFlag() == true || flagB == true)
	{
		flagB = true;
		float num = Easing::lerp(start, end, timeRate);
		if (num >= end)
		{
			fadeSpanaTex->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			nowTime = 0.0f;
			flagB = false;
			return;
		}

		SpanaTexSize = { SpanaTexSize.x - num, SpanaTexSize.y - (num / 2) };
		float flame = 60.0f;
		nowTime += 1.0f / flame;
	}
	fadeSpanaTex->SetSize(SpanaTexSize);
}
