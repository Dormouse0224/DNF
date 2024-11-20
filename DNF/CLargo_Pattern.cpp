#include "pch.h"
#include "CLargo_Pattern.h"
#include "CCameraMgr.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CPlayer.h"
#include "CKeyMgr.h"


CLargo_Pattern::CLargo_Pattern()
	: CState(L"Largo_Pattern")
	, m_PatternTimer(5)
{
}

CLargo_Pattern::~CLargo_Pattern()
{
}

void CLargo_Pattern::Enter()
{
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();

	GetOwnerObj()->SetState((int)LargoState::Pattern);
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
	pPlayer->SetImmune(true);
	pPlayer->SetYogiGauge(0);
	GetOwnerObj()->SetImmune(true);
	CCameraMgr::GetInst()->SetAlpha(150);
}

void CLargo_Pattern::FinalTick()
{
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
	pPlayer->SetYogiGauge(0);
	if (m_PatternTimer > 0)
	{
		m_PatternTimer -= CTimeMgr::GetInst()->GetDeltaTime();
		// 절단 패턴 도중
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::X) == Key_state::TAP)
		{
			GetFSM()->ChangeState((int)LargoState::Attack);
		}

	}
	else
	{
		// 타이머 종료까지 절단패턴 파훼 실패 시
		pPlayer->SetImmune(false);
		pPlayer->GiveDamage(pPlayer->GetMaxHP() * 1.1f);
		GetOwnerObj()->SetImmune(false);
		GetFSM()->ChangeState((int)LargoState::Attack);
	}
}

void CLargo_Pattern::Exit()
{
	CCameraMgr::GetInst()->SetAlpha(0);
	m_PatternTimer = 5;
}
