#include "pch.h"
#include "CMonster_Move.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CRigidBody.h"

CMonster_Move::CMonster_Move(wstring _name)
	: CState(_name)
{
}

CMonster_Move::~CMonster_Move()
{
}

void CMonster_Move::Enter()
{
	GetOwnerObj()->SetState((int)MonsterState::Move);
}

void CMonster_Move::FinalTick()
{
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	if (pPlayer)
	{
		if ((pPlayer->GetGroundPos() - pMonster->GetGroundPos()).Length() < pMonster->GetAttackRange())
		{
			GetFSM()->ChangeState((int)MonsterState::Attack);
		}
		else if ((pPlayer->GetGroundPos() - pMonster->GetGroundPos()).Length() > pMonster->GetDetectRange())
		{
			GetFSM()->ChangeState((int)MonsterState::Idle);
		}
	}

	GetOwnerObj()->GetRigidBody()
		->SetSpeed((CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetGroundPos() - GetOwnerObj()->GetGroundPos()).Normalize() * m_MoveSpeed);
}

void CMonster_Move::Exit()
{
}
