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

	// ���� ���̾��� �浹
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
	//�ٸ� ���̾��� �浹
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
	// �浹ü�� �浹 ������ ��Ͽ� ���
	COLLISION_ID CollisionID = {};
	CollisionID.first = _first->GetID();
	CollisionID.second = _second->GetID();

	map<ULONGLONG, bool>::iterator iter = m_ColliderCollisionList.find(CollisionID.ID);

	// �浹 ��Ͽ� ���� ��� ��Ͽ� �߰�
	if (iter == m_ColliderCollisionList.end())
	{
		iter = (m_ColliderCollisionList.insert(make_pair(CollisionID.ID, false))).first;
	}

	// �浹ü�� ������ ������Ʈ�� ���� ������ ��� �浹 �˻縦 �������� ����
	if (!(_first->GetOwner()->GetDead() || _second->GetOwner()->GetDead()))
	{
		// �� �浹ü�� ���� �浹��
		if (IsCollision(_first, _second))
		{
			// ���� �����ӿ� �浹���̾���
			if (iter->second)
			{
				_first->Overlap(_second);
				_second->Overlap(_first);
			}
			// ���� �����ӿ��� �浹������ �ʾ���
			else
			{
				_first->BeginOverlap(_second);
				_second->BeginOverlap(_first);
				iter->second = true;
			}
		}
		// �� �浹ü�� ���� �浹������ ����
		else
		{
			// ���� �����ӿ� �浹���̾���
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
