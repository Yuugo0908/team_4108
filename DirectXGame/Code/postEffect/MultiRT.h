#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "Operator.h"

class MultiRT
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	HRESULT result;

public: // サブクラス
// 頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列
	};

public: // 静的メンバ関数

private: // 静的メンバ変数
	// 画面クリアカラー
	static const float clearColor[4];
	// 頂点数
	static const int vertNum = 4;
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
public: // メンバ関数
	/// <summary>
	/// グラフィックスパイプライン生成
	/// </summary>
	/// <returns></returns>
	void CreateGraphicsPipeline();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 頂点バッファ生成
	/// </summary>
	void CreateVB();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateCB();

	/// <summary>
	/// テクスチャ生成
	/// </summary>
	void CreateTex();

	/// <summary>
	/// シェーダーリソースビュー作成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	void CreateRTV();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// デプスステンシルビュー生成
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList"></param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	/// <param name="cmdList"></param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuffer;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuffer[2];
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
};

