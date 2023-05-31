#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator*(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator/(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator+(const XMFLOAT3& v, float s);
const XMFLOAT3 operator-(const XMFLOAT3& v, float s);
const XMFLOAT3 operator*(const XMFLOAT3& v, float s);
const XMFLOAT3 operator/(const XMFLOAT3& v, float s);

const XMFLOAT3 operator+=(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator-=(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator*=(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator/=(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator+=(const XMFLOAT3& v, float s);
const XMFLOAT3 operator-=(const XMFLOAT3& v, float s);
const XMFLOAT3 operator*=(const XMFLOAT3& v, float s);
const XMFLOAT3 operator/=(const XMFLOAT3& v, float s);

const bool operator==(const XMFLOAT3& v1, const XMFLOAT3& v2);
const bool operator!=(const XMFLOAT3& v1, const XMFLOAT3& v2);

const XMFLOAT4 operator+(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator-(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator*(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator/(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator+(const XMFLOAT4& v, float s);
const XMFLOAT4 operator-(const XMFLOAT4& v, float s);
const XMFLOAT4 operator*(const XMFLOAT4& v, float s);
const XMFLOAT4 operator/(const XMFLOAT4& v, float s);