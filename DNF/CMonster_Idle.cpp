#include "pch.h"
#include "CMonster_Idle.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"

CMonster_Idle::CMonster_Idle(wstring _name)
	: CState(_name)
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
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	if (pPlayer)
	{
		if ((pPlayer->GetGroundPos() - pMonster->GetGroundPos()).Length() < pMonster->GetDetectRange())
		{
			GetFSM()->ChangeState((int)MonsterState::Move);
		}
	}
}

void CMonster_Idle::Exit()
{
}
