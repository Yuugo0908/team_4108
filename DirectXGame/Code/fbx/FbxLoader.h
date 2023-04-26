#pragma once

#include "fbxsdk.h"
#include <string>
#include "FbxModel.h"
#include <d3d12.h>
#include <d3dx12.h>

class FbxLoader
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
public:
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();
private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	FbxModel* LoadModelFromFile(const string& modelName);
	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxmodel"></param>
	/// <param name="fbxNode"></param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);
	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="fbxModel"></param>
	/// <param name="fbxNode"></param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	// 頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	// 面情報読み取り
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	// マテリアル読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	// テクスチャ読み取り
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);
	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);
	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">もととなるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);
	// スキニング情報の読み取り
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);
private:
	// D3D12デバイス
	ID3D12Device* device = nullptr;
	// FBXマネージャ
	FbxManager* fbxManager = nullptr;
	// FBXインポータ
	FbxImporter* fbxImporter = nullptr;
};