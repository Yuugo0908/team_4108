#pragma once
#include "Mouse.h"
#include "Controller.h"
#include "Operator.h"
#include "DebugText.h"
class Camera
{
private:
	// �J����������
	Camera();

	~Camera();

public:
	// �R�s�[�R���X�g���N�^�𖳌���
	Camera(const Camera& obj) = delete;
	// ������Z�q�𖳌���
	Camera& operator=(const Camera& obj) = delete;

public: // �ÓI�����o�֐�

	static Camera* GetInstance();
	// ������
	bool Initialize(const int window_width, const int window_height);
	// ���_���W�̎擾
	const XMFLOAT3& GetEye() { return eye; }
	// ���_���W�̐ݒ�
	void SetEye(XMFLOAT3 eye);
	// �����_���W�̎擾
	const XMFLOAT3& GetTarget() { return target; }
	// �����_���W�̐ݒ�
	void SetTarget(XMFLOAT3 target);
	// ������x�N�g���̎擾
	const XMFLOAT3& GetUp() { return up; }
	// ������x�N�g���̐ݒ�
	void SetUp(XMFLOAT3 up);

	// �r���[�s��̎擾
	const XMMATRIX& GetMatView() {
		return matView;
	}

	// �ˉe�s��̎擾
	const XMMATRIX& GetMatProjection() {
		return matProjection;
	}

	// �r���[�ˉe�s��̎擾
	const XMMATRIX& GetMatViewProjection() {
		return matViewProjection;
	}

	// �r���{�[�h�s��̎擾
	const XMMATRIX& GetMatBillboard() {
		return matBillboard;
	}

	// �x�N�g���ɂ��ړ�
	void CameraMove(const XMFLOAT3& move);
	void CameraMoveEye(const XMFLOAT3& move);
	void CameraMoveVector(const XMVECTOR& move);
	void CameraMoveEyeVector(const XMVECTOR& move);

	// ���t���[���X�V
	void Update();
	// �r���[�s����X�V
	void UpdateViewMatrix();
	// �ˉe�s����X�V
	void UpdateProjectionMatrix();
	// �J�����̃V�F�C�N�����s
	void CameraShake(bool& flag);
	// �J�����̒ǔ�
	XMFLOAT3 CameraTrack(XMFLOAT3 pPos);
	float CameraRot(XMFLOAT3 pPos);
	// ���������߂�
	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}
	// �ϐ��̃��Z�b�g
	void Reset();
private: // �����o�ϐ�
	// ���̓N���X�̃|�C���^
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	// �J�����̕ω��t���O
	bool dirty = false;
	// ��]
	float angleX = 0.0f;
	float angleY = 0.0f;
	//�ΏۂƃJ�����̋���
	float distance = 0.0f;
	const float distanceMin = 10.0f;
	const float distanceMax = 20.0f;
	// ���x
	float mouseSensitivity = 0.01f;
	float controllerSensitivity = 0.075f;

	// ���_���W�ۑ��p
	XMFLOAT3 saveEye = {};
	// ���_���W
	XMFLOAT3 eye = { 0, 5.0f, -15.0f };
	// ���_��Y���W�̍ŏ��ƍő�
	const float eyeMin = 0.0f;
	const float eyeMax = 20.0f;
	// �����_���W
	XMFLOAT3 target = { 0, 0, 0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	// �J�����ƑΏە��܂ł̋���
	XMVECTOR cameraLength = {};
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;
	// �V�F�C�N���W
	XMFLOAT3 shake = {};
	// �V�F�C�N�Ǘ��J�E���^�[
	int shakeCount = 0;
	// �J�����̓���Ǘ��p�J�E���^�[
	int moveCount = 30;
	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	XMVECTOR vTargetEye = {};
	XMVECTOR vUp = {};

};