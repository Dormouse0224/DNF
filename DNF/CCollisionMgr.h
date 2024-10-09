#pragma once

class CCollider;

union COLLISION_ID
{
	struct
	{
		UINT first;
		UINT second;
	};

	ULONGLONG ID;
};


class CCollisionMgr
{
	SINGLETON(CCollisionMgr)
private:
	ULONGLONG m_CollisionLayer[(UINT)LayerType::END];
	map<ULONGLONG, bool> m_ColliderCollisionList;		// ULONGLONG: 충돌 ID 값, bool: 이전 프레임 충돌 여부

public:
	void AddCollisionLayer(LayerType _first, LayerType _second);
	void DeleteCollisionLayer(LayerType _first, LayerType _second);
	void ClearCollisionLayer() { memset(m_CollisionLayer, 0, sizeof(ULONGLONG) * (UINT)LayerType::END); m_ColliderCollisionList.clear(); }
	void Tick();

private:
	void LayerCollisonCheck(LayerType _first, LayerType _second);
	void ColliderCollision(CCollider* _first, CCollider* _second);
	bool IsCollision(CCollider* _first, CCollider* _second);
};


