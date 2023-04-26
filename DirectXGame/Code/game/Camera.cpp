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
	// アスペクト比
	aspectRatio = (float)window_width / window_height;

	// カメラの最初の向きを変更するならここで変更
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
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
	// 視点と注視点座標を移動し、反映
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
	// 視点座標を移動し、反映
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

	// マウスのカーソル移動でカメラの回転
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

	// コントローラーが接続されているか
	if (controller->Detection())
	{
		// 右スティック操作でカメラを回転
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
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(angleY / 10.0f);
		// 累積の回転行列を合成
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		vTargetEye = { saveEye.x, saveEye.y, saveEye.z, 1.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);

		// 注視点からずらした位置に視点座標を決定
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	}

	if (viewDirty || projectionDirty)
	{
		// 再計算必要なら
		if (viewDirty)
		{
			// ビュー行列更新
			UpdateViewMatrix();
			viewDirty = false;
		}

		// 再計算必要なら
		if (projectionDirty)
		{
			// ビュー行列更新
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// ビュープロジェクションの合成
		matViewProjection = matView * matProjection;
	}
}

void Camera::UpdateViewMatrix()
{
	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分

	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;


#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1000.0f);
}

void Camera::CameraShake(bool& flag)
{
	//カメラ更新
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

	// ビュー行列
	matView = DirectX::XMMatrixIdentity();
	// ビルボード行列
	matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	matBillboardY = DirectX::XMMatrixIdentity();
	// 射影行列
	matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	matViewProjection = DirectX::XMMatrixIdentity();
	// 回転行列
	matRot = DirectX::XMMatrixIdentity();

	//ビュー行列の計算
	UpdateViewMatrix();
	// 射影行列の計算
	UpdateProjectionMatrix();
	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}
