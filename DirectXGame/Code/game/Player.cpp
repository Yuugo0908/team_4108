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
	hPos = pos;
	hPos.y += 1.0f;

	pScale = scale;
	reSpawnPos = pos;

	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	playerObj->Update();

	playerHedObj->SetPosition(hPos);
	playerHedObj->SetScale(pScale);
	playerHedObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	playerHedObj->Update();

	return true;
}

void Player::Update()
{
	MoveProcess();
	GravityProcess();
	//移動値加算
	pPos = pPos + move;
	hPos = hPos + move;
	
	GroundCollisionProcess();

	HeadUpdateProcess();

	//OBJ更新処理
	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->Update();

	playerHedObj->SetPosition(hPos);
	playerHedObj->Update();
}

void Player::MoveProcess()
{
	if (headState != STATE_NORMAL) return;

	//移動値初期化
	move = {};

	if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
	{
		move.x += 1.0f;
		pDirection = { 1.0f, 0.0f, 0.0f };
	}
	else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
	{
		move.x -= 1.0f;
		pDirection = { -1.0f, 0.0f, 0.0f };
	}

	//ジャンプ処理
	JumpProcess();
}

void Player::JumpProcess()
{
	if (onGround != true) return;

	if (keyboard->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		moveY = 1.25;
	}
}

void Player::GravityProcess()
{
	if (onGround != false) return;
	//下向き加速度
	const float fallAcc = -0.1f;
	const float fallVYMin = -0.5f;
	//加速
	moveY = max(moveY + fallAcc, fallVYMin);
	move.y = moveY;
}

void Player::GroundCollisionProcess()
{
	oldOnGround = onGround;

	if (pPos.y > 0.0f) return;

	pPos.y = 0.0f;
	onGround = true;
	moveY = 0.0f;
}

void Player::HeadInjectionProcess()
{
	XMFLOAT3 hInjectDis = { 15.0f, 0.0f, 0.0f };
	
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
		HeadBiteCollisionProcess();
	}
}

void Player::HeadBackMoveProcess()
{
	float disVal = 0.5f;

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

void Player::HeadBiteProcess()
{
	float timer = 5.0f;
	//タイマー起動
	if (timer > 0.0f) return;

	headState = STATE_BACK;

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

void Player::HeadUpdateProcess()
{
	if (headState == STATE_NORMAL)
	{
		HeadInjectionProcess();
	}
	else if (headState == STATE_INJECTION)
	{ 
		HeadInjectionMoveProcess();
	}
	else if (headState == STATE_BITE)
	{
		HeadBiteProcess();
	}
	else if (headState == STATE_BACK)
	{
		HeadBackMoveProcess();
	}
}
