#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera* Camera::GetInstance()
{
	static Camera instance;

	return &instance;
}

bool Camera::Initialize(const int window_width, const int window_height)
{
	// �A�X�y�N�g��
	aspectRatio = (float)window_width / window_height;

	// �J�����̍ŏ��̌�����ύX����Ȃ炱���ŕύX
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;

	return true;
}

void Camera::SetEye(XMFLOAT3 eye)
{
	Camera::eye = eye;
	viewDirty = true;
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;
	viewDirty = true;
}

void Camera::SetUp(XMFLOAT3 up)
{
	Camera::up = up;
	viewDirty = true;
}

void Camera::CameraMove(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveVector(const XMVECTOR& move)
{
	// ���_�ƒ����_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	target_moved.x += move.m128_f32[0];
	target_moved.y += move.m128_f32[1];
	target_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveEye(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::CameraMoveEyeVector(const XMVECTOR& move)
{
	// ���_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
}

void Camera::Update()
{
	angleX = 0.0f;
	angleY = 0.0f;
	dirty = false;

	XMFLOAT3 cameraTrack = CameraTrack(target);
	distance = GetLength(eye, target);

	if (saveEye.y > eyeMax)
	{
		saveEye.y = eyeMax;
	}
	if (saveEye.y < eyeMin)
	{
		saveEye.y = eyeMin;
	}

	// �}�E�X�̃J�[�\���ړ��ŃJ�����̉�]
	if (mouse->GetMouseMove().MouseX != 0)
	{
		angleY = mouse->GetMouseMove().MouseX * mouseSensitivity;
		dirty = true;
	}

	if (mouse->GetMouseMove().MouseY != 0)
	{
		saveEye.y += mouse->GetMouseMove().MouseY * mouseSensitivity;
		dirty = true;
	}

	// �R���g���[���[���ڑ�����Ă��邩
	if (controller->Detection())
	{
		// �E�X�e�B�b�N����ŃJ��������]
		if (controller->GetPadState(Controller::State::RIGHT_L_STICK, Controller::Type::NONE))
		{
			angleY -= XM_PI * controllerSensitivity;
			dirty = true;
		}
		else if (controller->GetPadState(Controller::State::RIGHT_R_STICK, Controller::Type::NONE))
		{
			angleY += XM_PI * controllerSensitivity;
			dirty = true;
		}

		if (controller->GetPadState(Controller::State::RIGHT_U_STICK, Controller::Type::NONE))
		{
			saveEye.y -= XM_PI * controllerSensitivity;
			dirty = true;
		}
		else if (controller->GetPadState(Controller::State::RIGHT_D_STICK, Controller::Type::NONE))
		{
			saveEye.y += XM_PI * controllerSensitivity;
			dirty = true;
		}
	}

	if (dirty || viewDirty)
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(angleY / 10.0f);
		// �ݐς̉�]�s�������
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		vTargetEye = { saveEye.x, saveEye.y, saveEye.z, 1.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	}

	if (viewDirty || projectionDirty)
	{
		// �Čv�Z�K�v�Ȃ�
		if (viewDirty)
		{
			// �r���[�s��X�V
			UpdateViewMatrix();
			viewDirty = false;
		}

		// �Čv�Z�K�v�Ȃ�
		if (projectionDirty)
		{
			// �r���[�s��X�V
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// �r���[�v���W�F�N�V�����̍���
		matViewProjection = matView * matProjection;
	}
}

void Camera::UpdateViewMatrix()
{
	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// �J������Y���i������j
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����

	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;


#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

#pragma region Y�����r���{�[�h�s��̌v�Z
	// �J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X���͋���
	ybillCameraAxisX = cameraAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

void Camera::UpdateProjectionMatrix()
{
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1000.0f);
}

void Camera::CameraShake(bool& flag)
{
	//�J�����X�V
	int power = 5;
	shake.x = static_cast<float>(rand() % power) / 10;
	shake.y = static_cast<float>(rand() % power) / 10;

	shakeCount++;
	if (shakeCount > 20)
	{
		shake.x = 0.0f;
		shake.y = 0.0f;
		shakeCount = 0;
		flag = false;
	}

	eye.x += shake.x;
	eye.y += shake.y;
	target.x += shake.x;
	target.y += shake.y;

	SetEye(eye);
	SetTarget(target);
}

XMFLOAT3 Camera::CameraTrack(XMFLOAT3 pPos)
{
	XMVECTOR playerPos = { pPos.x, pPos.y, pPos.z, 0 };
	XMVECTOR cameraPos = { eye.x, eye.y, eye.z, 0 };

	XMVECTOR subCameraPlayer = XMVectorSubtract(playerPos, cameraPos);
	cameraLength= XMVector3Normalize(subCameraPlayer);

	XMFLOAT3 Track = { cameraLength.m128_f32[0], cameraLength.m128_f32[1], cameraLength.m128_f32[2] };

	return Track;
}

float Camera::CameraRot(XMFLOAT3 pPos)
{
	XMVECTOR playerPos = { pPos.x, pPos.y, pPos.z, 0 };
	XMVECTOR cameraPos = { eye.x, eye.y, eye.z, 0 };

	XMVECTOR subCameraPlayer = XMVectorSubtract(playerPos, cameraPos);

	float angle = (float)atan2(subCameraPlayer.m128_f32[0], subCameraPlayer.m128_f32[2]);

	return angle;
}

void Camera::Reset()
{
	eye = { 0, 5.0f, -15.0f };
	saveEye = eye;
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };
	cameraLength = {};
	shake = {};
	vTargetEye = {};
	vUp = {};
	viewDirty = false;
	projectionDirty = false;
	shakeCount = 0;

	dirty = false;
	angleX = 0.0f;
	angleY = 0.0f;

	// �r���[�s��
	matView = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	matViewProjection = DirectX::XMMatrixIdentity();
	// ��]�s��
	matRot = DirectX::XMMatrixIdentity();

	//�r���[�s��̌v�Z
	UpdateViewMatrix();
	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();
	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}
