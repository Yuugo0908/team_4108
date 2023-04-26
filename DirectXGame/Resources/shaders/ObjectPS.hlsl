#include "Object.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv) * color;

	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 4.0f;
	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ライトに向かうベクトルの法線の内積
	float3 dotlightnormal = dot(lightVec, input.normal);
	//反射光ベクトル
	float3 reflect = normalize(-lightVec + 2 * dotlightnormal * input.normal);

	//環境反射光
	float3 ambient = m_ambient;
	//拡散反射光
	float3 diffuse = dotlightnormal * m_diffuse;
	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//全て加算する
	shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
	shadecolor.a = m_alpha;

	return shadecolor * texcolor;
}