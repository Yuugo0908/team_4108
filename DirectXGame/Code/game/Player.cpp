#include "Player.h"
#include "Helper.h"

bool Player::Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale)
{
	// モデルの生成
	playerModel = playerModel->CreateFromObject("box");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerHedObj = Object3d::Create();
	playerHedObj->SetModel(playerModel);

	pPos = pos;
	pScale = scale;
	reSpawnPos = pos;
	hPos = pPos;

	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->SetRotation({ 0.0f, 0.0f, 0.0f });
	playerObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	playerObj->Update();

	playerHedObj->SetPosition(hPos);
	playerHedObj->SetScale(pScale);
	playerHedObj->SetRotation({ 0.0f, 0.0f, 0.0f });
	playerHedObj->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	playerHedObj->Update();

	return true;
}

void Player::Update(std::vector<MapData*>& mapObjects)
{
	//デバック用テレポート
	/*if (keyboard->PushKey(DIK_R))
	{
		pPos = { -100.0f, 120.0f, 0.0f };
		hPos = { -100.0f, 120.0f, 0.0f };
	}*/

	if (keyboard->TriggerKey(DIK_Z))
	{
		ReturnCheckpoint();
	}

	MoveProcess();
	//移動値加算
	GravityProcess();
	pPos = pPos + move;

	AcidProcess(mapObjects);
	CheckPointProcess(mapObjects);
	MapChange(mapObjects);

	isHit = false;
	if (CsvFile::map_change_flag == false)
	{
		BlockCollisionProcess(mapObjects);
		CeilingBlockCollisionProcess(mapObjects);
		GroundCollisionProcess(mapObjects);
	}

	HeadUpdateProcess(mapObjects);

	if (headState == STATE_NORMAL || headState == STATE_INJECTIONLOCK)
	{
		hPos = pPos;
	}

	oldpPos = playerObj->GetPosition();
	//OBJ更新処理
	playerObj->SetPosition(pPos);
	playerObj->Update();
	
	playerHedObj->SetPosition(hPos);
	playerHedObj->Update();
}

void Player::AddMove(const XMFLOAT3& move)
{
	pPos = pPos + move;
	playerObj->SetPosition(pPos);
	playerObj->Update();

	if (headState == STATE_NORMAL)
	{
		hPos = pPos;
		playerHedObj->SetPosition(hPos);
		playerHedObj->Update();
	}
}

void Player::MoveProcess()
{
	//移動値初期化
	move = {};

	//体当たり判定状態初期化
	bodyColState = BODYSTATE_NULL;

	if (headState == STATE_INJECTION) return;
	if (headState == STATE_BITE) return;
	//if (headState == STATE_BACK) return;

	if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
	{
		bodyState = STATE_BODY_MOVE;
		move.x += 0.7f;
		pDirection = { 1.0f, 0.0f, 0.0f };
		direction.x = 1.0f;
	}
	else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
	{
		bodyState = STATE_BODY_MOVE;
		move.x -= 0.7f;
		pDirection = { -1.0f, 0.0f, 0.0f };
		direction.x = -1.0f;
	}
	else
	{
		bodyState = STATE_BODY_NORMAL;
	}

	//ジャンプ処理
	JumpProcess();
}

void Player::JumpProcess()
{
	if (onGround != true) return;

	if (controller->GetPadState(Controller::State::A, Controller::Type::NONE) || keyboard->TriggerKey(DIK_SPACE))
	{
		bodyState = STATE_BODY_JUMP_UP;
		onGround = false;
		moveY = 2.75f;
	}
}

void Player::GravityProcess()
{
	if (headState == STATE_INJECTION) return;
	if (headState == STATE_BITE && biteBlockState == NOTGRAVIT) return;
	
	//下向き加速度
	const float fallAcc = -0.1f;
	const float fallVYMin = -2.0f;

	// 加速
	moveY = max(moveY + fallAcc, fallVYMin);
	
	if (onGround != false) return;
	
	move.y = moveY;

	if (moveY >= 0.0f)
	{
		bodyState = STATE_BODY_JUMP_UP;
	}
	else
	{
		bodyState = STATE_BODY_JUMP_DOWN;
	}
}

void Player::GroundCollisionProcess(std::vector<MapData*>& mapObjects)
{
	oldOnGround = onGround;

	if (bodyState == STATE_BODY_JUMP_UP) return;
	XMFLOAT3 groundPos = { pPos.x, pPos.y - (pScale.y / 2), pPos.z };
	XMFLOAT3 groundSize = { pScale.x, (pScale.y / 2), pScale.z };

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;
		//X軸方向で当たり判定が発生したブロックは処理をしない
		if (bodyColState == BODYSTATE_CEILING_COLISION) return;
		if (bodyColState == BODYSTATE_X_COLISION && colisionBlockNum == i) continue;

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), groundPos, groundSize) == true)
		{
			pPos.y += (mapObjects[i]->object->GetPosition().y + mapObjects[i]->object->GetScale().y) - (pPos.y - pScale.y);
			
			if (headState != STATE_BITE)
			{
				hPos.y += (mapObjects[i]->object->GetPosition().y + mapObjects[i]->object->GetScale().y) - (hPos.y - pScale.y);
			}
			move.y = 0.0f;
			onGround = true;
			moveY = 0.0f;

 			if (headState == STATE_INJECTIONLOCK)
			{
				headState = STATE_NORMAL;
			}
			return;
		}
	}

	onGround = false;
}

void Player::BlockCollisionProcess(std::vector<MapData*>& mapObjects)
{
	if (headState == STATE_BITE) return;

	//少数補正値
	float correction = 0.1f;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), { pPos.x, pPos.y - (pScale.y/2), pPos.z }, { pScale.x, 0.01f, pScale.z }) == true)
		{
			//Y軸用当たり判定ブロック保持
			bodyColState = BODYSTATE_X_COLISION;
			colisionBlockNum = i;

			isHit = true;
			if (move.x < 0.0f)
			{
				pPos.x += (mapObjects[i]->object->GetPosition().x + mapObjects[i]->object->GetScale().x) - (pPos.x - pScale.x) + correction;
				hPos.x += (mapObjects[i]->object->GetPosition().x + mapObjects[i]->object->GetScale().x) - (hPos.x - pScale.x) + correction;
			}
			else if(move.x > 0.0f)
			{
				pPos.x -= (pPos.x + pScale.x) - (mapObjects[i]->object->GetPosition().x - mapObjects[i]->object->GetScale().x) + correction;
				hPos.x -= (hPos.x + pScale.x) - (mapObjects[i]->object->GetPosition().x - mapObjects[i]->object->GetScale().x) + correction;
			}
			else
			{
				if (pPos.x <= mapObjects[i]->object->GetPosition().x)
				{
					pPos.x = mapObjects[i]->object->GetPosition().x - mapObjects[i]->object->GetScale().x - pScale.x;
				}
				else
				{
					pPos.x = mapObjects[i]->object->GetPosition().x + mapObjects[i]->object->GetScale().x + pScale.x;
				}
			}

			return;
		}
	}
}

void Player::CeilingBlockCollisionProcess(std::vector<MapData*>& mapObjects)
{
	if (headState == STATE_BITE) return;	
	//少数補正値
	float correction = 0.1f;
	if (bodyState == STATE_BODY_NORMAL || bodyState == STATE_BODY_MOVE) return;

	XMFLOAT3 headPos = { pPos.x, pPos.y + (pScale.y / 2), pPos.z };
	XMFLOAT3 headScale = { pScale.x, (pScale.y / 2), pScale.z};

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;
		if (pPos.y > mapObjects[i]->object->GetPosition().y) continue;

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), headPos, headScale) == true)
		{
			bodyColState = BODYSTATE_CEILING_COLISION;

			pPos.y -= (pPos.y + pScale.y) - (mapObjects[i]->object->GetPosition().y - mapObjects[i]->object->GetScale().y);
			hPos.y -= (hPos.y + pScale.y) - (mapObjects[i]->object->GetPosition().y - mapObjects[i]->object->GetScale().y);
		}
	}
}

void Player::HeadInjectionProcess()
{
	XMFLOAT3 hInjectDis = { 80.0f, 0.0f, 0.0f };
	
	if (keyboard->TriggerKey(DIK_RETURN))
	{
		hInjectDis.x *= direction.x;
		headInjectDis = hInjectDis + hPos;
		headState = STATE_INJECTION;
	}
}

void Player::HeadInjectionMoveProcess()
{
	float time = timeMax - moveTime;			//加算時間に変化
	float timeRate = min(time / timeMax, 1.0f);	//タイムレート 0.0f->1.0f

	islonger = false;

	if (TimeCheck(moveTime) == true)
	{
		//伸ばしきった時
		moveTime = timeMax;
		headBackDis = hPos;
		headState = STATE_BACK;
		islonger = true;
	}

	hPos = Easing::easeOut(pPos, headInjectDis, timeRate);
}

void Player::HeadBackMoveProcess()
{
	float time = timeMax - moveTime;			//加算時間に変化
	float timeRate = min(time / timeMax, 1.0f);	//タイムレート 0.0f->1.0f
	
	if (TimeCheck(moveTime) == true || pPos == hPos)
	{
		moveTime = timeMax;
		headState = STATE_INJECTIONLOCK;
	}

	hPos = Easing::easeIn( headBackDis, pPos, timeRate);
}

void Player::HeadBiteProcess(std::vector<MapData*>& mapObjects)
{
	//引き寄せられるブロックにかみついた場合
	if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box_pull")
	{
		AttractBiteProcess(mapObjects);
		return;
	}
	else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "Ground_Move")
	{
		biteBlockState = NOTGRAVIT;
		CarryBlockProcess(mapObjects);
		return;
	}
	islonger = false;


	////タイマー起動
	if (TimeCheck(biteTimer) == true)
	{
		headBackDis = hPos;
		headState = STATE_BACK;
	}

	//噛み離す
	if (keyboard->PushKey(DIK_P))
	{
		headBackDis = hPos;
		headState = STATE_BACK;
	}

	//噛み壊せるブロックの場合壊す
	if (keyboard->TriggerKey(DIK_RETURN) && mapObjects[hitHeadMapObjNum]->object->GetType() == "box")
	{
		mapObjects.erase(mapObjects.begin() + hitHeadMapObjNum);
		headBackDis = hPos;
		headState = STATE_BACK;
		islonger = true;
	}
}

void Player::HeadBiteCollisionProcess()
{
	bool collision = false;
	if (collision == true)
	{
		headState = STATE_BITE;
	}
	else
	{
		headBackDis = hPos;
		headState = STATE_BACK;
	}
}

void Player::HeadUpdateProcess(std::vector<MapData*>& mapObjects)
{
	if (headState == STATE_NORMAL)
	{
		HeadInjectionProcess();
	}
	else if (headState == STATE_INJECTION)
	{ 
		HeadInjectionMoveProcess();

		//射出状況
		HeadInjectionState state = HeadCollision(mapObjects);
		if (state == STATE_BITEHIT)
		{
			biteTimer = 5.0f;
			headState = STATE_BITE;
			moveTime = timeMax;
		}
		else if (state == STATE_UNBITEHIT)
		{
			headBackDis = hPos;
			headState = STATE_BACK;
			moveY = 2.0f;
			moveTime = timeMax;
		}
	}
	else if (headState == STATE_BITE)
	{
		HeadBiteProcess(mapObjects);
	}
	else if (headState == STATE_BACK)
	{
		HeadBackMoveProcess();
	}
}

Player::HeadInjectionState Player::HeadCollision(std::vector<MapData*>& mapObjects)
{
	//ブロックとの当たり判定
	if (HeadBlockCollisionCheck(mapObjects) == true)
	{
		//当たっているブロックは噛みつけるか
		if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box")			//壊せるブロック
		{
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box_pull")	//引き寄せられるブロック
		{
			pPosMovePrevious = pPos;
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "Ground_Move")	//引っ張れるブロック
		{
			if (mapObjects[hitHeadMapObjNum]->isMove == false)
			{
				pPosMovePrevious = hPos;
				return STATE_BITEHIT;
			}
			else
			{
				return STATE_MOVE;
			}
		}
		else
		{
			return STATE_UNBITEHIT;
		}

	}
	else
	{
		return STATE_MOVE;
	}
}

bool Player::HeadBlockCollisionCheck(std::vector<MapData*>& mapObjects)
{
	Collision::Sphere sphereA, sphereB;
	sphereA.center = XMLoadFloat3(&hPos);
	sphereA.radius = 2.5f;
	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;
		sphereB.center = XMLoadFloat3(&mapObjects[i]->object->GetPosition());
		sphereB.radius = 2.5f;
		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), hPos, {pScale.x - 0.3f, pScale.y - 0.3f, pScale.z - 0.3f}) == true)
		{
			hitHeadMapObjNum = i;
			return true;
		}
	}

	return false;
}

bool Player::BodyBlockCollisionCheck(std::vector<MapData*>& mapObjects)
{
	XMFLOAT3 pScaleXHalf = { pScale.x / 2, pScale.y, pScale.z };

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), pPos, pScaleXHalf) == true)
		{
			if (mapObjects[i]->object->GetType() == "sprite") continue;
			hitbodyMapObjNum = i;
			return true;
		}
	}

	return false;
}

void Player::MapChange(std::vector<MapData*>& mapObjects)
{
	if (limitPos != NONE)
	{
		CsvFile::map_change_flag = false;
		limitPos = NONE;
		return;
	}
	else
	{
		if (pPos.x >= 160.0f)
		{
			limitPos = RIGHT_LIMIT;
			CsvFile::map_change_flag = true;
		}
		else if (pPos.x <= -160.0f)
		{
			limitPos = LEFT_LIMIT;
			CsvFile::map_change_flag = true;
		}
	}

	if (limitPos == RIGHT_LIMIT)
	{
		pPos.x = -159.0f;
		hPos = pPos;
		CsvFile::now_x++;
	}
	else if (limitPos == LEFT_LIMIT)
	{
		pPos.x = 159.0f;
		hPos = pPos;
		CsvFile::now_x--;
	}
}

void Player::AcidProcess(std::vector<MapData*>& mapObjects)
{
	if (BodyBlockCollisionCheck(mapObjects) == true)
	{
		//当たったブロックが酸ブロックか判定
		if (mapObjects[hitbodyMapObjNum]->object->GetType() == "Acid")
		{
			ReturnCheckpoint();
		}
	}
}

void Player::ReturnCheckpoint()
{
	// チェックポイントに戻る
	pPos = { CsvFile::check_pos.x, CsvFile::check_pos.y + 20.0f, CsvFile::check_pos.z };
	hPos = pPos;
	CsvFile::now_x = CsvFile::check_x;
	CsvFile::now_y = CsvFile::check_y;
	isKey = false;
	headState = STATE_NORMAL;
}

void Player::CheckPointProcess(std::vector<MapData*>& mapObjects)
{
	for (int i = 0; i < mapObjects.size(); i++)
	{
		//当たったブロックがチェックポイントか判定
		if (mapObjects[i]->object->GetType() == "checkPoint")
		{
			XMFLOAT3 pos = mapObjects[i]->object->GetPosition();
			XMFLOAT3 scale = mapObjects[i]->object->GetScale();
			// タイリングを設定
			mapObjects[i]->object->SetTiring({ 0.5f, 1.0f });

			XMFLOAT3 pScaleXHalf = { pScale.x / 2, pScale.y, pScale.z };

			if (Collision::CollisionBoxPoint(pos, scale, pPos, pScaleXHalf))
			{
				// 新しいチェックポイントに触れたら
				if (CsvFile::now_x != CsvFile::check_x || CsvFile::now_y != CsvFile::check_y || CsvFile::check_pos != mapObjects[i]->object->GetPosition())
				{
					// チェックポイントの座標とマップ番号を保存
					CsvFile::check_pos = mapObjects[i]->object->GetPosition();
					CsvFile::check_x = CsvFile::now_x;
					CsvFile::check_y = CsvFile::now_y;

					CsvFile::check_change_flag = true;
				}
			}
			else
			{
				CsvFile::check_change_flag = false;
			}

			// チェックポイントのオフセットの切り替え(テクスチャ)
			if (CsvFile::now_x == CsvFile::check_x && CsvFile::now_y == CsvFile::check_y && CsvFile::check_pos == mapObjects[i]->object->GetPosition())
			{
				mapObjects[i]->object->SetOffset({ 1.0f, 1.0f });
			}
			else
			{
				mapObjects[i]->object->SetOffset({ 0.5f, 1.0f });
			}
		}
	}
}

void Player::AttractBiteProcess(std::vector<MapData*>& mapObjects)
{
	static XMFLOAT3 oldPPos = {};

	//頭の位置に体が引き寄せられる
	float time = timeMax - moveTime;			//加算時間に変化
	float timeRate = min(time / timeMax, 1.0f);	//タイムレート 0.0f->1.0f

	if (TimeCheck(moveTime) == true)
	{
		//伸ばしきった時
		pPos = oldPPos;
		hPos = oldPPos;
		moveY = 2.4f;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		return;
	}

	if (Collision::CollisionBoxPoint(mapObjects[hitHeadMapObjNum]->object->GetPosition(), mapObjects[hitHeadMapObjNum]->object->GetScale(), pPos, pScale) == true)
	{
		pPos = oldPPos;
		hPos = oldPPos;
		moveY = 2.4f;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		return;
	}
	
	oldPPos = pPos;
	pPos = Easing::easeOut(pPosMovePrevious, hPos, timeRate);
	//頭の位置まで行ったら通常状態に戻る

}

void Player::CarryBlockProcess(std::vector<MapData*>& mapObjects)
{
	static XMFLOAT3 oldHPos = {};
	//体の位置に頭が引き寄せられる
	float time = timeMax - moveTime;			//加算時間に変化
	float timeRate = min(time / timeMax, 1.0f);	//タイムレート 0.0f->1.0f

	if (TimeCheck(moveTime) == true)
	{
		//体の位置に戻った時
		pPos = oldHPos;
		hPos = oldHPos;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		biteBlockState = NOTBITE;
		return;
	}

	if (Collision::CollisionBoxPoint(mapObjects[hitHeadMapObjNum]->object->GetPosition(), mapObjects[hitHeadMapObjNum]->object->GetScale(), pPos, pScale) == true)
	{
		pPos = oldHPos;
		hPos = oldHPos;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		return;
	}

	oldHPos = hPos;
	hPos = Easing::easeOut(pPosMovePrevious, pPos, timeRate);

	XMFLOAT3 move = oldHPos - hPos;
	//ブロック移動処理
	XMFLOAT3 mapPos = mapObjects[hitHeadMapObjNum]->object->GetPosition();
	mapPos = mapPos - move;
	mapObjects[hitHeadMapObjNum]->object->SetPosition(mapPos);

}

bool Player::TimeCheck(float& time)
{
	float flame = 60.0f;
	time -= 1.0f / flame;

	if (time <= 0.0f) return true;
	return false;
}

bool Player::GetNotGravityFlag()
{
	if (biteBlockState != NOTGRAVIT) return false;
	if (headState != STATE_BITE) return false;
	return true;
}
