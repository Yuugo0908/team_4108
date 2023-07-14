#pragma once
#include "Object3d.h"
#include "Operator.h"
#include <SafeDelete.h>

struct MapData
{
	Object3d* object = nullptr;
	XMFLOAT3 originPos = {}; // �������W
	float gravity = 0.0f; // �d��
	bool isMove = false; // �}�b�v�������Ă邩
	int moveFrame = 0; // �����Ă���t���[����

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