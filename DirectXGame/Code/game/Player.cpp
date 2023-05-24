#include "Player.h"

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
	playerObj->SetRotation({ 270.0f, 0.0f, 0.0f });
	playerObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	playerObj->Update();

	playerHedObj->SetPosition(hPos);
	playerHedObj->SetScale(pScale);
	playerHedObj->SetRotation({ 270.0f, 0.0f, 0.0f });
	playerHedObj->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	playerHedObj->Update();

	return true;
}

void Player::Update(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//デバック用テレポート
	if (keyboard->PushKey(DIK_R))
	{
		pPos = { 0.0f, 10.0f, 0.0f };
		hPos = { 0.0f, 10.0f, 0.0f };
	}


	MoveProcess();
	//移動値加算
	GravityProcess();
	pPos = pPos + move;
	hPos = hPos + hmove;

	AcidProcess(mapObjects);

	MapChange(mapObjects);

	if (mapChangeFlag == false)
	{
		BlockCollisionProcess(mapObjects);
		CeilingBlockCollisionProcess(mapObjects);
		GroundCollisionProcess(mapObjects);
	}

	HeadUpdateProcess(mapObjects);

	oldpPos = playerObj->GetPosition();
	//OBJ更新処理
	playerObj->SetPosition(pPos);
	playerObj->Update();
	
	playerHedObj->SetPosition(hPos);
	playerHedObj->Update();
}

void Player::MoveProcess()
{
	//移動値初期化
	move = {};
	hmove = {};

	//体当たり判定状態初期化
	bodyColState = BODYSTATE_NULL;

	if (headState == STATE_INJECTION) return;
	if (headState == STATE_BITE) return;

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

	hmove.x = move.x;
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

	if (headState != STATE_BITE)
	{
		hmove.y = move.y;
	}
	else
	{
		hmove.y = 0.0f;
	}

}

void Player::GroundCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	oldOnGround = onGround;

	if (bodyState == STATE_BODY_JUMP_UP) return;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, pScale) == true)
		{
			//X軸方向で当たり判定が発生したブロックは処理をしない
			if (bodyColState == BODYSTATE_CEILING_COLISION) return;
			if (bodyColState == BODYSTATE_X_COLISION && colisionBlockNum == i) continue;

			pPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().y) - (pPos.y - pScale.z);
			
			if (headState != STATE_BITE)
			{
				hPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().y) - (hPos.y - pScale.z);

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

void Player::BlockCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//少数補正値
	float correction = 0.1f;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, {pScale.x, 1.0f, 1.0f}) == true)
		{
			//Y軸用当たり判定ブロック保持
			bodyColState = BODYSTATE_X_COLISION;
			colisionBlockNum = i;

			if (move.x <= 0.0f)
			{
				pPos.x += (mapObjects[i].get()->GetPosition().x + mapObjects[i].get()->GetScale().x) - (pPos.x - pScale.x) + correction;
				hPos.x += (mapObjects[i].get()->GetPosition().x + mapObjects[i].get()->GetScale().x) - (hPos.x - pScale.x) + correction;
			}
			else if(move.x > 0.0f)
			{
				pPos.x -= (pPos.x + pScale.x) - (mapObjects[i].get()->GetPosition().x - mapObjects[i].get()->GetScale().x) + correction;
				hPos.x -= (hPos.x + pScale.x) - (mapObjects[i].get()->GetPosition().x - mapObjects[i].get()->GetScale().x) + correction;
			}
			return;
		}
	}
}

void Player::CeilingBlockCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//少数補正値
	float correction = 0.1f;
	if (bodyState == STATE_BODY_NORMAL || bodyState == STATE_BODY_MOVE) return;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, pScale) == true)
		{
			if (pPos.y > mapObjects[i].get()->GetPosition().y) continue;

			bodyColState = BODYSTATE_CEILING_COLISION;

			pPos.y -= (pPos.y + pScale.z) - (mapObjects[i].get()->GetPosition().y - mapObjects[i].get()->GetScale().y);
			hPos.y -= (hPos.y + pScale.z) - (mapObjects[i].get()->GetPosition().y - mapObjects[i].get()->GetScale().y);
		}
	}
}

void Player::HeadInjectionProcess()
{
	XMFLOAT3 hInjectDis = { 80.0f, 0.0f, 0.0f };
	
	if (keyboard->PushKey(DIK_RETURN))
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
	
	if (TimeCheck(moveTime) == true)
	{
		moveTime = timeMax;
		headState = STATE_INJECTIONLOCK;
	}

	hPos = Easing::easeIn( headBackDis, pPos, timeRate);
}

void Player::HeadBiteProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//引き寄せられるブロックにかみついた場合
	if (mapObjects[hitHeadMapObjNum].get()->GetType() == "Ground_MoveA")
	{
		AttractBiteProcess(mapObjects);
		return;
	}
	else if (mapObjects[hitHeadMapObjNum].get()->GetType() == "Ground_Move")
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
	if (keyboard->PushKey(DIK_O) && mapObjects[hitHeadMapObjNum].get()->GetType() == "box")
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

void Player::HeadUpdateProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
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

Player::HeadInjectionState Player::HeadCollision(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//ブロックとの当たり判定
	if (HeadBlockCollisionCheck(mapObjects) == true)
	{
		//当たっているブロックは噛みつけるか
		if (mapObjects[hitHeadMapObjNum].get()->GetType() == "box")			//壊せるブロック
		{
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum].get()->GetType() == "Ground_MoveA")	//引き寄せられるブロック
		{
			pPosMovePrevious = pPos;
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum].get()->GetType() == "Ground_Move")	//引っ張れるブロック
		{
			pPosMovePrevious = hPos;
			return STATE_BITEHIT;
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

bool Player::HeadBlockCollisionCheck(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	Collision::Sphere sphereA, sphereB;
	sphereA.center = XMLoadFloat3(&hPos);
	sphereA.radius = 2.5f;
	for (int i = 0; i < mapObjects.size(); i++)
	{
		sphereB.center = XMLoadFloat3(&mapObjects[i].get()->GetPosition());
		sphereB.radius = 2.5f;
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), hPos, {pScale.x - 0.3f, pScale.y - 0.3f, pScale.z - 0.3f}) == true)
		{
			hitHeadMapObjNum = i;
			return true;
		}
	}

	return false;
}

bool Player::BodyBlockCollisionCheck(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, pScale) == true)
		{
			hitbodyMapObjNum = i;
			return true;
		}
	}

	return false;
}

void Player::MapChange(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i].get()->GetType() == "checkPoint")
		{
			checkPointPos = { mapObjects[i].get()->GetPosition().x, 20.0f, mapObjects[i].get()->GetPosition().z };
			break;
		}
	}

	if (limitPos != NONE)
	{
		limitPos = NONE;
		mapChangeFlag = false;
		return;
	}
	else
	{
		if (pPos.y >= 160.0f)
		{
			limitPos = UP_LIMIT;
			mapChangeFlag = true;
		}
		else if (pPos.y <= 0.0f)
		{
			limitPos = DOWN_LIMIT;
		}

		if (pPos.x >= 160.0f)
		{
			limitPos = RIGHT_LIMIT;
			mapChangeFlag = true;
		}
		else if (pPos.x <= -160.0f)
		{
			limitPos = LEFT_LIMIT;
			mapChangeFlag = true;
		}
	}

	if (limitPos == UP_LIMIT)
	{
		pPos.y = 1.0f;
		hPos = pPos;
		CsvFile::now_y--;
	}
	else if (limitPos == DOWN_LIMIT)
	{
		pPos.y = 159.0f;
		hPos = pPos;
		CsvFile::now_y++;
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

void Player::AcidProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	if (BodyBlockCollisionCheck(mapObjects) == true)
	{
		//当たったブロックが酸ブロックか判定
		if (mapObjects[hitbodyMapObjNum].get()->GetType() == "Acid")
		{
			pPos = { 0.0f, 10.0f, 0.0f };
			hPos = { 0.0f, 10.0f, 0.0f };
		}
	}

}

void Player::AttractBiteProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
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

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, pScale) == true)
		{
			pPos = oldPPos;
			hPos = oldPPos;
			moveY = 2.4f;
			moveTime = timeMax;
			headState = STATE_NORMAL;
			return;
		}
	}
	
	oldPPos = pPos;
	pPos = Easing::easeOut(pPosMovePrevious, hPos, timeRate);
	//頭の位置まで行ったら通常状態に戻る

}

void Player::CarryBlockProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//static XMFLOAT3 oldHPos = {};
	////頭の位置に体が引き寄せられる
	//float time = timeMax - moveTime;			//加算時間に変化
	//float timeRate = min(time / timeMax, 1.0f);	//タイムレート 0.0f->1.0f

	//if (TimeCheck(moveTime) == true)
	//{
	//	//体の位置に戻った時
	//	pPos = oldHPos;
	//	hPos = oldHPos;
	//	moveTime = timeMax;
	//	headState = STATE_NORMAL;
	//	biteBlockState = NOTBITE;
	//	return;
	//}

	//oldHPos = hPos;
	//hPos = Easing::easeOut(pPosMovePrevious, pPos, timeRate);

	//XMFLOAT3 move = oldHPos - hPos;
	////ブロック移動処理
	//XMFLOAT3 mapPos = mapObjects[hitHeadMapObjNum].get()->GetPosition();
	//mapPos = mapPos - move;
	//mapObjects[hitHeadMapObjNum].get()->SetPosition(mapPos);
	headState = STATE_BACK;

}

bool Player::TimeCheck(float& time)
{
	float flame = 60.0f;
	time -= 1.0f / flame;

	if (time <= 0.0f) return true;
	return false;
}
