#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Operator.h"

class Object3d
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標(ワールド座標)
		float a;
		XMFLOAT4 color;
	};

public: // 静的メンバ関数
	// 静的初期化
	static bool StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	// 描画後処理
	static void PostDraw();
	// 3Dオブジェクト生成
	static std::unique_ptr<Object3d> Create();
	// グラフィックパイプライン生成
	static bool CreateGraphicsPipeline();
	// ライトのセット
	static void SetLight(Light* light)
	{
		Object3d::light = light;
	}
	// カメラのセット
	static void SetCamera(Camera* camera)
	{
		Object3d::camera = camera;
	}

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;	
	// 頂点シェーダオブジェクト
	static ComPtr<ID3DBlob> vsBlob;
	// ピクセルシェーダオブジェクト
	static ComPtr<ID3DBlob> psBlob;
	// エラーオブジェクト
	static ComPtr<ID3DBlob> errorBlob;
	// ライト
	static Light* light;
	// カメラ
	static Camera* camera;
public: // メンバ関数
	bool Initialize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();
	// モデルスケールの取得
	const XMFLOAT3& GetScale() { return scale; }
	// モデルスケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	// 回転角の取得
	const XMFLOAT3& GetRotation() { return rotation; }
	// 回転角の設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }
	// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	// モデルの設定
	void SetModel(Model* model) { this->model = model; }
	// 色の設定
	void SetColor(XMFLOAT4 color) { this->color = color; }
	// オブジェクトのタイプ設定
	void SetType(std::string type) { this->type = type; }
	// オブジェクトタイプの取得
	const std::string& GetType() { return type; }
	// 移動先の座標の設定
	void SetMovePos(XMFLOAT3 movePos) { this->movePos = movePos; }
	// 移動先の座標の取得
	const XMFLOAT3& GetMovePos() { return movePos; }
	// 描画フラグの設定
	void SetDrawFlag(bool dFlag) { drawFlag = dFlag; }
	// 描画フラグの取得
	const bool& GetDrawFlag() { return drawFlag; }

	// かみつけるフラグの取得
	const bool& GetBiteFlag() { return biteFlag; }
	// 壊せるフラグの取得
	const bool& GetBreakFlag() { return breakFlag; }

	// レベルエディタ用 当たり判定スケール設定
	void SetCollisionScale(XMFLOAT3 collisionScale) { this->collisionScale = collisionScale; }
	// レベルエディタ用 当たり判定スケール取得
	const XMFLOAT3& GetCollisionScale() { return collisionScale; }

	inline const XMFLOAT3& GetPosiPos() { return model->GetPosiPos(); }
	inline const XMFLOAT3& GetNegaPos() { return model->GetNegaPos(); }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBufferB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	Model* model = nullptr;
	// オブジェクトタイプ
	std::string type;
	// コリジョンスケール
	XMFLOAT3 collisionScale = {};
	// 移動先の座標
	XMFLOAT3 movePos = {};
	// 描画フラグ
	bool drawFlag = true;
	// かみつけるフラグ
	bool biteFlag = false;
	// 壊せるフラグ
	bool breakFlag = false;
};