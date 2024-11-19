#include "pch.h"
#include "CPlayerSkill.h"
#include "CMonster.h"
#include "CLargo.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"

CPlayerSkill::CPlayerSkill(wstring _name, float _Lifetime)
	: CObj(_name)
	, m_EffAni{}
	, m_ATK(0)
	, m_DotATK(0)
	, m_FinATK(0)
	, m_DotDuration(0.f)
	, m_DotTimer(0.f)
	, m_Lifetime(_Lifetime)
	, m_EffSound(nullptr)
	, m_bSoundPlayed(false)
{
}

CPlayerSkill::~CPlayerSkill()
{
	for (int i = 0; i < (int)SkillEffLayer::END; ++i)
	{
		delete m_EffAni[i];
	}
}

void CPlayerSkill::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
	CMonster* pMon = dynamic_cast<CMonster*>(_Other->GetOwner());
	CLargo* pLargo = dynamic_cast<CLargo*>(_Other->GetOwner());
	if (pMon)
	{
		pMon->GiveDamage(m_ATK);
	}
	else if (pLargo)
	{
		pLargo->GiveDamage(m_ATK);
	}
}

void CPlayerSkill::Overlap(CCollider* _Self, CCollider* _Other)
{
	m_DotTimer += CTimeMgr::GetInst()->GetDeltaTime();
	if (m_DotDuration < m_DotTimer)
	{
		m_DotTimer -= m_DotDuration;
		CMonster* pMon = dynamic_cast<CMonster*>(_Other->GetOwner());
		CLargo* pLargo = dynamic_cast<CLargo*>(_Other->GetOwner());
		if (pMon)
		{
			if (pMon->GetBodyCollider() == _Other)
				pMon->GiveDamage(m_DotATK);
		}
		else if (pLargo)
		{
			pLargo->GiveDamage(m_DotATK);
		}
	}

	if (m_Lifetime < 0)
	{
		CMonster* pMon = dynamic_cast<CMonster*>(_Other->GetOwner());
		CLargo* pLargo = dynamic_cast<CLargo*>(_Other->GetOwner());
		if (pMon)
		{
			pMon->GiveDamage(m_FinATK);
		}
		else if (pLargo)
		{
			pLargo->GiveDamage(m_FinATK);
		}
	}
}

void CPlayerSkill::EndOverlap(CCollider* _Self, CCollider* _Other)
{
	
}

void CPlayerSkill::Begin()
{
}

void CPlayerSkill::Tick()
{
	for (int i = 0; i < (int)SkillEffLayer::END; ++i)
	{
		if (m_EffAni[i])
			m_EffAni[i]->FinalTick();
	}

	m_Lifetime -= CTimeMgr::GetInst()->GetDeltaTime();
	if (m_Lifetime < 0)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
	}
}

void CPlayerSkill::Render()
{
	for (int i = 0; i < (int)SkillEffLayer::END; ++i)
	{
		if (m_EffAni[i])
			m_EffAni[i]->Render(this, false, m_bLookLeft);
	}
}
