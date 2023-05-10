#include "Player.h"

bool Player::Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale)
{
	// ���f���̐���
	playerModel = playerModel->CreateFromObject("box");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerHedObj = Object3d::Create();
	playerHedObj->SetModel(playerModel);

	pPos = pos;
	pScale = scale;
	reSpawnPos = pos;
	objPos = pPos;
	objPos.y += pScale.z; //obj���f������

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
	//�ړ��l���Z
	GravityProcess();
	pPos = pPos + move;
	hPos = hPos + move;
	GroundCollisionProcess(mapObjects);
	

	HeadUpdateProcess(mapObjects);

	oldpPos = playerObj->GetPosition();
	//OBJ�X�V����
	playerObj->SetPosition(pPos);
	playerObj->Update();
	
	playerHedObj->SetPosition(hPos);
	playerHedObj->Update();
}

void Player::MoveProcess()
{
	//�ړ��l������
	move = {};

	if (headState != STATE_NORMAL) return;

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

	//�W�����v����
	JumpProcess();
}

void Player::JumpProcess()
{
	if (onGround != true) return;

	if (controller->GetPadState(Controller::State::A, Controller::Type::NONE) || keyboard->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		moveY = 2.25;
	}
}

void Player::GravityProcess()
{
	if (onGround != false) return;
	//�����������x
	const float fallAcc = -0.1f;
	const float fallVYMin = -2.0f;
	//����
	moveY = max(moveY + fallAcc, fallVYMin);
	move.y = moveY;
}

void Player::GroundCollisionProcess(std::vector<std::unique_ptr<Object3d>>& mapObjects)
{
	oldOnGround = onGround;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i].get()->GetPosition(), mapObjects[i].get()->GetScale(), pPos, pScale) == true)
		{
			pPos.y += (mapObjects[i].get()->GetPosition().y + mapObjects[i].get()->GetScale().z) - (pPos.y - pScale.z);
			move.y = 0.0f;
			onGround = true;
			moveY = 0.0f;
			return;
		}
	}

	onGround = false;
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
		//�L�΂���������
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

	////�^�C�}�[�N��
	if (TimeCheck(biteTimer) == true)
	{
		headState = STATE_BACK;
	}

	//���ݗ���
	if (keyboard->PushKey(DIK_P))
	{
		headState = STATE_BACK;
	}

	//���݉�
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

		//�ˏo��
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
	//�u���b�N�Ƃ̓����蔻��
	if (HradBlockCollisionCheck(mapObjects) == true)
	{
		//�������Ă���u���b�N�͊��݂��邩
		if (mapObjects[hitMapObjNum].get()->GetType() == "Ground")
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
