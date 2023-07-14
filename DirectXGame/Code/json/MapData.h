#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <SafeDelete.h>

struct MapData
{
	Object3d* object = nullptr;
	XMFLOAT3 originPos = {}; // 初期座標
	float gravity = 0.0f; // 重力
	bool isMove = false; // マップが動いてるか
	int moveFrame = 0; // 動いているフレーム数

	MapData(Object3d* object3d, const XMFLOAT3& position)
	{
		object = object3d;
		originPos = position;
	}
	~MapData()
	{
		safe_delete(object);
	}
};