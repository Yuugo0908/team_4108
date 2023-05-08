#pragma once
#include "Mouse.h"
#include "Controller.h"
#include "Operator.h"
#include "DebugText.h"
class Camera
{
private:
	// カメラ初期化
	Camera();

	~Camera();

public:
	// コピーコンストラクタを無効化
	Camera(const Camera& obj) = delete;
	// 代入演算子を無効化
	Camera& operator=(const Camera& obj) = delete;

public: // 静的メンバ関数

	static Camera* GetInstance();
	// 初期化
	bool Initialize(const int window_width, const int window_height);
	// 視点座標の取得
	const XMFLOAT3& GetEye() { return eye; }
	// 視点座標の設定
	void SetEye(XMFLOAT3 eye);
	// 注視点座標の取得
	const XMFLOAT3& GetTarget() { return target; }
	// 注視点座標の設定
	void SetTarget(XMFLOAT3 target);
	// 上方向ベクトルの取得
	const XMFLOAT3& GetUp() { return up; }
	// 上方向ベクトルの設定
	void SetUp(XMFLOAT3 up);

	// ビュー行列の取得
	const XMMATRIX& GetMatView() {
		return matView;
	}

	// 射影行列の取得
	const XMMATRIX& GetMatProjection() {
		return matProjection;
	}

	// ビュー射影行列の取得
	const XMMATRIX& GetMatViewProjection() {
		return matViewProjection;
	}

	// ビルボード行列の取得
	const XMMATRIX& GetMatBillboard() {
		return matBillboard;
	}

	// ベクトルによる移動
	void CameraMove(const XMFLOAT3& move);
	void CameraMoveEye(const XMFLOAT3& move);
	void CameraMoveVector(const XMVECTOR& move);
	void CameraMoveEyeVector(const XMVECTOR& move);

	// 毎フレーム更新
	void Update();
	// ビュー行列を更新
	void UpdateViewMatrix();
	// 射影行列を更新
	void UpdateProjectionMatrix();
	// カメラのシェイクを実行
	void CameraShake(bool& flag);
	// カメラの追尾
	XMFLOAT3 CameraTrack(XMFLOAT3 pPos);
	float CameraRot(XMFLOAT3 pPos);
	// 距離を求める
	float GetLength(XMFLOAT3 posA, XMFLOAT3 posB)
	{
		XMFLOAT3 len = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}
	// 変数のリセット
	void Reset();
private: // メンバ変数
	// 入力クラスのポインタ
	Mouse* mouse = Mouse::GetInstance();
	Controller* controller = Controller::GetInstance();
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// ビルボード行列
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	// カメラの変化フラグ
	bool dirty = false;
	// 回転
	float angleX = 0.0f;
	float angleY = 0.0f;
	//対象とカメラの距離
	float distance = 0.0f;
	const float distanceMin = 10.0f;
	const float distanceMax = 20.0f;
	// 感度
	float mouseSensitivity = 0.01f;
	float controllerSensitivity = 0.075f;

	// 視点座標保存用
	XMFLOAT3 saveEye = {};
	// 視点座標
	XMFLOAT3 eye = { 0, 80.0f, -156.0f };
	// 視点のY座標の最小と最大
	const float eyeMin = 0.0f;
	const float eyeMax = 20.0f;
	// 注視点座標
	XMFLOAT3 target = { 0, 80, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	// カメラと対象物までの距離
	XMVECTOR cameraLength = {};
	// ビュー行列ダーティフラグ
	bool viewDirty = false;
	// 射影行列ダーティフラグ
	bool projectionDirty = false;
	// アスペクト比
	float aspectRatio = 1.0f;
	// シェイク座標
	XMFLOAT3 shake = {};
	// シェイク管理カウンター
	int shakeCount = 0;
	// カメラの動作管理用カウンター
	int moveCount = 30;
	// 注視点から視点へのベクトルと、上方向ベクトル
	XMVECTOR vTargetEye = {};
	XMVECTOR vUp = {};
};