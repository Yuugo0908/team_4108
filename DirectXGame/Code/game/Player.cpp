#include "Player.h"
#include "Helper.h"

bool Player::Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale)
{
	// ���f���̐���
	playerBodyModel = playerBodyModel->CreateFromObject("playerBody");
	playerBodyObj = Object3d::Create();
	playerBodyObj->SetModel(playerBodyModel);

	playerHeadModel = playerHeadModel->CreateFromObject("playerHead");
	playerHeadObj = Object3d::Create();
	playerHeadObj->SetModel(playerHeadModel);

	pPos = pos;
	pScale = scale;
	reSpawnPos = pos;
	hPos = pPos;

	playerBodyObj->SetPosition(pPos);
	playerBodyObj->SetScale(pScale);
	playerBodyObj->SetRotation({ 0.0f, 0.0f, 0.0f });
	playerBodyObj->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	playerBodyObj->SetTiring({ 0.25f, 1.0f });
	playerBodyObj->SetOffset({ 0.25f, 1.0f });
	playerBodyObj->Update();

	playerHeadObj->SetPosition(hPos);
	playerHeadObj->SetScale({ 4.0f, 4.0f, 1.0f });
	playerHeadObj->SetRotation({ 0.0f, 0.0f, 0.0f });
	playerHeadObj->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	playerHeadObj->SetTiring({ 0.5f, 1.0f });
	playerHeadObj->SetOffset({ 1.0f, 1.0f });
	playerHeadObj->Update();

	return true;
}

void Player::Update(std::vector<MapData*>& mapObjects)
{
	isReturn = false;

	MoveProcess();
	//�ړ��l���Z
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

	oldpPos = playerBodyObj->GetPosition();
	//OBJ�X�V����
	playerBodyObj->SetPosition(pPos);
	playerBodyObj->Update();
	
	playerHeadObj->SetPosition(hPos);
	playerHeadObj->Update();
}

void Player::SetPositionPlayer(const XMFLOAT3& pos, const XMFLOAT3& move)
{
	pPos = pos + move;
	playerBodyObj->SetPosition(pPos);
	playerBodyObj->Update();

	if (headState == STATE_NORMAL)
	{
		playerHeadObj->SetPosition(pPos);
		playerHeadObj->Update();
	}
}

void Player::OnGrounding()
{
	move.y = 0.0f;
	onGround = true;
	jumpParameter = 0.0f;
}

void Player::MoveProcess()
{
	//�ړ��l������
	move = {};

	//�̓����蔻���ԏ�����
	bodyColState = BODYSTATE_NULL;

	// ���݂̃v���C���[�̃I�t�Z�b�g���擾
	XMFLOAT2 nowOffset = playerBodyObj->GetOffset();
	if (headState == STATE_INJECTION || headState == STATE_BITE)
	{
		if (nowOffset.x == rightOffsetBody.x)
		{
			playerBodyObj->SetOffset(rightOffsetBodyInjection);
		}
		else if (nowOffset.x == leftOffsetBody.x)
		{
			playerBodyObj->SetOffset(leftOffsetBodyInjection);
		}
		return;
	}

	if (pPos == hPos)
	{
		if (nowOffset.x == rightOffsetBodyInjection.x)
		{
			playerHeadObj->SetOffset(rightOffsetHead);
			playerBodyObj->SetOffset(rightOffsetBody);
		}
		else if (nowOffset.x == leftOffsetBodyInjection.x)
		{
			playerHeadObj->SetOffset(leftOffsetHead);
			playerBodyObj->SetOffset(leftOffsetBody);
		}
	}

	if (controller->GetPadState(Controller::State::LEFT_R_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_D))
	{
		bodyState = STATE_BODY_MOVE;
		move.x += 0.7f;
		pDirection = { 1.0f, 0.0f, 0.0f };
		direction.x = 1.0f;
		if (headState != STATE_BACK)
		{
			playerHeadObj->SetOffset(rightOffsetHead);
			playerBodyObj->SetOffset(rightOffsetBody);
		}
	}
	else if (controller->GetPadState(Controller::State::LEFT_L_STICK, Controller::Type::NONE) || keyboard->PushKey(DIK_A))
	{
		bodyState = STATE_BODY_MOVE;
		move.x -= 0.7f;
		pDirection = { -1.0f, 0.0f, 0.0f };
		direction.x = -1.0f;
		if (headState != STATE_BACK)
		{
			playerHeadObj->SetOffset(leftOffsetHead);
			playerBodyObj->SetOffset(leftOffsetBody);
		}
	}
	else
	{
		bodyState = STATE_BODY_NORMAL;
	}

	//�W�����v����
	JumpProcess();
}

void Player::JumpProcess()
{
	if (onGround != true) return;

	if (controller->GetPadState(Controller::State::A, Controller::Type::NONE) || keyboard->TriggerKey(DIK_SPACE))
	{
		// �W�����v
		Audio::GetInstance()->PlayWave("Resources/SE/se2.wav", 0, 0.1f);
		bodyState = STATE_BODY_JUMP_UP;
		onGround = false;
		jumpParameter = 2.75f;
	}
}

void Player::GravityProcess()
{
	if (headState == STATE_INJECTION) return;
	if (headState == STATE_BITE && biteBlockState == NOTGRAVIT) return;
	//�����������x
	const float fallAcc = -0.1f;
	const float fallVYMin = -2.0f;

	// ����
	jumpParameter = max(jumpParameter + fallAcc, fallVYMin);
	
	if (onGround != false) return;
	
	move.y = jumpParameter;

	if (jumpParameter >= 0.0f)
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
	XMFLOAT3 playerPos = { pPos.x, pPos.y - (pScale.y / 2), pPos.z };
	XMFLOAT3 playerSize = { pScale.x, (pScale.y / 2), pScale.z };

	// X�������œ����蔻�肪���������u���b�N�͏��������Ȃ�
	if (bodyColState == BODYSTATE_CEILING_COLISION) return;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (bodyColState == BODYSTATE_X_COLISION && colisionBlockNum == i) continue;
		if (mapObjects[i]->object->GetType() == "sprite") continue;
		if (mapObjects[i]->object->GetType() == "Ground_Move")
		{
			if (headState == STATE_INJECTIONLOCK)
			{
				headState = STATE_NORMAL;
			}
			continue;
		}

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), playerPos, playerSize) == true)
		{
			pPos.y += (mapObjects[i]->object->GetPosition().y + mapObjects[i]->object->GetScale().y) - (pPos.y - pScale.y);

			if (headState != STATE_BITE)
			{
				hPos.y += (mapObjects[i]->object->GetPosition().y + mapObjects[i]->object->GetScale().y) - (hPos.y - pScale.y);
			}
			OnGrounding();

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

	// �����␳�l
	float correction = 0.1f;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), { pPos.x, pPos.y - (pScale.y/2), pPos.z }, { pScale.x, 0.01f, pScale.z }) == true)
		{
			// Y���p�����蔻��u���b�N�ێ�
			bodyColState = BODYSTATE_X_COLISION;
			colisionBlockNum = i;

			isHit = true;
			if (move.x < 0.0f)
			{
				pPos.x += (mapObjects[i]->object->GetPosition().x + mapObjects[i]->object->GetScale().x) - (pPos.x - pScale.x) + correction;
				hPos.x += (mapObjects[i]->object->GetPosition().x + mapObjects[i]->object->GetScale().x) - (hPos.x - pScale.x) + correction;
			}
			else if (move.x > 0.0f)
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
	//�����␳�l
	float correction = 0.1f;
	if (bodyState == STATE_BODY_NORMAL || bodyState == STATE_BODY_MOVE) return;

	XMFLOAT3 headPos = { pPos.x, pPos.y + (pScale.y / 2), pPos.z };
	XMFLOAT3 headScale = { pScale.x, (pScale.y / 2), pScale.z};

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (mapObjects[i]->object->GetType() == "sprite") continue;
		if (pPos.y > mapObjects[i]->object->GetPosition().y) continue;
		if (mapObjects[i]->object->GetType() == "Ground_Move") continue;

		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), headPos, headScale) == true)
		{
			bodyColState = BODYSTATE_CEILING_COLISION;

			pPos.y -= (pPos.y + pScale.y) - (mapObjects[i]->object->GetPosition().y - mapObjects[i]->object->GetScale().y);
			hPos.y -= (hPos.y + pScale.y) - (mapObjects[i]->object->GetPosition().y - mapObjects[i]->object->GetScale().y);
			jumpParameter = 0.0f;

		}
	}
}

void Player::HeadInjectionProcess()
{
	XMFLOAT3 hInjectDis = { 80.0f, 0.0f, 0.0f };
	
	if (keyboard->TriggerKey(DIK_RETURN))
	{
		// ����΂�
		Audio::GetInstance()->PlayWave("Resources/SE/se3.wav", 0, 0.1f);
		hPos = pPos;
		playerHeadObj->SetPosition(hPos);
		hInjectDis.x *= direction.x;
		headInjectDis = hInjectDis + hPos;
		headState = STATE_INJECTION;
		moveTime = timeMax;
		biteProcessSE = false;
	}
}

void Player::HeadInjectionMoveProcess()
{
	float time = timeMax - moveTime;			//���Z���Ԃɕω�
	float timeRate = min(time / timeMax, 1.0f);	//�^�C�����[�g 0.0f->1.0f

	islonger = false;

	if (TimeCheck(moveTime) == true)
	{
		//�L�΂���������
		moveTime = timeMax;
		headBackDis = hPos;
		headState = STATE_BACK;
		islonger = true;
	}

	hPos = Easing::easeOut(pPos, headInjectDis, timeRate);
}

void Player::HeadBackMoveProcess()
{
	float time = timeMax - moveTime;			//���Z���Ԃɕω�
	float timeRate = min(time / timeMax, 1.0f);	//�^�C�����[�g 0.0f->1.0f
	
	if (TimeCheck(moveTime) == true)
	{
		moveTime = timeMax;
		headState = STATE_NORMAL;
	}

	hPos = Easing::easeIn( headBackDis, pPos, timeRate);
}

void Player::HeadBiteProcess(std::vector<MapData*>& mapObjects)
{
	// �����񂹂���u���b�N�ɂ��݂����ꍇ
	if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box_pull")
	{
		// �����񂹂���
		if (!biteProcessSE)
		{
			biteProcessSE = true;
			Audio::GetInstance()->PlayWave("Resources/SE/se6.wav", 2, 0.1f);
		}
		AttractBiteProcess(mapObjects);
		return;
	}
	else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "Box_Move")
	{
		// �u���b�N����������
		if (!biteProcessSE)
		{
			biteProcessSE = true;
			Audio::GetInstance()->PlayWave("Resources/SE/se6.wav", 2, 0.1f);
		}
		biteBlockState = NOTGRAVIT;
		CarryBlockProcess(mapObjects);
		return;
	}
	islonger = false;

	// �^�C�}�[�N��
	if (TimeCheck(biteTimer) == true)
	{
		headBackDis = hPos;
		headState = STATE_BACK;
	}
	 
	// ���ݗ���
	if (keyboard->PushKey(DIK_P))
	{
		headBackDis = hPos;
		headState = STATE_BACK;
	}

	// ���݉󂹂�u���b�N�̏ꍇ��
	if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box")
	{
		// ���ݍӂ�
		Audio::GetInstance()->PlayWave("Resources/SE/se5.wav", 0, 0.1f);
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

		//�ˏo��
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
			jumpParameter = 2.0f;
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
	//�u���b�N�Ƃ̓����蔻��
	if (HeadBlockCollisionCheck(mapObjects) == true)
	{
		//�������Ă���u���b�N�͊��݂��邩
		if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box")			//�󂹂�u���b�N
		{
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "box_pull")	//�����񂹂���u���b�N
		{
			pPosMovePrevious = pPos;
			return STATE_BITEHIT;
		}
		else if (mapObjects[hitHeadMapObjNum]->object->GetType() == "Box_Move")	//���������u���b�N
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
		if (mapObjects[i]->object->GetType() == "Ground_Move") continue;
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
			if (mapObjects[i]->object->GetType() == "Ground_Move") continue;
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
		headInjectDis = {};
		CsvFile::now_x++;
	}
	else if (limitPos == LEFT_LIMIT)
	{
		pPos.x = 159.0f;
		hPos = pPos;
		headInjectDis = {};
		CsvFile::now_x--;
	}
}

void Player::AcidProcess(std::vector<MapData*>& mapObjects)
{
	if (AcidBlockOnlyCollisionCheck(mapObjects) == true)
	{
		// ���S
		Audio::GetInstance()->PlayWave("Resources/SE/se4.wav", 0, 0.1f);
		ReturnCheckpoint();
		pPos = { CsvFile::check_pos.x, CsvFile::check_pos.y + 20.0f, CsvFile::check_pos.z };
		playerHeadObj->SetPosition(pPos);
		playerHeadObj->Update();
	}
}

void Player::ReturnCheckpoint()
{
	// �`�F�b�N�|�C���g�ɖ߂�
	CsvFile::now_x = CsvFile::check_x;
	CsvFile::now_y = CsvFile::check_y;
	isKey = false;
	isReturn = true;
	headState = STATE_NORMAL;
	headInjectDis = {};
}

void Player::CheckPointProcess(std::vector<MapData*>& mapObjects)
{
	for (int i = 0; i < mapObjects.size(); i++)
	{
		//���������u���b�N���`�F�b�N�|�C���g������
		if (mapObjects[i]->object->GetType() == "checkPoint")
		{
			XMFLOAT3 pos = mapObjects[i]->object->GetPosition();
			XMFLOAT3 scale = mapObjects[i]->object->GetScale();
			// �^�C�����O��ݒ�
			mapObjects[i]->object->SetTiring({ 0.5f, 1.0f });

			XMFLOAT3 pScaleXHalf = { pScale.x / 2, pScale.y, pScale.z };

			if (CsvFile::check_change_flag == false && Collision::CollisionBoxPoint(pos, scale, pPos, pScaleXHalf))
			{
				CsvFile::check_change_flag = true;
				// �V�����`�F�b�N�|�C���g�ɐG�ꂽ��
				if (CsvFile::now_x != CsvFile::check_x || CsvFile::now_y != CsvFile::check_y || CsvFile::check_pos != mapObjects[i]->object->GetPosition())
				{
					// �`�F�b�N�|�C���g�̍��W�ƃ}�b�v�ԍ���ۑ�
					CsvFile::check_pos = mapObjects[i]->object->GetPosition();
					CsvFile::check_x = CsvFile::now_x;
					CsvFile::check_y = CsvFile::now_y;
				}
				Audio::GetInstance()->PlayWave("Resources/SE/se9.wav", 0, 0.1f);
				ReturnCheckpoint();
			}
			else
			{
				CsvFile::check_change_flag = false;
			}

			// �`�F�b�N�|�C���g�̃I�t�Z�b�g�̐؂�ւ�(�e�N�X�`��)
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

	//���̈ʒu�ɑ̂������񂹂���
	float time = timeMax - moveTime;			//���Z���Ԃɕω�
	float timeRate = min(time / timeMax, 1.0f);	//�^�C�����[�g 0.0f->1.0f

	if (TimeCheck(moveTime) == true)
	{
		//�L�΂���������
		pPos = oldPPos;
		hPos = oldPPos;
		jumpParameter = 2.4f;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		return;
	}

	if (Collision::CollisionBoxPoint(mapObjects[hitHeadMapObjNum]->object->GetPosition(), mapObjects[hitHeadMapObjNum]->object->GetScale(), pPos, pScale) == true)
	{
		pPos = oldPPos;
		hPos = oldPPos;
		jumpParameter = 2.4f;
		moveTime = timeMax;
		headState = STATE_NORMAL;
		return;
	}
	
	oldPPos = pPos;
	pPos = Easing::easeOut(pPosMovePrevious, hPos, timeRate);
	//���̈ʒu�܂ōs������ʏ��Ԃɖ߂�
}

void Player::CarryBlockProcess(std::vector<MapData*>& mapObjects)
{
	static XMFLOAT3 oldHPos = {};
	//�̂̈ʒu�ɓ��������񂹂���
	float time = timeMax - moveTime;			//���Z���Ԃɕω�
	float timeRate = min(time / timeMax, 1.0f);	//�^�C�����[�g 0.0f->1.0f

	if (TimeCheck(moveTime) == true)
	{
		//�̂̈ʒu�ɖ߂�����
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
	//�u���b�N�ړ�����
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

bool Player::AcidBlockOnlyCollisionCheck(std::vector<MapData*>& mapObjects)
{
	XMFLOAT3 pScaleXHalf = { pScale.x / 2, pScale.y, pScale.z };

	bool AcidFlag = false;

	for (int i = 0; i < mapObjects.size(); i++)
	{
		if (Collision::CollisionBoxPoint(mapObjects[i]->object->GetPosition(), mapObjects[i]->object->GetScale(), pPos, pScaleXHalf) == true)
		{
			if (mapObjects[i]->object->GetType() == "sprite") continue;
			if (mapObjects[i]->object->GetType() == "Acid")
			{
				AcidFlag = true;
				hitbodyMapObjNum = i;
			}
			else if (mapObjects[i]->object->GetType() == "floor")
			{
				AcidFlag = false;
			}
		}
	}

	return AcidFlag;
}

bool Player::GetNotGravityFlag()
{
	if (biteBlockState != NOTGRAVIT) return false;
	if (headState != STATE_BITE) return false;
	return true;
}
