#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <vector>
class Collision
{
public: // サブクラス
	struct Sphere
	{
		// 中心座標
		XMVECTOR center = { 0, 0, 0 };
		// 半径
		float radius = 1.0f;
	};
	struct Plane
	{
		// 法線ベクトル
		XMVECTOR normal = { 0, 0, 0 };
		// 原点からの距離
		float distance = 0.0f;
	};
	struct Ray
	{
		// 始点座標
		XMVECTOR start = { 0, 0, 0 };
		// 方向
		XMVECTOR dir = { 1, 0, 0 };
	};
	struct Triangle
	{
		// 頂点座標
		XMVECTOR p0;
		XMVECTOR p1;
		XMVECTOR p2;
		// 法線ベクトル
		XMVECTOR normal;
	};

public: // 静的メンバ関数

	//オブジェクト同士の当たり判定
	static bool CollisionObject(const std::unique_ptr<Object3d>& object_a, const std::unique_ptr<Object3d>& object_b);
	// 球と球の当たり判定
	static bool CollisionSphere(const Sphere& sphere1, const Sphere& sphere2);
	// 球と平面の当たり判定
	static bool CollisionSpherePlane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter);

	// レイと平面の当たり判定
	static bool CollisionRayPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, XMVECTOR* inter = nullptr);
	// レイと球の当たり判定
	static bool CollisionRaySphere(const Ray& ray, const Sphere& sphere);
	// レイとBOXの当たり判定
	static bool CollisionRayBox(const XMFLOAT3 startPos, const XMFLOAT3 endPos, const XMFLOAT3 boxPos, const XMFLOAT3 boxScale);

	// BOXと点の当たり判定
	static bool CollisionBoxPoint(const XMFLOAT3 boxPos, const XMFLOAT3 boxRadius, XMFLOAT3& pos, const XMFLOAT3 radius, XMFLOAT3 oldPos);
	
	// 地面との当たり判定
	static bool CollisionStage(const XMFLOAT3 stagePos, const XMFLOAT3 stageRadius, XMFLOAT3& pos, const XMFLOAT3 radius, XMFLOAT3 oldPos);
	// オブジェクト同士の距離を取得
	static float GetLength(XMFLOAT3 pos_a, XMFLOAT3 pos_b);
};

