#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <SafeDelete.h>

struct MapData
{
	Object3d* object = nullptr;
	XMFLOAT3 originPos = {};
	float gravity = 0.0f;
	bool isMove = false;
	int moveFrame = 0;

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