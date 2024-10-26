#include "pch.h"
#include "CCollider.h"
#include "CObj.h"
#include "CLevelMgr.h"

CCollider::CCollider(wstring _Name)
	: CComponent(_Name)
	, m_Location(Vec2D(0.f, 0.f))
	, m_Offset(Vec2D(0.f, 0.f))
	, m_OverlapCount(0)
	, m_Size(Vec2D(1.f, 1.f))
	, m_IsActive(false)
	, m_LayerType(LayerType::END)
{

}

CCollider::~CCollider()
{
}

void CCollider::BeginOverlap(CCollider* _Other)
{
	++m_OverlapCount;
	GetOwner()->BeginOverlap(this, _Other);
}

void CCollider::Overlap(CCollider* _Other)
{
	GetOwner()->Overlap(this, _Other);
}

void CCollider::EndOverlap(CCollider* _Other)
{
	--m_OverlapCount;
	GetOwner()->EndOverlap(this, _Other);
}

void CCollider::FinalTick()
{
	// 충돌체 위치 갱신
	m_Location = m_Offset + GetOwner()->GetLocation();

}
