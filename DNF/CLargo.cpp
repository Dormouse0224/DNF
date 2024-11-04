#include "pch.h"
#include "CLargo.h"
#include "CAlbumPlayer.h"
#include "CLargo_Attack.h"

CLargo::CLargo()
	: CObj(L"Largo")
	, m_Animation{}
	, m_bIncomeFin(false)
	, m_AttackState(nullptr)
{
}

CLargo::~CLargo()
{
}

void CLargo::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::Begin()
{
}

void CLargo::Tick()
{
}

void CLargo::Render()
{
	if (GetState() == (int)LargoState::Attack)
	{
		m_AttackState->Render();
	}
	else
	{
		for (int i = 0; i < m_Animation[GetState()].size(); ++i)
		{
			m_Animation[GetState()][i]->Render(this, m_bFallowCam, m_bLookLeft);
		}
	}

	
	if (GetState() == (int)LargoState::Income && m_Animation[GetState()][0]->GetCurSceneNum() == m_Animation[GetState()][0]->GetFinal())
	{
		m_bIncomeFin = true;
	}
	
}
