#pragma once
#include <DirectXMath.h>

using namespace DirectX;
class Operator : public XMFLOAT3
{
public:
	//コンストラクタ
	Operator();
	Operator(float x, float y, float z);

	//メンバ関数
	float length();
	XMFLOAT3 normalize();
	static float dot(const XMFLOAT3& v1, const XMFLOAT3& v2);
	XMFLOAT3 cross(const XMFLOAT3& v);
	XMFLOAT3 cross3D(const XMFLOAT3& v1, XMFLOAT3& v2);

	//単項演算子オーバーロード
	Operator operator+();
	Operator operator-();

	//代入演算子オーバーロード
	Operator& operator+=(const Operator& v);
	Operator& operator-=(const Operator& v);
	Operator& operator*=(float s);
	Operator& operator/=(float s);
};

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

const XMFLOAT4 operator+(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator-(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator*(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator/(const XMFLOAT4& v1, const XMFLOAT4& v2);
const XMFLOAT4 operator+(const XMFLOAT4& v, float s);
const XMFLOAT4 operator-(const XMFLOAT4& v, float s);
const XMFLOAT4 operator*(const XMFLOAT4& v, float s);
const XMFLOAT4 operator/(const XMFLOAT4& v, float s);