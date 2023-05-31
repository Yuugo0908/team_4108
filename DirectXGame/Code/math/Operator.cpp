#include "Operator.h"

// XMFLOAT3
const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x + v2.x;
	w.y = v1.y + v2.y;
	w.z = v1.z + v2.z;
	return w;
}
const XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x - v2.x;
	w.y = v1.y - v2.y;
	w.z = v1.z - v2.z;
	return w;
}
const XMFLOAT3 operator*(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x * v2.x;
	w.y = v1.y * v2.y;
	w.z = v1.z * v2.z;
	return w;
}
const XMFLOAT3 operator/(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x / v2.x;
	w.y = v1.y / v2.y;
	w.z = v1.z / v2.z;
	return w;
}

const XMFLOAT3 operator+(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x + s;
	w.y = v.y + s;
	w.z = v.z + s;
	return w;
}
const XMFLOAT3 operator-(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x - s;
	w.y = v.y - s;
	w.z = v.z - s;
	return w;
}
const XMFLOAT3 operator*(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x * s;
	w.y = v.y * s;
	w.z = v.z * s;
	return w;
}
const XMFLOAT3 operator/(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x / s;
	w.y = v.y / s;
	w.z = v.z / s;
	return w;
}

const XMFLOAT3 operator+=(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x + v2.x;
	w.y = v1.y + v2.y;
	w.z = v1.z + v2.z;
	return w;
}
const XMFLOAT3 operator-=(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x - v2.x;
	w.y = v1.y - v2.y;
	w.z = v1.z - v2.z;
	return w;
}
const XMFLOAT3 operator*=(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x * v2.x;
	w.y = v1.y * v2.y;
	w.z = v1.z * v2.z;
	return w;
}
const XMFLOAT3 operator/=(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 w = {};
	w.x = v1.x / v2.x;
	w.y = v1.y / v2.y;
	w.z = v1.z / v2.z;
	return w;
}

const XMFLOAT3 operator+=(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x + s;
	w.y = v.y + s;
	w.z = v.z + s;
	return w;
}
const XMFLOAT3 operator-=(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x - s;
	w.y = v.y - s;
	w.z = v.z - s;
	return w;
}
const XMFLOAT3 operator*=(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x * s;
	w.y = v.y * s;
	w.z = v.z * s;
	return w;
}
const XMFLOAT3 operator/=(const XMFLOAT3& v, float s)
{
	XMFLOAT3 w = {};
	w.x = v.x / s;
	w.y = v.y / s;
	w.z = v.z / s;
	return w;
}

const bool operator==(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}
const bool operator!=(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	if (v1.x != v2.x || v1.y != v2.y || v1.z != v2.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// XMFLOAT4
const XMFLOAT4 operator+(const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	XMFLOAT4 w = {};
	w.x = v1.x + v2.x;
	w.y = v1.y + v2.y;
	w.z = v1.z + v2.z;
	w.w = v1.w + v2.w;
	return w;
}
const XMFLOAT4 operator-(const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	XMFLOAT4 w = {};
	w.x = v1.x - v2.x;
	w.y = v1.y - v2.y;
	w.z = v1.z - v2.z;
	w.w = v1.w - v2.w;
	return w;
}
const XMFLOAT4 operator*(const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	XMFLOAT4 w = {};
	w.x = v1.x * v2.x;
	w.y = v1.y * v2.y;
	w.z = v1.z * v2.z;
	w.w = v1.w * v2.w;
	return w;
}
const XMFLOAT4 operator/(const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	XMFLOAT4 w = {};
	w.x = v1.x / v2.x;
	w.y = v1.y / v2.y;
	w.z = v1.z / v2.z;
	w.w = v1.w / v2.w;
	return w;
}

const XMFLOAT4 operator+(const XMFLOAT4& v, float s)
{
	XMFLOAT4 w = {};
	w.x = v.x + s;
	w.y = v.y + s;
	w.z = v.z + s;
	w.w = v.w + s;
	return w;
}
const XMFLOAT4 operator-(const XMFLOAT4& v, float s)
{
	XMFLOAT4 w = {};
	w.x = v.x - s;
	w.y = v.y - s;
	w.z = v.z - s;
	w.w = v.w - s;
	return w;
}
const XMFLOAT4 operator*(const XMFLOAT4& v, float s)
{
	XMFLOAT4 w = {};
	w.x = v.x * s;
	w.y = v.y * s;
	w.z = v.z * s;
	w.w = v.w * s;
	return w;
}
const XMFLOAT4 operator/(const XMFLOAT4& v, float s)
{
	XMFLOAT4 w = {};
	w.x = v.x / s;
	w.y = v.y / s;
	w.z = v.z / s;
	w.w = v.w / s;
	return w;
}