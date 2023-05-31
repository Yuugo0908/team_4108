#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//2項演算子オーバーロード
//※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
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