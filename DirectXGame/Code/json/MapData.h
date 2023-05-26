#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <SafeDelete.h>

struct MapData
{
	Object3d* object;
	XMFLOAT3 originPos = {};

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