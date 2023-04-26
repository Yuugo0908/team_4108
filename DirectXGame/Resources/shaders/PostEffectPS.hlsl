#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);       // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float density = 100.0f;
	float4 texcolor0 = tex0.Sample(smp, input.uv);
	float4 texcolor1 = tex1.Sample(smp, floor(input.uv * density) / density);

	float4 color = texcolor0;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = texcolor1;
	}

	return float4(texcolor0.rgb, 1);
}