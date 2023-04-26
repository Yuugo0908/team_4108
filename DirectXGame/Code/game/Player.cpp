#include "Player.h"

bool Player::Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale)
{
	// モデルの生成
	playerModel = playerModel->CreateFromObject("player");
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

void Player::Update(bool rFlag, bool moveFlag)
{
	// カメラが向いている方向を調べる
	cameraTrack = camera->CameraTrack(pPos);
	cameraTrack = cameraTrack * pSpeed;

	pPosOld = playerObj->GetPosition();

	// ダメージを受けた後の処理
	if (damageInterval > 0)
	{
		damageInterval--;
		float alpha = 0.5f;
		playerObj->SetColor({ 1.0f, 0.0f, 0.4f, alpha });
	}
	else
	{
		playerObj->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}


	if (moveFlag)
	{
		if (!rFlag)
		{
			Jump();
			Avoid();
		}

		// 移動
		rate = 1.0f;
		// 移動量の倍数計算
		if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE))
		{
			if (controller->GetPadState(Controller::State::LEFT_U_STICK, Controller::Type::NONE) || controller->GetPadState(Controller::State::LEFT_D_STICK, Controller::Type::NONE))
			{
				rate = sqrtf(1.0f / 2.0f);
			}
		}
		if (keyboard->PushKey(DIK_A) || keyboard->PushKey(DIK_D))
		{
			if (keyboard->PushKey(DIK_W) || keyboard->PushKey(DIK_S))
			{
				rate = sqrtf(1.0f / 2.0f);
			}
		}
		cameraTrack = cameraTrack * pAcc * rate;

		if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
		{
			pPos.x += cameraTrack.z;
			pPos.z -= cameraTrack.x;
		}
		else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
		{
			pPos.x -= cameraTrack.z;
			pPos.z += cameraTrack.x;
		}

		if (controller->GetPadState(Controller::State::LEFT_U_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_W))
		{
			pPos.x += cameraTrack.x;
			pPos.z += cameraTrack.z;
		}
		else if (controller->GetPadState(Controller::State::LEFT_D_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_S))
		{
			pPos.x -= cameraTrack.x;
			pPos.z -= cameraTrack.z;
		}
	}

	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->Update();
}

void Player::Rush(XMFLOAT3 targetPos, bool& flag, float& avoidTime)
{
	// フラグがtrueじゃない場合はリターンする
	if (!flag)
	{
		avoidTime = 0.0f;
		return;
	}

	if (avoidTime < 1.0f)
	{
		avoidTime += 0.1f;
	}

	pPos = Easing::easeIn(pPos, targetPos, avoidTime);
	playerObj->Update();
}

void Player::Avoid()
{
	if (avoidFlag)
	{
		pAcc -= pGra;
	}
	// 自機の突進
	else if (mouse->TriggerMouseRight() || controller->GetPadState(Controller::State::RT, Controller::Type::NONE))
	{
		avoidFlag = true;
		// 加速力の更新
		pAcc = 3.0f;
	}

	if (!avoidFlag)
	{
		pAcc = 1.0f;
	}
	else if (pAcc <= 1.0f)
	{
		avoidFlag = false;
	}
}

void Player::Jump()
{
	if (!jumpFlag && (keyboard->TriggerKey(DIK_SPACE) || controller->GetPadState(Controller::State::A, Controller::Type::TRIGGER)))
	{
		onGround = false;
		jumpFlag = true;
		// 上昇率の更新
		pVel = 1.25f;
	}

	// ジャンプ
	if (jumpFlag)
	{
		pVel -= pGra;
		pPos.y += pVel;
		pPos.x += inertiaSave.x * 0.1f;
		pPos.z += inertiaSave.z * 0.1f;

		if (onGround)
		{
			jumpFlag = false;
			pVel = 0.0f;
			inertiaSave = {};
		}
	}
	// 重力
	else
	{
		pDown = -0.75f;
		pPos.y += pDown;
		if (onGround)
		{
			pDown = 0.0f;
		}
	}
}

bool Player::Damage(const std::unique_ptr<Object3d>& object)
{
	if (damageInterval == 0 && Collision::CollisionObject(object, playerObj))
	{
		damageInterval = 120;
		return true;
	}
	return false;
}

void Player::Reset()
{
	pPos = { 0.0f, 10.0f, 0.0f };//座標
	pPosOld = { 0.0f, 10.0f, 0.0f };
	pRot = { 0.0f, 0.0f, 0.0f };//回転

	playerObj->SetPosition(pPos);
	playerObj->SetScale({ 0.8f,0.8f,0.8f });
	playerObj->SetRotation(pRot);
	playerObj->Update();

	pSpeed = 0.35f;
	onGround = false;//自由落下のフラグ
	jumpFlag = false;
	pMove = 0.0f;//移動量
	pAcc = 0.2f;//加速
	pVel = 0.2f;//速度
	pDown = 0.0f;
	pGra = 0.1f;//重力
	rate = 1.0f; // 斜め移動時の制限

	// 突進用
	avoidFlag = false; // 回避開始フラグ
	avoidCount = 0;

	// カメラ距離取得用
	cameraTrack = {};
	cameraRot = 0.0f;
}

void Player::ReSpawn()
{
	pPos = reSpawnPos;
	playerObj->SetPosition(pPos);
	playerObj->SetScale(pScale);
	playerObj->Update();
}

void Player::TrackRot(const XMFLOAT3& startPos, const XMFLOAT3& endPos)
{
	XMVECTOR sPos = { startPos.x, startPos.y, startPos.z, 0 };
	XMVECTOR ePos = { endPos.x, endPos.y, endPos.z, 0 };

	XMVECTOR subPos = XMVectorSubtract(sPos, ePos);

	float angle = (float)atan2(subPos.m128_f32[0], subPos.m128_f32[2]);

	playerObj->SetRotation({ 0, XMConvertToDegrees(angle), 0 });
}

bool Player::MapCollide(XMFLOAT3 boxPos, XMFLOAT3 boxScale)
{
	//フラグ
	bool hitFlag = false;

	// 判定
	float maxBoxX = boxPos.x + boxScale.x;
	float minBoxX = boxPos.x - boxScale.x;
	float maxBoxY = boxPos.y + boxScale.y;
	float minBoxY = boxPos.y - boxScale.y;
	float maxBoxZ = boxPos.z + boxScale.z;
	float minBoxZ = boxPos.z - boxScale.z;

	if ((pPos.x <= maxBoxX && pPos.x >= minBoxX) &&
		(pPos.z <= maxBoxZ && pPos.z >= minBoxZ))
	{
		if (maxBoxY + pScale.y > pPos.y && boxPos.y < pPosOld.y)
		{
			hitFlag = true;
			if (maxBoxY + pScale.y >= pPos.y)
			{
				pPos.y = pPosOld.y;
			}
			onGround = true;
		}
		else if (minBoxY - pScale.y < pPos.y && boxPos.y > pPosOld.y)
		{
			hitFlag = true;
			if (minBoxY - pScale.y <= pPos.y)
			{
				pPos.y = pPosOld.y;
			}
		}
	}

	if ((pPos.x <= maxBoxX && pPos.x >= minBoxX) &&
		(pPos.y <= maxBoxY && pPos.y >= minBoxY))
	{
		if (maxBoxZ + pScale.z > pPos.z && boxPos.z < pPosOld.z)
		{
			pPos.z = maxBoxZ + pScale.z;
			hitFlag = true;
			avoidFlag = false;
		}
		else if (minBoxZ - pScale.z < pPos.z && boxPos.z > pPosOld.z)
		{
			pPos.z = minBoxZ - pScale.z;
			hitFlag = true;
			avoidFlag = false;
		}
	}

	if ((pPos.z <= maxBoxZ && pPos.z >= minBoxZ) &&
		(pPos.y <= maxBoxY && pPos.y >= minBoxY))
	{
		if (maxBoxX + pScale.x > pPos.x && boxPos.x < pPosOld.x)
		{
 			pPos.x = maxBoxX + pScale.x;
			hitFlag = true;
			avoidFlag = false;
		}
		else if (minBoxX - pScale.x < pPos.x && boxPos.x > pPosOld.x)
		{
			pPos.x = minBoxX - pScale.x;
			hitFlag = true;
			avoidFlag = false;
		}
	}

	playerObj->SetPosition(pPos);
	playerObj->Update();


	return hitFlag;
}

bool Player::StageCollide(XMFLOAT3 stagePos, XMFLOAT3 stageScale, bool& reverseFlag)
{
	//フラグ
	bool hitFlag = false;

	// 判定
	float maxX = stagePos.x + stageScale.x;
	float maxY = stagePos.y + stageScale.y;
	float maxZ = stagePos.z + stageScale.z;
	float minX = stagePos.x - stageScale.x;
	float minY = stagePos.y - stageScale.y;
	float minZ = stagePos.z - stageScale.z;

	if ((pPos.x <= maxX && pPos.x >= minX) &&
		(pPos.z <= maxZ && pPos.z >= minZ))
	{
		if (maxY + pScale.y > pPos.y && stagePos.y < pPosOld.y)
		{
			hitFlag = true;
			if (stagePos.y + pScale.y > pPos.y)
			{
				pPos.y = pPosOld.y;
			}
			onGround = true;
		}

		else if (minY - pScale.y < pPos.y && stagePos.y > pPosOld.y)
		{
			hitFlag = true;
			if (stagePos.y - pScale.y <= pPos.y)
			{
				pPos.y = pPosOld.y;
			}
			reverseFlag = true;
		}
	}

	playerObj->SetPosition(pPos);
	playerObj->Update();

	return hitFlag;
}

bool Player::PoleCollide(XMFLOAT3 polePos, XMFLOAT3 poleScale)
{
	//フラグ
	bool hitFlag = false;

	// 判定
	float maxBoxX = polePos.x + poleScale.x;
	float minBoxX = polePos.x - poleScale.x;
	float maxBoxY = polePos.y + poleScale.y;
	float minBoxY = polePos.y - poleScale.y;
	float maxBoxZ = polePos.z + poleScale.z;
	float minBoxZ = polePos.z - poleScale.z;

	playerObj->SetPosition(pPos);

	if ((pPos.x <= maxBoxX && pPos.x >= minBoxX) &&
		(pPos.z <= maxBoxZ && pPos.z >= minBoxZ))
	{
		if (maxBoxY + pScale.y > pPos.y && polePos.y < pPosOld.y)
		{
			hitFlag = true;
		}
		else if (minBoxY - pScale.y < pPos.y && polePos.y > pPosOld.y)
		{
			hitFlag = true;
		}
	}

	else if ((pPos.x <= maxBoxX && pPos.x >= minBoxX) &&
		(pPos.y <= maxBoxY && pPos.y >= minBoxY))
	{
		if (maxBoxZ + pScale.z > pPos.z && polePos.z < pPosOld.z)
		{
			hitFlag = true;
		}
		else if (minBoxZ - pScale.z < pPos.z && polePos.z > pPosOld.z)
		{
			hitFlag = true;
		}
	}

	else if ((pPos.z <= maxBoxZ && pPos.z >= minBoxZ) &&
		(pPos.y <= maxBoxY && pPos.y >= minBoxY))
	{
		if (maxBoxX + pScale.x > pPos.x && polePos.x < pPosOld.x)
		{
			hitFlag = true;
		}
		else if (minBoxX - pScale.x < pPos.x && polePos.x > pPosOld.x)
		{
			hitFlag = true;
		}
	}

	if (hitFlag)
	{
		inertiaSave = pPos - pPosOld;
		onGround = false;
		jumpFlag = true;
		// 上昇率の更新
		pVel = 1.25f;
	}

	playerObj->SetPosition(pPos);
	playerObj->Update();

	return hitFlag;
}
