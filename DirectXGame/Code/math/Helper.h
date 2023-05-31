#pragma once
#include <DirectXMath.h>

/// </summary>
class Helper
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// �������擾
	/// </summary>
	/// <param name="num">����</param>
	/// <returns>���Ȃ�-1�A���Ȃ�1</returns>
	inline static int GetSign(float num)
	{
		int result = static_cast<int>(num / fabs(num));

		return result;
	}

	/// <summary>
	/// XMFLOAT2���m�̒������v�Z
	/// </summary>
	/// <param name="tmp1">�I�_</param>
	/// <param name="tmp2">�n�_</param>
	/// <returns>����</returns>
	inline static float LengthFloat2(const DirectX::XMFLOAT2& tmp1, const DirectX::XMFLOAT2& tmp2 = { 0, 0 })
	{
		DirectX::XMFLOAT2 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y };

		return sqrtf(len.x * len.x + len.y * len.y);
	}

	/// <summary>
	/// XMFLOAT3���m�̒������v�Z
	/// </summary>
	/// <param name="tmp1">�I�_</param>
	/// <param name="tmp2">�n�_</param>
	/// <returns>����</returns>
	inline static float LengthFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 })
	{
		DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

		return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
	}

	/// <summary>
	/// XMFLOAT3���m�̒������v�Z
	/// </summary>
	/// <param name="tmp1">�I�_</param>
	/// <param name="tmp2">�n�_</param>
	/// <returns>����</returns>
	inline static DirectX::XMFLOAT3 VectorFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 })
	{
		DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

		return len;
	}

	/// <summary>
	/// XMFLOAT3�𐳋K��
	/// </summary>
	/// <param name="tmp">XMFLOAT3</param>
	/// <param name="num">���K������l</param>
	/// <returns>�ϊ����ꂽXMFLOAT3</returns>
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
	/// �l��͈͓��Ɏ��߂�
	/// </summary>
	/// <param name="low">�Œ�l</param>
	/// <param name="num">�l</param>
	/// <param name="high">�ō��l</param>
	/// <returns>�l</returns>
	inline static float Clamp(float low, float num, float high)
	{
		return min(max(num, low), high);
	}
};
