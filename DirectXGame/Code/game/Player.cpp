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
	MoveProcess();
	//移動値加算
	GravityProcess();
	pPos = pPos + move;
	hPos = hPos + move;
	GroundCollisionProcess(mapObjects);
	BlockCollisionProcess(mapObjects);

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

	if (headState != STATE_NORMAL) return;

	if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
	{
		bodyState = STATE_BODY_MOVE;
		move.x += 1.0f;
		pDirection = { 1.0f, 0.0f, 0.0f };
	}
	else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
	{
		bodyState = STATE_BODY_MOVE;
		move.x -= 1.0f;
		pDirection = { -1.0f, 0.0f, 0.0f };
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
		moveY = 1.50f;
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
  			pPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().z) - (pPos.y - pScale.z);
			hPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().z) - (hPos.y - pScale.z);
			move.y = 0.0f;
			onGround = true;
			moveY = 0.0f;
			return;
		}
	}

	onGround = false;
}

void Player::BlockCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	//少数補正値
	float correction = 0.01f;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, {pScale.x, 1.0f, 1.0f}) == true)
		{
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
		}
	}
}

void Player::HeadInjectionProcess()
{
	XMFLOAT3 hInjectDis = { 40.0f, 0.0f, 0.0f };
	
	if (keyboard->PushKey(DIK_RETURN))
	{
		headInjectDis = hInjectDis;
		headBackDis = hInjectDis;
		headState = STATE_INJECTION;
	}
}

void Player::HeadInjectionMoveProcess()
{
	float disVal = 0.5f;

	if (headInjectDis.x > 0.0f)
	{
		headInjectDis.x -= disVal;
		hPos.x += pDirection.x * disVal;
	}
	else
	{
		//伸ばしきった時
		headState = STATE_BACK;
	}
}

void Player::HeadBackMoveProcess()
{
	float disVal = 0.5f;

	if (hPos.x < pPos.x)
	{
		headBackDis.x = pPos.x - hPos.x;
	}
	else
	{
		headBackDis.x = hPos.x - pPos.x;
	}

	if (headBackDis.x > 0.0f)
	{
		headBackDis.x -= disVal;
		hPos.x += -pDirection.x * disVal;
	}
	else
	{
		headState = STATE_NORMAL;
	}
}

void Player::HeadBiteProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{

	////タイマー起動
	if (TimeCheck(biteTimer) == true)
	{
		headState = STATE_BACK;
	}

	//噛み離す
	if (keyboard->PushKey(DIK_P))
	{
		headState = STATE_BACK;
	}

	//噛み壊す
	if (keyboard->PushKey(DIK_O))
	{
		mapObjects.erase(mapObjects.begin() + hitMapObjNum);
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
		}
		else if (state == STATE_UNBITEHIT)
		{
			headState = STATE_BACK;
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
	sphereA.radius = 2.0f;
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

bool Player::TimeCheck(float& time)
{
	float flame = 60.0f;
	time -= 1.0f / flame;

	if (time <= 0.0f) return true;
	return false;
}
