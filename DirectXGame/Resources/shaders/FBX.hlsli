// ボーンの最大数
static const int MAX_BONES = 32;

// 行列用定数バッファ
cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	// ビュープロジェクション行列
	matrix world;		// ワールド行列
	float3 cameraPos;	// カメラ座標
};
cbuffer skinning:register(b1)
{
	// ボーンのスキニング行列
	matrix matSkinning[MAX_BONES];
};
cbuffer cbuff1 : register(b2)
{
	float3 m_ambient;// : packoffset(c0); //アンビエント係数
	float3 m_diffuse;// : packoffset(c1); //ディフューズ係数
	float3 m_specular;// : packoffset(c2); //スペキュラー係数
	float m_alpha;// : packoffset(c2.w); //アルファ
};
cbuffer cbuff2 : register(b3)
{
	float3 lightVec;		// ライト方向の単位ベクトル
	float3 lightColor;		// ライトの色(RGB)
}

// vertexバッファーの入力
struct VSInput
{
	float4 pos : POSITION;	// 位置
	float3 normal : NORMAL;	// 頂点法線
	float2 uv : TEXCOORD;	//テクスチャ座標
	uint4 boneIndices : BONEINDICES; // ボーンの番号
	float4 boneWeights : BONEWEIGHTS; // ボーンのスキンウェイト
};
// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; //ワールド座標
	float3 normal : NORMAL;		// 法線
	float2 uv : TEXCOORD;		// uv値
};

