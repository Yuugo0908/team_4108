#include "Player.h"

bool Player::Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale)
{
	// モデルの生成
	playerModel = playerModel->CreateFromObject("box");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);

	pPos = pos;
	pScale = scale;
	reSpawnPos = pos;

	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	playerObj->Update();

	return true;
}

void Player::Update()
{
	MoveProcess();
	GravityProcess();
	//移動値加算
	pPos = pPos + move;

	//地面当たり判定処理(仮置き)
	GroundCollisionProcess();



	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->Update();
}

void Player::MoveProcess()
{
	//移動値初期化
	move = {};

	if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
	{
		move.x += 1.0f;
	}
	else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
	{
		move.x -= 1.0f;
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
