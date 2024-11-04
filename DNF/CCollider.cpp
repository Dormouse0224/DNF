#include "pch.h"
#include "CCollider.h"
#include "CObj.h"
#include "CLevelMgr.h"
#include "CDbgRender.h"

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
	if (GetOwner()->IsLookLeft())
	{
		m_RelativeOffset = Vec2D(GetOwner()->GetScale().x - m_Offset.x - m_Size.x, m_Offset.y);
	}
	else
	{
		m_RelativeOffset = m_Offset;
	}


	// �浹ü ��ġ ����
	m_Location = m_RelativeOffset + GetOwner()->GetLocation();

	// Collider ����ϱ�
	LayerType layertype = GetOwner()->GetLayerType();
	CLevelMgr::GetInst()->GetCurrentLevel()->AddCollider(this, layertype);

	// �浹ü ����� ������
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Rectangle, m_Location
		, m_Size, 0, Color(255, 255, 0, 0));
}

void CCollider::EraseFromLevelVec()
{
	CLevelMgr::GetInst()->GetCurrentLevel()->EraseCollider(GetID(), m_LayerType);
}
