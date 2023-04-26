#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "Operator.h"

class Model
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name; //マテリアル名
		XMFLOAT3 ambient; //アンビエント影響度
		XMFLOAT3 diffuse; //ディフューズ影響度
		XMFLOAT3 specular; //スペキュラー影響度
		float alpha; //アルファ
		std::string textureFilename; //テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient =	{ 0.0f, 0.0f, 0.0f };
			diffuse =	{ 0.0f, 0.0f, 0.0f };
			specular =	{ 1.0f, 1.0f, 1.0f };
			alpha =		1.0f;
		}
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //アンビエント係数
		float pad1; //パディング
		XMFLOAT3 diffuse; //ディフューズ係数
		float pad2; //パディング
		XMFLOAT3 specular; //スペキュラー係数
		float alpha; //アルファ
	};

private: // 定数

public: // 静的メンバ関数
	// 静的初期化
	static bool StaticInitialize(ID3D12Device* device);

	Model* CreateFromObject(const std::string& text);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuffer;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuffer;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuffer;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// マテリアル
	Material material;

private:// 静的メンバ関数
	// デスクリプタヒープの初期化
	bool InitializeDescriptorHeap();
	// テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	// モデル作成
	void CreateModel(const std::string& text);
	//マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

public: // メンバ関数
	bool Initialize(const std::string& text);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBufferB1; // 定数バッファ
};