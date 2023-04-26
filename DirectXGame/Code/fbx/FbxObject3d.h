#pragma once

#include "FbxModel.h"
#include "Camera.h"
#include "Operator.h"
#include "Light.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	// X, Y, Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	// ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FbxModel* fbxModel = nullptr;
	// 1フレームの時間
	FbxTime frameTime;
	// アニメーション開始時間
	FbxTime startTime;
	// アニメーション終了時間
	FbxTime endTime;
	// 現在時間（アニメーション）
	FbxTime currentTime;
	// アニメーション再生中
	bool isPlay = false;
public: // 定数
	// ボーンの最大数
	static const int MAX_BONES = 32;
	// ライト
	static Light* light;
public: // 定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public: // 静的メンバ関数
	// setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }
	static void SetLight(Light* light) { FbxObject3d::light = light; }
private:
	// デバイス
	static ID3D12Device* device;
	// カメラ
	static Camera* camera;
public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	// ビュープロジェクション行列
		XMMATRIX world;		// ワールド行列
		XMFLOAT3 cameraPos;	// カメラ座標（ワールド座標）
	};
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

	// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	// モデルスケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	// 回転角の設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBufferTransform;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBufferSkin;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
};
