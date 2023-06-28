#pragma once
#include "Object3d.h"
#include "Keyboard.h"
#include "Controller.h"
#include "Collision.h"
#include "Easing.h"
#include "Operator.h"
#include "Camera.h"
#include "Particle.h"
#include "DirectXCommon.h"
#include "CsvFile.h"
#include "MapData.h"

class Player
{
public:
	enum BodyState
	{
		STATE_BODY_NORMAL,
		STATE_BODY_JUMP_UP,
		STATE_BODY_JUMP_DOWN,
		STATE_BODY_MOVE
	};

	enum HeadState
	{
		STATE_NORMAL,
		STATE_INJECTION,
		STATE_BITE,
		STATE_BACK,
		STATE_INJECTIONLOCK
	};

	enum HeadInjectionState
	{
		STATE_NULL,
		STATE_MOVE,
		STATE_BITEHIT,
		STATE_UNBITEHIT,
	};

	enum BodyCollsionState
	{
		BODYSTATE_NULL,
		BODYSTATE_CEILING_COLISION,
		BODYSTATE_X_COLISION,
		BODYSTATE_Y_COLISION,
		BODYSTATE_XY_COLISION
	};

	enum LimitPos
	{
		NONE,
		UP_LIMIT,
		DOWN_LIMIT,
		RIGHT_LIMIT,
		LEFT_LIMIT
	};

	enum BiteBlockState
	{
		NOTBITE,
		NORMAL,
		NOTGRAVIT,
	};

public: // �����o�֐�
	//����������
	bool Initialize(const XMFLOAT3 pos, const XMFLOAT3 scale);
	// �X�V����
	void Update(std::vector<MapData*> &mapObjects);
	// �����߂�
	void AddMove(const XMFLOAT3& move);
	// �I�u�W�F�N�g
	Object3d* GetObj() { return playerObj; }
	Object3d* GetHedObj() { return playerHedObj; }

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void MoveProcess();
	/// <summary>
	/// �W�����v����
	/// </summary>
	void JumpProcess();
	/// <summary>
	/// �d�͏���
	/// </summary>
	void GravityProcess();
	/// <summary>
	/// �n��Y���Փ˔��菈��
	/// </summary>
	void GroundCollisionProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �n�`�u���b�NX���Փˏ���
	/// </summary>
	/// <param name="mapObjects">�}�b�v�̃u���b�N</param>
	void BlockCollisionProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �V��Ɋւ��Ẵu���b�N�Փ˔���
	/// </summary>
	/// <param name="mapObjects">�}�b�v�̃u���b�N</param>
	void CeilingBlockCollisionProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// ���ˏo����
	/// </summary>
	void HeadInjectionProcess();
	/// <summary>
	/// ���ˏo�ړ�����
	/// </summary>
	void HeadInjectionMoveProcess();
	/// <summary>
	/// ���݂��邩����
	/// </summary>
	void HeadBiteCollisionProcess();
	/// <summary>
	/// ���߂鏈��
	/// </summary>
	void HeadBackMoveProcess();
	/// <summary>
	/// ���݂�����
	/// </summary>
	void HeadBiteProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// ���X�V����
	/// </summary>
	void HeadUpdateProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �������蔻�菈��
	/// </summary>
	/// <returns>���̓����蔻�茋��</returns>
	HeadInjectionState HeadCollision(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// ���ƃu���b�N�̓����蔻��
	/// </summary>
	/// <returns>�����蔻��</returns>
	bool HeadBlockCollisionCheck(std::vector<MapData*> &mapObjects);

	bool BodyBlockCollisionCheck(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �}�b�v�ύX
	/// </summary>
	void MapChange(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �_�u���b�N�Ƃ̓����蔻��
	/// </summary>
	/// <param name="mapObjects"></param>
	void AcidProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �`�F�b�N�|�C���g�ɖ߂�
	/// </summary>
	void ReturnCheckpoint();
	/// <summary>
	/// �`�F�b�N�|�C���g�Ƃ̓����蔻��
	/// </summary>
	/// <param name="mapObjects"></param>
	void CheckPointProcess(std::vector<MapData*>& mapObjects);
	/// <summary>
	/// �����񂹃u���b�N����
	/// </summary>
	/// <param name="mapObjects"></param>
	void AttractBiteProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// �u���b�N�^�я���
	/// </summary>
	/// <param name="mapObjects"></param>
	void CarryBlockProcess(std::vector<MapData*> &mapObjects);
	/// <summary>
	/// ���Ԋm�F
	/// </summary>
	/// <param name="time"></param>
	/// <returns></returns>
	bool TimeCheck(float& time);

public:
	//Geter
	bool& GetOnGround() { return onGround; }
	float& GetmoveY() { return moveY; }
	XMFLOAT3& GetBodyPos() { return pPos; }
	void SetBodyPos(const XMFLOAT3& pos) { pPos = pos; }
	XMFLOAT3& GetHeadPos() { return hPos; }
	void SetHeadPos(const XMFLOAT3& pos) { hPos = pos; }
	void SetBodyMove(const XMFLOAT3& move) { this->move = move; }
	XMFLOAT3& GetHeadInjectPos() { return headInjectDis; }
	float& GetBiteTimer() { return biteTimer; }
	HeadState& GetHeadState() { return headState; }

	bool GetNotGravityFlag();
	int GetHeadHitMapNum() { return hitHeadMapObjNum; }
	bool GetOnGrounding()
	{
		return onGround == true && oldOnGround == false;
	}
	void OnGrounding()
	{
		this->onGround = true;
		this->oldOnGround = true;
	}
	bool& GetIsLonger() { return islonger; }
	bool& GetIsKey() { return isKey; }
	void SetIKey(bool iskey) { this->isKey = iskey; }
	bool& GetIsHit() { return isHit; }

private: // �����o�ϐ�

	Keyboard* keyboard = Keyboard::GetInstance();
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	Camera* camera = Camera::GetInstance();

	// ���f��
	Model* playerModel = nullptr;
	Object3d* playerObj = nullptr;
	Object3d* playerHedObj = nullptr;

	// �v���C���[
	XMFLOAT3 pDirection = {1.0f, 0.0f, 0.0f};	//����
	XMFLOAT3 objPos = {};						//�`���p�̍��W
	XMFLOAT3 pPos = {};							//�̍��W
	XMFLOAT3 oldpPos = {};						//�O�t���[���̑̂̈ʒu
	XMFLOAT3 hPos = {};							//�����W
	XMFLOAT3 headInjectDis = {};				//���ˏo��ʒu
	XMFLOAT3 headBackDis = {};					//���߂��
	XMFLOAT3 pPosMovePrevious = {};				//�̈ړ��O�̈ʒu
	XMFLOAT3 pScale = {};						//�傫��
	XMFLOAT3 pRot = {};							//��]
	XMFLOAT3 reSpawnPos = {};					//
	XMFLOAT3 move = {};							//�ړ���
	XMFLOAT3 hmove = {};						//���̈ړ���
	XMFLOAT3 direction = {};					//�����Ă������
	float moveY = 0.0f;							//�W�����v�y�яd��
	bool onGround = false;
	bool oldOnGround = false;
	HeadState headState = STATE_NORMAL;
	BodyState bodyState = STATE_BODY_NORMAL;
	int hitHeadMapObjNum = 0;
	int hitbodyMapObjNum = 0;
	float biteTimer = 5.0f;
	float timeMax = 1.0f;
	float moveTime = 1.0f;
	BodyCollsionState bodyColState = BODYSTATE_NULL;
	int colisionBlockNum = 0;
	bool islonger = false;
	bool isKey = false;
	bool isHit = false;
	LimitPos limitPos = NONE;
	BiteBlockState biteBlockState = NOTBITE;
};

