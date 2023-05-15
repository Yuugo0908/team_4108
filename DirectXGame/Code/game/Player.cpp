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
	playerHedObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
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

	MapChange(mapObjects);

	MoveProcess();
	//移動値加算
	GravityProcess();
	pPos = pPos + move;
	hPos = hPos + move;

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
}

void Player::JumpProcess()
{
	if (onGround != true) return;

	if (keyboard->TriggerKey(DIK_SPACE))
	{
		bodyState = STATE_BODY_JUMP_UP;
		onGround = false;
		moveY = 2.75f;
	}
}

void Player::GravityProcess()
{
	if (headState == STATE_INJECTION) return;

	if (onGround != false) return;
	//下向き加速度
	const float fallAcc = -0.1f;
	const float fallVYMin = -2.0f;
	//加速
	moveY = max(moveY + fallAcc, fallVYMin);
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
			hPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().y) - (hPos.y - pScale.z);
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

	if (TimeCheck(moveTime) == true)
	{
		//伸ばしきった時
		moveTime = timeMax;
		headBackDis = hPos;
		headState = STATE_BACK;
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

	//噛み壊す
	if (keyboard->PushKey(DIK_O))
	{
		mapObjects.erase(mapObjects.begin() + hitMapObjNum);
		headBackDis = hPos;
		headState = STATE_BACK;
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
	if (HradBlockCollisionCheck(mapObjects) == true)
	{
		//当たっているブロックは噛みつけるか
		if (mapObjects[hitMapObjNum].get()->GetType() == "box")
		{
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

bool Player::HradBlockCollisionCheck(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	Collision::Sphere sphereA, sphereB;
	sphereA.center = XMLoadFloat3(&hPos);
	sphereA.radius = 2.5f;
	for (int i = 0; i < mapObjects.size(); i++)
	{
		sphereB.center = XMLoadFloat3(&mapObjects[i].get()->GetPosition());
		sphereB.radius = 2.5f;
		if (Collision::CollisionSphere(sphereA, sphereB) == true)
		{
			hitMapObjNum = i;
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
		else if (pPos.y <= -10.0f)
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
		pPos.y = 0.0f;
		hPos = pPos;
	}
	else if (limitPos == DOWN_LIMIT)
	{
		pPos = checkPointPos;
		hPos = pPos;
	}

	if (limitPos == RIGHT_LIMIT)
	{
		pPos.x = -159.0f;
		hPos = pPos;
	}
	else if (limitPos == LEFT_LIMIT)
	{
		pPos.x = 159.0f;
		hPos = pPos;
	}
}

bool Player::TimeCheck(float& time)
{
	float flame = 60.0f;
	time -= 1.0f / flame;

	if (time <= 0.0f) return true;
	return false;
}
