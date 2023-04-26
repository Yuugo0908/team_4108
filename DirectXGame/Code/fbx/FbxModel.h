#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <vector>
#include <DirectXTex.h>
#include <fbxsdk.h>

#include "Operator.h"

// DirectX::を省略
using TexMetadata = DirectX::TexMetadata;
using ScratchImage = DirectX::ScratchImage;

struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	XMVECTOR scaling = { 1, 1, 1, 0 };
	// ローカル回転角
	XMVECTOR rotation = { 0, 0, 0, 0 };
	// ローカル移動
	XMVECTOR translation = { 0, 0, 0, 1 };
	// ローカル変形行列
	XMMATRIX transform;
	// グローバル変形行列
	XMMATRIX globalTransform;
	// グローバル変形逆行列
	XMMATRIX inverseGlobalTransform;
	// 親ノード
	Node* parent = nullptr;
};

class FbxModel
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::を省略
	template <class T> using vector = std::vector<T>;
	using string = std::string;
public:
	// フレンドクラス
	friend class FbxLoader;
public: // 定数
	static const int MAX_BONE_INDICES = 4;
public:// サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;	// xyz座標
		XMFLOAT3 normal;// 法線ベクトル
		XMFLOAT2 uv;	// uv座標
		UINT boneIndex[MAX_BONE_INDICES]; // ボーン 番号
		float boneWeight[MAX_BONE_INDICES]; // ボーン 重み
	};
	struct Bone
	{
		// 名前
		std::string name;
		// 初期姿勢の逆行列
		XMMATRIX invInitialPose;
		// クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		// コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
private:
	// FBXシーン
	FbxScene* fbxScene = nullptr;
	// モデル名
	std::string name;
	// ノード配列
	std::vector<Node> nodes;
	// ボーン配列
	std::vector<Bone> bones;
	// メッシュを持つノード
	Node* meshNode = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// アンビエント係数
	XMFLOAT3 ambient = { 1, 1, 1 };
	// ディフューズ係数
	XMFLOAT3 diffuse = { 1, 1, 1 };
	// テクスチャメタデータ
	TexMetadata metadata = {};
	// スクラッチイメージ
	ScratchImage scratchImg = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuffer;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuffer;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
public:
	//デストラクタ
	~FbxModel();
	void CreateBuffers(ID3D12Device* device);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// getter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
	// モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//モデルの変形逆行列取得
	const XMMATRIX& GetInverseGlobalTransform()
	{
		meshNode->inverseGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshNode->globalTransform);
		return meshNode->inverseGlobalTransform;
	}
};