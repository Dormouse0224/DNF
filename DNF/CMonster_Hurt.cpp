#include "pch.h"
#include "CMonster_Hurt.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"

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
	time = 0;
	((CMonster*)GetOwnerObj())->SetHurt(false);
	GetOwnerObj()->SetState((int)MonsterState::Hurt);
}

void CMonster_Hurt::FinalTick()
{
	time += CTimeMgr::GetInst()->GetDeltaTime();
	if (time > 0.5)
	{
		GetFSM()->ChangeState(GetFSM()->GetPrevState());
	}
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
}

void CMonster_Hurt::Exit()
{
}
