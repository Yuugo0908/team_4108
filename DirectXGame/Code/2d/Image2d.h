#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "Operator.h"

class Image2d
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	// 静的初期化
	static bool StaticInitialize(ID3D12Device* device);
	// テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);
	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	// 描画後処理
	static void PostDraw();
	// 画像生成
	static Image2d* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);
	// グラフィックパイプライン生成
	static bool CreateGraphicsPipeline();

protected: // 静的メンバ変数
	// テクスチャの最大枚数
	static const int srvCount = 512;
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
	// 射影行列
	static XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuffer[srvCount];

public: // メンバ関数
	// コンストラクタ
	Image2d(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	// 初期化
	bool Initialize();
	// 角度の設定
	void SetRotation(float rotation);
	// 座標の設定
	void SetPosition(XMFLOAT2 position);
	// サイズの設定
	void SetSize(XMFLOAT2 size);
	// 色の設定
	void SetColor(XMFLOAT4 color);
	// アンカーポイントの設定
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	// 左右反転の設定
	void SetIsFlipX(bool isFlipX);
	// 上下反転の設定
	void SetIsFlipY(bool isFlipY);
	// テクスチャ範囲設定
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	// 描画
	void Draw();
	// 頂点データ転送
	void TransferVertices();

protected: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuffer;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// テクスチャ番号
	UINT texNumber = 0;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{};
	// 画像幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texBase = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f, 100.0f };
};