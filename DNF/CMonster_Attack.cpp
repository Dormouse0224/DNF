#include "pch.h"
#include "CMonster_Attack.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CMonster.h"
#include "CAlbumPlayer.h"
#include "CCollider.h"
#include "CRigidBody.h"

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
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	for (int i = 0; i < pMonster->GetCurAnimation().size(); ++i)
	{
		pMonster->GetCurAnimation()[i]->SetCurrentScene(0);
	}
}

void CMonster_Attack::FinalTick()
{
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
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

	if (frame == pMonster->GetCurAnimation()[0]->GetFinal())
	{
		GetFSM()->ChangeState((int)MonsterState::Idle);
	}

}

void CMonster_Attack::Exit()
{
	CMonster* pMonster = (CMonster*)GetOwnerObj();
	pMonster->GetAttackCol()->SetActiveState(false);
}
