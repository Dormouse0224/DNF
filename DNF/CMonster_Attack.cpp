#include "pch.h"
#include "CMonster_Attack.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CAlbumPlayer.h"
#include "CCollider.h"

CMonster_Attack::CMonster_Attack(wstring _name)
	: CState(_name)
{
}

CMonster_Attack::~CMonster_Attack()
{
}

void CMonster_Attack::Enter()
{
	GetOwnerObj()->SetState((int)MonsterState::Attack);
}

void CMonster_Attack::FinalTick()
{
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	int frame = pMonster->GetCurAnimation()[0]->GetCurSceneNum();
	if (frame >= pMonster->GetAttackFrame().first && frame <= pMonster->GetAttackFrame().second)
	{
		pMonster->GetAttackCol()->SetActiveState(true);
	}
	else
	{
		pMonster->GetAttackCol()->SetActiveState(false);
	}
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
	if (pPlayer)
	{
		if ((pPlayer->GetGroundPos() - pMonster->GetGroundPos()).Length() > pMonster->GetAttackRange())
		{
			GetFSM()->ChangeState((int)MonsterState::Move);
		}
	}
}

void CMonster_Attack::Exit()
{
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	pMonster->GetAttackCol()->SetActiveState(false);
}
