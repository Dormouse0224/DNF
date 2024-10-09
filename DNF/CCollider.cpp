#include "pch.h"
#include "CCollider.h"
#include "CObj.h"
#include "CLevelMgr.h"

CCollider::CCollider(wstring _Name)
	: CComponent(_Name)
	, m_Location(GetOwner()->GetLocation())
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
	// �浹ü ��ġ ����
	m_Location = m_Offset + GetOwner()->GetLocation();

	// �浹ü�� ���� ������ �浹ü ���Ϳ� ���
	m_LayerType = GetOwner()->GetLayerType();
	CLevelMgr::GetInst()->GetCurrentLevel()->AddCollider(this, m_LayerType);

}
