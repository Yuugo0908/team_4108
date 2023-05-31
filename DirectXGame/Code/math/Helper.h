#pragma once
#include <DirectXMath.h>

/// </summary>
class Helper
{
public: // 静的メンバ関数
	/// <summary>
	/// 符号を取得
	/// </summary>
	/// <param name="num">数字</param>
	/// <returns>負なら-1、正なら1</returns>
	inline static int GetSign(float num)
	{
		int result = static_cast<int>(num / fabs(num));

		return result;
	}

	/// <summary>
	/// XMFLOAT2同士の長さを計算
	/// </summary>
	/// <param name="tmp1">終点</param>
	/// <param name="tmp2">始点</param>
	/// <returns>長さ</returns>
	inline static float LengthFloat2(const DirectX::XMFLOAT2& tmp1, const DirectX::XMFLOAT2& tmp2 = { 0, 0 })
	{
		DirectX::XMFLOAT2 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y };

		return sqrtf(len.x * len.x + len.y * len.y);
	}

	/// <summary>
	/// XMFLOAT3同士の長さを計算
	/// </summary>
	/// <param name="tmp1">終点</param>
	/// <param name="tmp2">始点</param>
	/// <returns>長さ</returns>
	inline static float LengthFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 })
	{
		DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

	/// <summary>
	/// XMFLOAT3同士の長さを計算
	/// </summary>
	/// <param name="tmp1">終点</param>
	/// <param name="tmp2">始点</param>
	/// <returns>長さ</returns>
	inline static DirectX::XMFLOAT3 VectorFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 })
	{
		DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

		return len;
	}

	/// <summary>
	/// XMFLOAT3を正規化
	/// </summary>
	/// <param name="tmp">XMFLOAT3</param>
	/// <param name="num">正規化する値</param>
	/// <returns>変換されたXMFLOAT3</returns>
	inline static DirectX::XMFLOAT3 NormalizeFloat3(const DirectX::XMFLOAT3& tmp, float num = 1.0f)
	{
		DirectX::XMFLOAT3 _normalize = tmp;

		float len = LengthFloat3(tmp);
		_normalize.x /= len;
		_normalize.y /= len;
		_normalize.z /= len;

		_normalize.x *= num;
		_normalize.y *= num;
		_normalize.z *= num;

		return _normalize;
	}

	/// <summary>
	/// 値を範囲内に収める
	/// </summary>
	/// <param name="low">最低値</param>
	/// <param name="num">値</param>
	/// <param name="high">最高値</param>
	/// <returns>値</returns>
	inline static float Clamp(float low, float num, float high)
	{
		return min(max(num, low), high);
	}
};
