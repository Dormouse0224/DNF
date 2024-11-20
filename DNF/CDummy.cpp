#include "pch.h"
#include "CDummy.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"

CDummy::CDummy(wstring _name)
	: CObj(_name)
	, m_SuicideTimer(0)
	, m_bTimerOn(false)
{
}

CDummy::~CDummy()
{
}

void CDummy::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::Begin()
{
}

void CDummy::Tick()
{
	if (m_SuicideTimer > 0)
	{
		m_bTimerOn = true;
		m_SuicideTimer -= CTimeMgr::GetInst()->GetDeltaTime();
	}

	if (m_bTimerOn && m_SuicideTimer <= 0)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
	}
}

void CDummy::Render()
{
	CObj::Render();
}
