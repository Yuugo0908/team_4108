#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include "Operator.h"

class Light
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightVec; // ライトのベクトル
		XMFLOAT3 lightColor; // ライトの色
	};
private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static bool StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static Light* Create();
private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// ライト光線方向(単位ベクトル)
	XMVECTOR lightdir = { 1, 0, 0, 0 };
	// ライト色
	XMFLOAT3 lightColor = { 0.8f, 0.8f, 0.8f };
	// ダーティフラグ
	bool dirty = false;
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 初期化
	/// </summary>
	void TransferConstBuffer();
	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name = "lightdir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightdir);
	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name = "lightcolor">ライト方向</param>
	void SetLightColor(const XMFLOAT3& lightcolor);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
};

