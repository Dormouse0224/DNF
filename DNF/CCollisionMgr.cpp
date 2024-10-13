#include "pch.h"
#include "CCollisionMgr.h"
#include "CLevelMgr.h"
#include "CCollider.h"


CCollisionMgr::CCollisionMgr()
	: m_CollisionLayer{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::AddCollisionLayer(LayerType _first, LayerType _second)
{
	int row = (int)_first;
	int col = (int)_second;
	if ((int)_first > (int)_second)
	{
		row = (int)_second;
		col = (int)_first;
	}

	if (!(m_CollisionLayer[row] & ((ULONGLONG)1 << col)))
	{
		m_CollisionLayer[row] |= ((ULONGLONG)1 << col);
	}
}

void CCollisionMgr::DeleteCollisionLayer(LayerType _first, LayerType _second)
{
	int row = (int)_first;
	int col = (int)_second;
	if ((int)_first > (int)_second)
	{
		row = (int)_second;
		col = (int)_first;
	}

	if (m_CollisionLayer[row] & ((ULONGLONG)1 << col))
	{
		m_CollisionLayer[row] &= ~((ULONGLONG)1 << col);
	}
}

void CCollisionMgr::Tick()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = i; i < sizeof(ULONGLONG) * 8; ++j)
		{
			if (m_CollisionLayer[i] & ((ULONGLONG)1 << j))
			{
				LayerCollisonCheck((LayerType)i, (LayerType)j);
			}
		}
	}
}

void CCollisionMgr::LayerCollisonCheck(LayerType _first, LayerType _second)
{
	const vector<CCollider*>& first = CLevelMgr::GetInst()->GetCurrentLevel()->GetCollider(_first);
	const vector<CCollider*>& second = CLevelMgr::GetInst()->GetCurrentLevel()->GetCollider(_second);

	// 같은 레이어의 충돌
	if (_first == _second)
	{
		for (int i = 0; i < first.size(); ++i)
		{
			for (int j = i + 1; j < second.size(); ++j)
			{
				ColliderCollision(first[i], second[j]);
			}
		}
	}
	//다른 레이어의 충돌
	else
	{
		for (int i = 0; i < first.size(); ++i)
		{
			for (int j = 0; j < second.size(); ++j)
			{
				ColliderCollision(first[i], second[j]);
			}
		}
	}
}

void CCollisionMgr::ColliderCollision(CCollider* _first, CCollider* _second)
{
	// 충돌체간 충돌 정보를 목록에 등록
	COLLISION_ID CollisionID = {};
	CollisionID.first = _first->GetID();
	CollisionID.second = _second->GetID();

	map<ULONGLONG, bool>::iterator iter = m_ColliderCollisionList.find(CollisionID.ID);

	// 충돌 목록에 없는 경우 목록에 추가
	if (iter == m_ColliderCollisionList.end())
	{
		iter = (m_ColliderCollisionList.insert(make_pair(CollisionID.ID, false))).first;
	}

	// 충돌체를 소유한 오브젝트가 삭제 예정인 경우 충돌 검사를 수행하지 않음
	if (!(_first->GetOwner()->GetDead() || _second->GetOwner()->GetDead()))
	{
		// 두 충돌체가 현재 충돌중
		if (IsCollision(_first, _second))
		{
			// 이전 프레임에 충돌중이었음
			if (iter->second)
			{
				_first->Overlap(_second);
				_second->Overlap(_first);
			}
			// 이전 프레임에는 충돌중이지 않았음
			else
			{
				_first->BeginOverlap(_second);
				_second->BeginOverlap(_first);
				iter->second = true;
			}
		}
		// 두 충돌체가 현재 충돌중이지 않음
		else
		{
			// 이전 프레임에 충돌중이었음
			if (iter->second)
			{
				_first->EndOverlap(_second);
				_second->EndOverlap(_first);
				iter->second = false;
			}
		}
	}
}


bool CCollisionMgr::IsCollision(CCollider * _first, CCollider * _second)
{
	Vec2D vect = (_second->GetLocation() + (_second->GetSize()) / 2.f) - (_first->GetLocation() + (_first->GetSize()) / 2.f);
	Vec2D benchmark = (_second->GetSize() + _first->GetSize()) / 2.f;

	if (abs(benchmark.x) > abs(vect.x) && abs(benchmark.y) > abs(vect.y))
		return true;
	else
		return false;
}
