#include "pch.h"
#include "CMonster_Hurt.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"

CMonster_Hurt::CMonster_Hurt(wstring _name)
	: CState(_name)
	, time(0.f)
{
}

CMonster_Hurt::~CMonster_Hurt()
{
}

void CMonster_Hurt::Enter()
{
	GetOwnerObj()->SetState((int)MonsterState::Hurt);
}

void CMonster_Hurt::FinalTick()
{
	time += CTimeMgr::GetInst()->GetDeltaTime();
	if (time > 1)
	{
		GetFSM()->ChangeState(GetFSM()->GetPrevState());
	}
}

void CMonster_Hurt::Exit()
{
}
