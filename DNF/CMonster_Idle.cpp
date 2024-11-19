#include "pch.h"
#include "CMonster_Idle.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"

CMonster_Idle::CMonster_Idle(wstring _name)
	: CState(_name)
	, m_Timer(0)
{
}

CMonster_Idle::~CMonster_Idle()
{
}

void CMonster_Idle::Enter()
{
	GetOwnerObj()->SetState((int)MonsterState::Idle);
}

void CMonster_Idle::FinalTick()
{
	if (((CMonster*)GetOwnerObj())->GetHurt())
		GetFSM()->ChangeState((int)MonsterState::Hurt);

	m_Timer += CTimeMgr::GetInst()->GetDeltaTime();
	if (m_Timer > 2.f)
	{
		CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
		CMonster* pMonster = (CMonster*)GetOwnerObj();
		if (pPlayer)
		{
			if ((pPlayer->GetGroundPos() - pMonster->GetGroundPos()).Length() < pMonster->GetDetectRange())
			{
				m_Timer = 0;
				GetFSM()->ChangeState((int)MonsterState::Move);
			}
		}
	}
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));


}

void CMonster_Idle::Exit()
{
}
