#include "Bullet.h"

bool Bullet::Initialize(Model* bulletModel, const XMFLOAT3& pPos, const XMFLOAT3& ePos)
{
	this->pPos = pPos;
	this->ePos = ePos;
	bPos = ePos;
	bScale = { 0.4f, 0.4f, 0.4f };
	bOldPos = bPos;

	Search();

	bulletObj = Object3d::Create();
	bulletObj->SetModel(bulletModel);
	bulletObj->SetPosition(bPos);
	bulletObj->SetScale(bScale);
	return true;
}

void Bullet::Update()
{
	bOldPos = bPos;

	bPos.x += bSpeed.x / 3;
	bPos.y += bSpeed.y / 3;
	bPos.z += bSpeed.z / 3;

	bulletObj->SetPosition(bPos);
	bulletObj->Update();
}

void Bullet::Search()
{
	XMVECTOR playerPos = { pPos.x, pPos.y, pPos.z, 1 };
	XMVECTOR bulletPos = { ePos.x, ePos.y, ePos.z, 1 };

	XMVECTOR subPlayerEnemy = XMVectorSubtract(playerPos, bulletPos);
	XMVECTOR NsubPlayerEnemy = XMVector3Normalize(subPlayerEnemy);

	bSpeed = { NsubPlayerEnemy.m128_f32[0], NsubPlayerEnemy.m128_f32[1], NsubPlayerEnemy.m128_f32[2] };
}

void Bullet::Draw()
{
	bulletObj->Draw();
}

bool Bullet::MapCollide(XMFLOAT3 boxPos, XMFLOAT3 boxScale)
{
	// ”»’è
	float maxBoxX = boxPos.x + boxScale.x;
	float minBoxX = boxPos.x - boxScale.x;
	float maxBoxY = boxPos.y + boxScale.y;
	float minBoxY = boxPos.y - boxScale.y;
	float maxBoxZ = boxPos.z + boxScale.z;
	float minBoxZ = boxPos.z - boxScale.z;

	if ((bPos.x <= maxBoxX && bPos.x >= minBoxX) &&
		(bPos.z <= maxBoxZ && bPos.z >= minBoxZ))
	{
		if (maxBoxY + bScale.y > bPos.y && boxPos.y < bOldPos.y)
		{
			return true;
		}
		else if (minBoxY - bScale.y < bPos.y && boxPos.y > bOldPos.y)
		{
			return true;
		}
	}

	if ((bPos.x <= maxBoxX && bPos.x >= minBoxX) &&
		(bPos.y <= maxBoxY && bPos.y >= minBoxY))
	{
		if (maxBoxZ + bScale.z > bPos.z && boxPos.z < bOldPos.z)
		{
			return true;
		}
		else if (minBoxZ - bScale.z < bPos.z && boxPos.z > bOldPos.z)
		{
			return true;
		}
	}

	if ((bPos.z <= maxBoxZ && bPos.z >= minBoxZ) &&
		(bPos.y <= maxBoxY && bPos.y >= minBoxY))
	{
		if (maxBoxX + bScale.x > bPos.x && boxPos.x < bOldPos.x)
		{
			return true;
		}
		else if (minBoxX - bScale.x < bPos.x && boxPos.x > bOldPos.x)
		{
			return true;
		}
	}

	return false;
}
